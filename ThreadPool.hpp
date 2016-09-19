//
// Created by gaspar_q on 4/12/16.
//

#ifndef CPP_PLAZZA_THREADPOOL_HPP
#define CPP_PLAZZA_THREADPOOL_HPP

#include <map>
#include <vector>
#include <limits.h>
#include "Thread.hpp"

template <typename threadParam>
class ThreadPool
{
private:
    typedef typename std::map<threadParam *, Thread<threadParam> *>::iterator threadIterator;
    typedef typename std::map<threadParam *, Thread<threadParam> *>::const_iterator threadConstIterator;

public:
    ThreadPool(size_t poolSize = 0) :
            runningThreads(0),
            poolSize(poolSize)
    {

    }
    ThreadPool(ThreadPool const &ref)
    {
        *this = ref;
    }
    ~ThreadPool()
    {
        for (threadIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            (it->second)->cancel();
            delete(it->second);
        }
        for (typename std::vector<threadParam *>::iterator it = threadElems.begin(), end = threadElems.end(); it != end; ++it)
        {
            delete(*it);
        }
    }
    ThreadPool  &operator=(ThreadPool const &ref)
    {
        runningThreads = ref.runningThreads;
        poolSize = ref.poolSize;
        pool = ref.pool;
        return (*this);
    }

public:
    bool        isSaturated() const
    {
        return runningThreads > 0;
    }
    void        launch(void *arg = NULL)
    {
        for (size_t i = 0; i < poolSize; ++i)
        {
            threadElems.push_back(new threadParam());
            pool[threadElems[i]] = new Thread<threadParam>(arg, *threadElems[i]);
	    // pool[threadElems[i]]->run();
        }
    }
    std::vector<threadParam *> const  &getThreadsElements() const
    {
        return threadElems;
    }
    void updateActivity()
    {
        runningThreads = 0;
        for (threadIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            runningThreads += (!it->second->isWaiting());
        }
    }
    size_t  getPoolSize() const
    {
        return (poolSize);
    }
    std::vector<Thread<threadParam> *>   getThreads() const
    {
        std::vector<Thread<threadParam> *>   vector;

        for (threadConstIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            vector.push_back(it->second);
        }
        return vector;
    }
    void cancelThreads()
    {
        for (threadIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            it->second->cancel();
        }
    }
    void joinThreads()
    {
        for (threadIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            it->second->join();
        }
    }
    bool isCorrupted() const
    {
        for (threadConstIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            if (it->second->isCorrupted())
                return true;
        }
        return (false);
    }
    void runThreads()
    {
        for (threadIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            it->second->run();
        }
    }

public:
    ThreadPool  &operator--()
    {
        if (runningThreads == 0)
            throw std::logic_error("ThreadPool: Thread Pool: Running threads can't be less than 0");
        --runningThreads;
        return *this;
    }
    ThreadPool  operator--(int)
    {
        ThreadPool cpy(*this);
        --(*this);
        return cpy;
    }
    ThreadPool  &operator++()
    {
        if (runningThreads == ULONG_MAX)
            throw std::logic_error("ThreadPool: ULONG_MAX overflowed");
        ++runningThreads;
        return *this;
    }
    ThreadPool  operator++(int)
    {
        ThreadPool cpy(*this);
        ++(*this);
        return cpy;
    }
    bool        operator>(ThreadPool const &pool) const
    {
        return runningThreads > pool.runningThreads;
    }
    bool        operator>=(ThreadPool const &pool) const
    {
        return runningThreads >= pool.runningThreads;
    }
    bool        operator<=(ThreadPool const &pool) const
    {
        return runningThreads <= pool.runningThreads;
    }
    bool        operator<(ThreadPool const &pool) const
    {
        return runningThreads < pool.runningThreads;
    }
    template <typename handlerType>
    ThreadPool<threadParam>     &operator<<(handlerType handle)
    {
        for (threadIterator it = pool.begin(), end = pool.end(); it != end; ++it)
        {
            (*it->second) += handle;
        }
        return *this;
    }
    Thread<threadParam> &operator[](threadParam &ref)
    {
        threadParam *idx = &ref;

        return *(pool[idx]);
    }

private:
    size_t                              runningThreads;
    size_t                              poolSize;
    std::vector<threadParam *>            threadElems;
    std::map<threadParam *, Thread<threadParam> *>  pool;
};

#endif //CPP_PLAZZA_THREADPOOL_HPP
