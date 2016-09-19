//
// Created by gaspar_q on 4/12/16.
//

#include "PlazzaCore.hpp"

plazza::PlazzaCore::PlazzaCore(size_t threadNbr) : threadNbr(threadNbr)
{
}

plazza::PlazzaCore::~PlazzaCore()
{
    for (std::list<OrdersDispatcher *>::iterator it = poolDispatcher.begin(), end = poolDispatcher.end();
         it != end; ++it)
    {
        delete *it;
    }
}

bool        plazza::PlazzaCore::allProcessAreSaturated() const
{
    for (std::list<OrdersDispatcher *>::const_iterator it = poolDispatcher.begin(),
                 end = poolDispatcher.end();
         it != end; ++it)
    {
        if (!(*it)->isSaturated())
            return (false);
    }
    return (true);
}

OrdersDispatcher *plazza::PlazzaCore::createOrdersDispatcher(NamedPipe *log)
{
    OrdersDispatcher *newOrdersDispatcher = new OrdersDispatcher(threadNbr);

    poolDispatcher.push_back(newOrdersDispatcher);
    newOrdersDispatcher->Launch();
    if (log)
    {
        *log << std::to_string(newOrdersDispatcher->getPid());
        *log << "\n";
    }
    return (newOrdersDispatcher);
}

OrdersDispatcher *plazza::PlazzaCore::getProcessWithTheLowestActivity() const
{
    OrdersDispatcher *ordersDispatcher = NULL;

    for (std::list<OrdersDispatcher *>::const_iterator it = poolDispatcher.begin(),
                 end = poolDispatcher.end();
         it != end; ++it)
    {
        if (!ordersDispatcher || *ordersDispatcher > **it)
        {
            ordersDispatcher = *it;
        }
    }
    return (ordersDispatcher);
}

void                plazza::PlazzaCore::run()
{
    std::string     commands;

    while (std::getline(std::cin, commands))
    {
        resolveCommandLine(commands, std::cout);
        std::cout.flush();
    }

}

void plazza::PlazzaCore::cleanDispatchers()
{
    std::list<OrdersDispatcher *>::iterator it = poolDispatcher.begin();

    while (it != poolDispatcher.end())
    {
        if (!(*it)->isAlive())
        {
            OrdersDispatcher *toRm;

            toRm = *it;
            poolDispatcher.erase(it);
            delete(toRm);
            it = poolDispatcher.begin();
        }
        else
        {
            ++it;
        }
    }
}

void plazza::PlazzaCore::launchDispatching(std::string commandLine, NamedPipe *log)
{
    size_t pos = 0;
    OrdersDispatcher *orderDispatcher = NULL;
    std::string     order;

    while ((pos = commandLine.find(';')) != std::string::npos || !commandLine.empty())
    {
        if (pos == std::string::npos)
            pos = commandLine.size();
        order = commandLine.substr(0, pos);
        commandLine.erase(0, pos + 1);

        cleanDispatchers();
        if (allProcessAreSaturated())
        {
            orderDispatcher = createOrdersDispatcher(log);
        }
        else
        {
            orderDispatcher = getProcessWithTheLowestActivity();
        }

        if (orderDispatcher)
        {
            try
            {
                orderDispatcher->OrderToSon(order);
            }
            catch (OrdersDispatcher::TimeoutException timeoutException)
            {
                std::cerr << "Ordering to son: " << timeoutException.what() << std::endl;
                return;
            }
        }
    }
}


