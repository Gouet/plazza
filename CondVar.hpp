//
// Created by gaspar_q on 4/14/16.
//

#ifndef CPP_PLAZZA_CONDVAR_HPP
#define CPP_PLAZZA_CONDVAR_HPP

#include <pthread.h>
#include "Mutex.hpp"

class CondVar : public ILockable
{
public:
    CondVar(pthread_condattr_t *attr = NULL);
    CondVar(CondVar const &);
    ~CondVar();
    CondVar &operator=(CondVar const &);

public:
    void    Wait();
    void    WakeUp();
    int     Lock() const;
    int     Unlock() const;
    bool    IsWaiting() const;

private:
    void setWait(bool waitstate);

private:
    bool            waiting;
    mutable Mutex   waitingMutex;
    Mutex           mut;
    pthread_cond_t  con;
};

#endif //CPP_PLAZZA_CONDVAR_HPP
