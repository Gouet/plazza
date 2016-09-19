//
// Created by gaspar_q on 4/20/16.
//

#ifndef CPP_PLAZZA_LOCKABLEDATA_HPP
#define CPP_PLAZZA_LOCKABLEDATA_HPP

#include "Mutex.hpp"

/**
 * \brief A class that allow any type of data to be thread safe
 *
 * \param DataType The type of the data you wont to be thread safe
 */
template <typename DataType>
class   LockableData : public Mutex
{
public:
    /**
     * \brief Constructor complient with DataType constructor
     *
     * \param args All args needed by the constructor of the data to lock
     */
    template <typename ... Types>
    LockableData(Types ... args) :
            dataToLock(args...)
    {
    }
    /**
     * \brief Copy constructor with the data type used to call cpy constructor of data to lock
     *
     * \param ref The reference of the cpy object
     */
    LockableData(DataType const &ref) :
            dataToLock(ref)
    {
    }
    /**
     * \brief Copy constructor with a LockableData. Call the cpy constructor of data to lock
     *
     * \param ref The reference of the LockableData
     */
    LockableData(LockableData const &ref) :
            dataToLock(*ref)
    {
    }
    /**
     * \brief The destructor that does nothing for the moment except call the mutex Destructor
     */
    ~LockableData()
    {
        this->~Mutex();
    }
    /**
     * \brief Operator equal that make a copy of the reference Thread Safe
     *
     * \param ref The reference to cpy
     *
     * \return A reference on this
     */
    LockableData    &operator=(LockableData const &ref)
    {
        Lock();
        dataToLock = *ref;
        Unlock();
        return *this;
    }

public:
    /**
     * \brief Getter thread safe that return a copy of the data
     *
     * \return A copy of the data
     */
    DataType        operator*() const
    {
        DataType    cpy;

        Lock();
        cpy = dataToLock;
        Unlock();
        return cpy;
    }
    /**
     * \brief Getter the return a reference of the data to lock Not Thread Safe
     *
     * \return A reference on the data
     */
    DataType    &operator*()
    {
        return dataToLock;
    }
    /**
     * \brief Call a method thread safely with all the arguments and return the value wanted
     *
     * \param method The method to call
     * \param args The arguments of the method
     *
     * \return The return of the method called
     */
    template <typename returnType, typename ... Types>
    returnType      call(returnType (DataType::*method)(Types...), Types ... args)
    {
        returnType  ret;

        Lock();
        ret = (dataToLock.*method)(args...);
        Unlock();
        return ret;
    }
    /**
     * \brief Call a method that return nothing with all the arguments
     *
     * \param method The method to call
     * \param args The arguments of the method
     */
    template <typename ... Types>
    void    call(void (DataType::*method)(Types...), Types ... args)
    {
        Lock();
        (dataToLock.*method)(args...);
        Unlock();
    }

private:
    DataType    dataToLock;
};

#endif //CPP_PLAZZA_LOCKABLEDATA_HPP
