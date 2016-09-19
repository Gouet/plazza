//
// Created by gaspar_q on 4/19/16.
//

#ifndef CPP_PLAZZA_SEMAPHORE_HPP
#define CPP_PLAZZA_SEMAPHORE_HPP

#include <string>
#include "ILockable.hpp"

class Semaphore : public ILockable
{
public:
    Semaphore(std::string const &path = ".");
    Semaphore(Semaphore const &);
    Semaphore   &operator=(Semaphore const &);
    ~Semaphore();

public:
    int Lock() const;
    int Unlock() const;
    void setPath(std::string const &);
    static void cleanSemaphora(std::string const &path = ".");

private:
    void acquireSemaphora();

private:
    bool isCreator;
    int semid;
    int key;
};

#endif //CPP_PLAZZA_SEMAPHORE_HPP
