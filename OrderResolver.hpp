//
// Created by gaspar_q on 4/13/16.
//

#ifndef CPP_PLAZZA_ORDERRESOLVER_HPP
#define CPP_PLAZZA_ORDERRESOLVER_HPP

#include <queue>
#include <map>
#include "Order.hpp"
#include "Thread.hpp"
#include "Mutex.hpp"
#include "Regex.hpp"
#include "CondVar.hpp"

class OrderResolver
{
public:
    OrderResolver();
    OrderResolver(OrderResolver const &);
    ~OrderResolver();
    OrderResolver   &operator=(OrderResolver const &);

public:
    thread::State   resolve(void *);
    OrderResolver   &operator<<(std::string const &) throw(std::overflow_error);
    bool            isSaturated() const;
    size_t          getStackLen() const;
    void            start();
    void            stop();
    bool            isRunning() const;

public:
    bool            operator>(OrderResolver const &);
    bool            operator>=(OrderResolver const &);
    bool            operator<(OrderResolver const &);
    bool            operator<=(OrderResolver const &);

private:
    size_t                              stackLen;
    bool                                running;
    bool                                waiting;
    mutable Mutex                       runningMutex;
    mutable Mutex                       waintingMutex;
    std::queue<std::string>             orderQueue;
    std::map<Order::Information, Regex> toGet;
    mutable CondVar                     condVar;
};

#endif //CPP_PLAZZA_ORDERRESOLVER_HPP
