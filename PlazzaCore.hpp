//
// Created by gaspar_q on 4/12/16.
//

#ifndef CPP_PLAZZA_PLAZZACORE_HPP
# define CPP_PLAZZA_PLAZZACORE_HPP

# include <string>
# include <list>
# include <sstream>
# include <map>
# include "NamedPipe.hpp"
# include "Regex.hpp"
# include "Process.hpp"
# include "Order.hpp"
# include "OrdersDispatcher.hpp"
#include "Semaphore.hpp"

namespace plazza
{
    const std::string suffixDatas = "datas.fifo";
    const std::string suffixOrders = "orders.fifo";

    class PlazzaCore
    {
    public:
        PlazzaCore(size_t threadNbr);

        ~PlazzaCore();

    public:
        void run();

        template<typename stream>
        void resolveCommandLine(std::string command, stream &output, NamedPipe *log = NULL)
        {
            launchDispatching(command, log);
            onReceiveOutput(output);
        }

    private:
        bool allProcessAreSaturated() const;

        OrdersDispatcher *createOrdersDispatcher(NamedPipe *log = NULL);

        OrdersDispatcher *getProcessWithTheLowestActivity() const;

        void cleanDispatchers();

    private:
        template<typename stream>
        void onReceiveOutput(stream &output)
        {
            std::string data;

            for (std::list<OrdersDispatcher *>::iterator it = poolDispatcher.begin();
                 it != poolDispatcher.end(); ++it)
            {
                try
                {
                    data = (*it)->receiveOutput();
                    if (!data.empty())
                    {
                        writeSemaphora.Lock();
                        output << data << "\n";
                        writeSemaphora.Unlock();
                    }
                }
                catch (OrdersDispatcher::TimeoutException &exc)
                {
                    writeSemaphora.Lock();
                    delete *it;
                    it = poolDispatcher.erase(it);
                    std::cerr << "Plazza core: Receive output: " << exc.what() << std::endl;
                    writeSemaphora.Unlock();
                }
            }
        }

        void launchDispatching(std::string, NamedPipe *log = NULL);

    private:
        std::list<OrdersDispatcher *> poolDispatcher;

    private:
        const size_t threadNbr;
        Semaphore writeSemaphora;
    };
};

#endif //CPP_PLAZZA_PLAZZACORE_HPP
