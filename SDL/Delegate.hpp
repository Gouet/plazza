//
// Created by veyrie_f on 4/21/16.
//

#ifndef CPP_PLAZZA_CALLBACK_HPP
#define CPP_PLAZZA_CALLBACK_HPP

#include <functional>
#include <vector>

/*
 * \Brief Base class to use for template iterator and smart pointer
 */
class IDelegate
{
public:
    IDelegate()
    {}
    virtual ~IDelegate()
    {}

    virtual void operator()() const = 0;
};

/*
 * \Brief Delegate class that can call delegates.
 * Throws bad_function_call on error
 */
template <class T>
class Delegate : public IDelegate
{
public:
    Delegate(std::function<T> delegate) :
            m_delegate(delegate)
    {
    }

    virtual ~Delegate()
    {

    }

public:
    virtual void operator()() const
    {
        m_delegate();
    }

    void operator=(std::function<T> &delegate)
    {
        m_delegate = delegate;
    }

private:
    std::function<T> m_delegate;
};

#endif //CPP_PLAZZA_CALLBACK_HPP
