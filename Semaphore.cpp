//
// Created by gaspar_q on 4/19/16.
//

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <system_error>
#include <iostream>
#include "Semaphore.hpp"

Semaphore::Semaphore(std::string const &path) :
    isCreator(false),
    semid(-1),
    key(-1)
{
    setPath(path);
    acquireSemaphora();
}

Semaphore::Semaphore(const Semaphore &semaphore)
{
    *this = semaphore;
}

Semaphore &Semaphore::operator=(const Semaphore &semaphore)
{
    key = semaphore.key;
    semid = semaphore.semid;
    isCreator = false;
    return *this;
}

Semaphore::~Semaphore()
{
    if (isCreator)
        semctl(semid, 0, IPC_RMID, 0);
}

int Semaphore::Lock() const
{
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_flg = 0;
    sops.sem_op = -1;
    return (semop(semid, &sops, 1));
}

int Semaphore::Unlock() const
{
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_flg = 0;
    sops.sem_op = 1;
    return (semop(semid, &sops, 1));
}

void Semaphore::setPath(const std::string &string)
{
    if ((key = ftok(string.c_str(), 0)) == -1)
        throw std::system_error();
    if (isCreator && semid != -1)
        semctl(semid, 0, IPC_RMID, 0);
    acquireSemaphora();
}

void Semaphore::acquireSemaphora()
{
    if ((semid = semget(key, 1, SHM_R | SHM_W)) == -1)
    {
        if ((semid = semget(key, 1, IPC_CREAT | SHM_R | SHM_W)) == -1)
            throw std::system_error();
        if (semctl(semid, 0, SETVAL, 1) == -1)
            throw std::system_error();
        isCreator = true;
    }
}

void Semaphore::cleanSemaphora(const std::string &path)
{
    Semaphore   *semaphore;

    semaphore = new Semaphore(path);
    if (!semaphore->isCreator)
        semctl(semaphore->semid, 0, IPC_RMID, 0);
    delete(semaphore);
}
