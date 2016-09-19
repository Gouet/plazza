//
// Created by gaspar_q on 4/19/16.
//

#ifndef CPP_PLAZZA_ILOCKABLE_HPP
#define CPP_PLAZZA_ILOCKABLE_HPP

class ILockable
{
public:
    virtual ~ILockable(){}

public:
    virtual int Lock() const = 0;
    virtual int Unlock() const = 0;
};

#endif //CPP_PLAZZA_ILOCKABLE_HPP
