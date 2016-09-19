//
// Created by gaspar_q on 4/12/16.
//

#include <ctime>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include "OrdersDispatcher.hpp"
#include "NamedPipe.hpp"
#include "Order.hpp"
#include "PlazzaCore.hpp"
#include "ResolvedData.hpp"

const size_t OrdersDispatcher::communicationTimeout = 10;
const size_t OrdersDispatcher::activityTimeout = 5;

OrdersDispatcher::OrdersDispatcher(size_t poolSize) :
        ordersChannel(NULL),
        datasChannel(NULL),
        threadPool(NULL),
        process(),
        orderNb(0)
{
    threadPool = new ThreadPool<OrderResolver>(poolSize);
}

OrdersDispatcher::~OrdersDispatcher()
{
    if (ordersChannel)
    {
        ordersChannel->unlinkPipe();
        delete (ordersChannel);
    }
    if (datasChannel)
    {
        datasChannel->unlinkPipe();
        delete (datasChannel);
    }
    if (!process.isChildProcess())
        process.killSon();
    delete(threadPool);
}

OrdersDispatcher::OrdersDispatcher(const OrdersDispatcher &dispatcher)
{
    *this = dispatcher;
}

OrdersDispatcher &OrdersDispatcher::operator=(const OrdersDispatcher &dispatcher)
{
    threadPool = dispatcher.threadPool;
    process = dispatcher.process;
    return *this;
}

/**
 * \brief Launch the process dispatcher
 */
void OrdersDispatcher::Launch()
{
    process.launch(ActionHandler<OrdersDispatcher>(*this, &OrdersDispatcher::Dispatch));
    //name pipe pour les dire les ordres
    ordersChannel = new NamedPipe(std::to_string(process.getPid()) + plazza::suffixOrders, NamedPipe::OUT);
    //name pipe pour recupérer la data
    datasChannel = new NamedPipe(std::to_string(process.getPid()) + plazza::suffixDatas, NamedPipe::IN);
    datasChannel->setTimeout(OrdersDispatcher::communicationTimeout);
}


/**
 * \brief Get the orders on his named pipe channel and launch thread for getting the data
 */
void                            OrdersDispatcher::Dispatch(void)
{
    std::string                 order;
    ResolvedData                toUnserialize;
    std::vector<std::string>    orders;
    std::time_t                 ref = std::time(NULL);
    Semaphore                   writeSemaphore;

    //name pipe pour récupérer les ordres
    ordersChannel = new NamedPipe(std::to_string(getpid()) + plazza::suffixOrders, NamedPipe::IN);
    //name pipe pour envoyer la data
    datasChannel = new NamedPipe(std::to_string(getpid()) + plazza::suffixDatas, NamedPipe::OUT);
    ordersChannel->setTimeout(1);
    threadPool->launch(datasChannel);
    *threadPool << &OrderResolver::resolve;
    threadPool->runThreads();
    while (1)
    {
        *ordersChannel >> order;
        if (threadPool->isCorrupted())
            break;
        if (!order.empty())
        {
            OrderResolver *resolver;

            ref = std::time(NULL);
            try
            {
                toUnserialize.unserialize(order);
                orders = toUnserialize.getDatas();
                for (std::vector<std::string>::iterator ord = orders.begin(), endOrd = orders.end(); ord != endOrd; ++ord)
                {
                    if ((resolver = getThreadWithLowestActivity()) != NULL)
                    {
                        *resolver << *ord;
                    }
                }
            }
            catch (ResolvedData::SyntaxErrorException   &exc)
            {
                writeSemaphore.Lock();
                std::cerr << "Order unserialization: " << exc.what() << std::endl;
                writeSemaphore.Unlock();
            }
        }
        else if (allThreadAreInactive())
        {
            if (static_cast<size_t >(std::time(NULL) - ref) >= OrdersDispatcher::activityTimeout)
            {
                break;
            }
        }
        else
        {
            ref = std::time(NULL);
        }
    }
    stopSolvers();
    threadPool->joinThreads();
    delete(threadPool);
}

bool                    OrdersDispatcher::allThreadAreInactive() const
{
    std::vector<OrderResolver *> solvers;

    solvers = threadPool->getThreadsElements();
    for (std::vector<OrderResolver *>::iterator it = solvers.begin(), end = solvers.end(); it != end; ++it)
    {
        if ((*it)->getStackLen() > 0)
        {
            return false;
        }
    }
    return (true);
}

bool OrdersDispatcher::isSaturated() const
{
    return orderNb == threadPool->getPoolSize() * 2;
}

pid_t OrdersDispatcher::getPid() const
{
    return process.getPid();
}

void OrdersDispatcher::updatePoolActivity()
{
    threadPool->updateActivity();
}

void OrdersDispatcher::OrderToSon(std::string const &orderName)
{
    ResolvedData dataToOrder;

    dataToOrder.addData(orderName);
    *ordersChannel << dataToOrder.serialize();
    ++orderNb;
}

std::string         OrdersDispatcher::receiveOutput()
{
    std::string     res;
    ResolvedData    toGet;
    std::string     buff;

    while (orderNb)
    {
        *datasChannel >> buff;
        if (!buff.empty())
        {
            toGet.unserialize(buff);
            if (!(buff = toGet.toOutput()).empty())
            {
                if (!res.empty())
                    res += "\n";
                res += buff;
            }
            orderNb -= toGet.getDatas().size();
        }
        else
        {
            if (res.empty())
                throw TimeoutException();
            break;
        }
    }
    return res;
}

OrderResolver *OrdersDispatcher::getThreadWithLowestActivity() const
{
    std::vector<OrderResolver *> solvers;
    OrderResolver   *toReturn = NULL;

    solvers = threadPool->getThreadsElements();
    for (std::vector<OrderResolver *>::iterator it = solvers.begin(), end = solvers.end(); it != end; ++it)
    {
        if (toReturn == NULL || *toReturn > **it)
        {
            toReturn = *it;
        }
    }
    return toReturn;
}

bool OrdersDispatcher::operator>(const OrdersDispatcher &dispatcher) const
{
    return orderNb > dispatcher.orderNb;
}

bool OrdersDispatcher::operator<(const OrdersDispatcher &dispatcher) const
{
    return orderNb < dispatcher.orderNb;
}

bool OrdersDispatcher::operator>=(const OrdersDispatcher &dispatcher) const
{
    return orderNb >= dispatcher.orderNb;
}

bool OrdersDispatcher::operator<=(const OrdersDispatcher &dispatcher) const
{
    return orderNb <= dispatcher.orderNb;
}

bool OrdersDispatcher::isAlive() const
{
    return process.isAlive();
}

void OrdersDispatcher::stopSolvers()
{
    std::vector<OrderResolver *>  solvers = threadPool->getThreadsElements();

    for (std::vector<OrderResolver *>::iterator it = solvers.begin(), end = solvers.end(); it != end; ++it)
    {
        (*it)->stop();
    }
}
