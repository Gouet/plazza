//
// Created by gaspar_q on 4/14/16.
//

#include "CondVar.hpp"

CondVar::CondVar(pthread_condattr_t *attr) :
    waiting(false)
{
    pthread_cond_init(&con, attr);
}

CondVar::CondVar(const CondVar &var)
{
    *this = var;
}

CondVar::~CondVar()
{

}

CondVar &CondVar::operator=(const CondVar &var)
{
    con = var.con;
    mut = var.mut;
    return *this;
}

void CondVar::WakeUp()
{
    if (IsWaiting())
        pthread_cond_signal(&con);
}

void CondVar::Wait()
{
    setWait(true);
    pthread_cond_wait(&con, &*mut);
    setWait(false);
}

int CondVar::Lock() const
{
    return mut.Lock();
}

int CondVar::Unlock() const
{
    return mut.Unlock();
}

void CondVar::setWait(bool waitstate)
{
    waitingMutex.Lock();
    waiting = waitstate;
    waitingMutex.Unlock();
}

bool CondVar::IsWaiting() const
{
    bool cpy;

    waitingMutex.Lock();
    cpy = waiting;
    waitingMutex.Unlock();
    return cpy;
}
