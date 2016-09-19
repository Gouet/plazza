//
// Created by gaspar_q on 4/12/16.
//

#ifndef CPP_PLAZZA_ORDERSDISPATCHER_HPP
#define CPP_PLAZZA_ORDERSDISPATCHER_HPP

#include <stddef.h>
#include "ThreadPool.hpp"
#include "FileDecryptor.hpp"
#include "Process.hpp"
#include "NamedPipe.hpp"
#include "OrderResolver.hpp"
#include "CondVar.hpp"

class OrdersDispatcher
{
public:
    class TimeoutException : public std::runtime_error
    {
    public:
        TimeoutException() :
                std::runtime_error("Timeout")
        {

        }

        virtual ~TimeoutException()
        {

        }
    };

public:
    OrdersDispatcher(size_t poolSize);

    OrdersDispatcher(OrdersDispatcher const &);

    ~OrdersDispatcher();

    OrdersDispatcher &operator=(OrdersDispatcher const &);

    static const size_t communicationTimeout;
    static const size_t activityTimeout;

public:
    void Launch();

    void Dispatch();

    void OrderToSon(std::string const &orderName);

    std::string receiveOutput();

    bool                        operator>(OrdersDispatcher const &) const;

    bool                        operator<(OrdersDispatcher const &) const;

    bool                        operator>=(OrdersDispatcher const &) const;

    bool                        operator<=(OrdersDispatcher const &) const;

    bool isSaturated() const;

    pid_t getPid() const;

    void updatePoolActivity();

    bool isAlive() const;

private:
    OrderResolver *getThreadWithLowestActivity() const;

    bool allThreadAreInactive() const;

    void stopSolvers();

private:
    NamedPipe *ordersChannel;
    NamedPipe *datasChannel;
    ThreadPool<OrderResolver> *threadPool;
    Process process;
    size_t orderNb;
    CondVar condVar;
};

#endif //CPP_PLAZZA_ORDERSDISPATCHER_HPP
