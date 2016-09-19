//
// thread.hpp for THREAD in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Thu Apr  7 13:56:21 2016 Victor Gouet
// Last update Sat Apr 23 18:31:51 2016 Victor Gouet
//

#ifndef THREAD_HPP_
# define THREAD_HPP_

# include <signal.h>
# include <pthread.h>
# include <stdexcept>
# include <iostream>
# include <list>
# include <unistd.h>
# include <string.h>
# include "ActionHandler.hpp"
# include "CondVar.hpp"

namespace thread
{
    enum State
    {
        Continue,
        Stop,
        Last,
        Restart
    };
};

template<class T>
class Thread
{
public:
    Thread(void *args, T &object) : waiting(true), _args(args), handler(object)/* _object(object)*/
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_mutex_init(&_isWaitingMutex, NULL);
        pthread_mutex_lock(&_mutex);

        stillAlive = true;

        if (pthread_create(&this->_restrict, NULL, onRunThread, this) == -1)
        {
            throw std::runtime_error("pthread_create error");
        }
        // usleep(0);
    }

    Thread(Thread &ref) :
            Thread(ref._args, *ref.handler)
//            handler(ref.handler), threadHandler(*this)
    {
//        this->Thread(ref._args, *ref.handler);
        *this = ref;
    }

    ~Thread()
    {
        if (isStillAlive())
            cancel();
    }

    Thread<T> &operator=(Thread<T> const &thread1)
    {
        waiting = thread1.waiting;
        _args = thread1._args;
        handler = thread1.handler;
        stillAlive = thread1.stillAlive;
        return *this;
    }

public:

    void run()
    {
        if (stillAlive == false)
            return;

        while (1)
        {
            pthread_mutex_lock(&_isWaitingMutex);

            if (waiting)
            {
                pthread_mutex_unlock(&_isWaitingMutex);
                break ;
            }
            pthread_mutex_unlock(&_isWaitingMutex);
            usleep(0);
        }
        pthread_mutex_lock(&_isWaitingMutex);
        waiting = false;
        pthread_mutex_unlock(&_isWaitingMutex);

        pthread_mutex_unlock(&_mutex);
    }

    void join()
    {
        pthread_join(this->_restrict, NULL);
    }

    bool isStillAlive() const
    {
        return (stillAlive);
    }

    bool isWaiting() const
    {
        return waiting;
    }

    void cancel()
    {
      pthread_mutex_trylock(&_mutex);
      pthread_mutex_unlock(&_mutex);
      pthread_cancel(_restrict);
      stillAlive = false;
      join();
    }

    bool isCorrupted() const
    {
        return (isStillAlive() && pthread_kill(_restrict, 0) != 0);
    }

    /*
     *  Warning !
     *  Not thread safe, you have to use this METHOD when the thread is waiting
     */
    void setArgs(void *newArgs)
    {
        _args = newArgs;
    }

public:
    template<class U>
    void            operator+=(U method)
    {
        // condVar.Lock();
        handler << method;
        // condVar.Unlock();
//        _callBack.push_back(method);
    }

private:
    static void *onRunThread(void *ptr)
    {
        Thread<T> *newThis;
        thread::State state;
        pthread_mutex_t *running;

        // newThis->condVar.Lock();
        // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        newThis = static_cast<Thread *>(ptr);
        state = thread::Restart;

        running = &newThis->_mutex;
        // run = &newThis->condVar;
        // newThis->condVar.Unlock();

        while (state == thread::Restart)
        {
            // newThis->condVar.Lock();
            pthread_mutex_lock(&newThis->_isWaitingMutex);

            newThis->waiting = true;

            pthread_mutex_unlock(&newThis->_isWaitingMutex);

            pthread_mutex_lock(running);

            for (size_t j = 0, len = newThis->handler.getHandleNumber(); j < len; ++j)
            {
                state = newThis->handler[j](newThis->_args);
                if (state == thread::Stop || state == thread::Restart)
                {
                    break;
                }
                else if (state == thread::Last)
                {
                    j = len - 2;
                }
            }
        }
        newThis->stillAlive = false;
        pthread_mutex_unlock(running);
        return (NULL);
    }

private:
    bool waiting;
    pthread_t _restrict;
    void *_args;
    ActionHandler<T, thread::State> handler;
    pthread_mutex_t _mutex;
    mutable pthread_mutex_t _isWaitingMutex;
    bool stillAlive;
    // CondVar				condVar;
};

#endif
