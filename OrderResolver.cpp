//
// Created by gaspar_q on 4/13/16.
//

#include "OrderResolver.hpp"
#include "FileDecryptor.hpp"
#include "NamedPipe.hpp"
#include "PlazzaCore.hpp"
#include "CondVar.hpp"
#include "ResolvedData.hpp"

OrderResolver::OrderResolver() :
        stackLen(0),
        running(false),
        waiting(false)
{
    toGet[Order::EMAIL_ADDRESS] = Regex("[A-Z0-9a-z\\._\\-]+\\@[A-Za-z0-9\\.\\-]+\\.[A-Za-z]{2,}");
    //TODO fix regex IP nicquel mais broken en C
    toGet[Order::IP_ADDRESS] = Regex("([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\.([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\.([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\.([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])");
    //todo peut être un +33
    toGet[Order::PHONE_NUMBER] = Regex("0[1-9]\\ ?[0-9]{2}\\ ?[0-9]{2}\\ ?[0-9]{2}\\ ?[0-9]{2}");
}

OrderResolver::OrderResolver(const OrderResolver &resolver)
{
    *this = resolver;
}

OrderResolver::~OrderResolver()
{

}

OrderResolver &OrderResolver::operator=(const OrderResolver &resolver)
{
    condVar.Lock();
    stackLen = resolver.stackLen;
    orderQueue = resolver.orderQueue;
    condVar.Unlock();
    return *this;
}

thread::State       OrderResolver::resolve(void *pipe)
{
    Order           toResolve;
    ResolvedData    dataToPrint;
    FileDecryptor   decryptor;
    std::vector<std::string> matches;
    std::string     res;
    std::list<std::string>  files;
    Semaphore       writeSemaphora;
;
    writeSemaphora.Lock();
    NamedPipe       *output = static_cast<NamedPipe *>(pipe);
    writeSemaphora.Unlock();

    start();
    condVar.Lock();
    while (1)
    {
        if (orderQueue.empty())
        {
            condVar.Wait();
        }
        if (!isRunning())
            break;
        dataToPrint.clear();
        res.clear();
        try
        {
            toResolve.clear();
            toResolve.evalutation(orderQueue.front());
            files = toResolve.getFileContainers();
            for (std::list<std::string>::iterator it = files.begin(), end = files.end(); it != end; ++it)
            {
                try
                {
                    decryptor.setFilename(*it);
                    decryptor.tryDecryption(toGet[toResolve.getInfo()]);
                    if (toResolve.getInfo() == Order::IP_ADDRESS)
                        matches = toGet[toResolve.getInfo()].search(decryptor.getUncryptedFile(), 0, 5);
                    else
                        matches = toGet[toResolve.getInfo()].search(decryptor.getUncryptedFile());
                    for (std::vector<std::string>::iterator it1 = matches.begin(), end1 = matches.end(); it1 != end1; ++it1)
                    {
                        if (!res.empty())
                            res += '\n';
                        res += *it1;
                    }
                }
                catch (std::runtime_error &exc)
                {
                    writeSemaphora.Lock();
                    std::cerr << "Decryption: " << exc.what() << std::endl;
                    writeSemaphora.Unlock();
                }
            }
        }
        catch (std::runtime_error const &exc)
        {
            writeSemaphora.Lock();
            std::cerr << "Commande invalide: " << exc.what() << std::endl;
            writeSemaphora.Unlock();
        }
        dataToPrint.addData(res);
        condVar.Unlock();
        writeSemaphora.Lock();
        *output << dataToPrint.serialize();
        writeSemaphora.Unlock();
        condVar.Lock();
        --stackLen;
        orderQueue.pop();
    }
    condVar.Unlock();
    return thread::Stop;
}

OrderResolver &OrderResolver::operator<<(std::string const &order) throw(std::overflow_error)
{
    if (isSaturated())
        throw std::overflow_error("OrderResolver: Cannot take a third order");
    condVar.Lock();
    orderQueue.push(order);
    ++stackLen;
    condVar.WakeUp();
    condVar.Unlock();
    return *this;
}

bool OrderResolver::isSaturated() const
{
    bool cpy;

    condVar.Lock();
    cpy = stackLen == 2;
    condVar.Unlock();
    return cpy;
}

size_t OrderResolver::getStackLen() const
{
    size_t cpylen;

    condVar.Lock();
    cpylen = stackLen;
    condVar.Unlock();
    return cpylen;
}

bool OrderResolver::operator>(const OrderResolver &resolver)
{
    return getStackLen() > resolver.getStackLen();
}

bool OrderResolver::operator>=(const OrderResolver &resolver)
{
    return getStackLen() >= resolver.getStackLen();
}

bool OrderResolver::operator<(const OrderResolver &resolver)
{
    return getStackLen() < resolver.getStackLen();
}

bool OrderResolver::operator<=(const OrderResolver &resolver)
{
    return getStackLen() <= resolver.getStackLen();
}

void OrderResolver::stop()
{
    condVar.Lock();
    runningMutex.Lock();
    running = false;
    runningMutex.Unlock();
    stackLen = 1;
    condVar.WakeUp();
    condVar.Unlock();
}

void OrderResolver::start()
{
    runningMutex.Lock();
    running = true;
    runningMutex.Unlock();
}

bool OrderResolver::isRunning() const
{
    bool cpy;

    runningMutex.Lock();
    cpy = running;
    runningMutex.Unlock();
    return cpy;
}