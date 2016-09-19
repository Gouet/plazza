//
// Created by gaspar_q on 4/8/16.
//

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <system_error>
#include "Process.hpp"
#include "Regex.hpp"
#include "Pipe.hpp"

/**
 * \brief Constructor of the process, just used for internal data initialisation
 */
Process::Process() :
        pid(-1), alive(false), fdIn(-1), fdOut(-1), fdErr(-1)
{

}

/**
 * \brief copy constructor that call default constructor + copy the pid
 *
 * \param process The process to copy
 */
Process::Process(const Process &process) :
        Process()
{
    *this = process;
}

/**
 * \brief operator equal for coplience
 *
 * \param process The process to copy
 *
 * \return A reference on this for linking
 */
Process &Process::operator=(const Process &process)
{
    pid = process.pid;
    return *this;
}

/**
 * \brief Destructor that kill the process if he's still alive
 */
Process::~Process()
{
    if (isAlive())
        killSon();
}

/**
 * \brief Check if the process is the child process
 *
 * \return A boolean, true if he is the chield false either
 */
bool Process::isChildProcess() const
{
    return pid == 0;
}

/**
 * \brief Getter for the pid
 *
 * \return The child pid if the process has been launch and if you'r in the father
 */
pid_t const &Process::getPid(void) const
{
    return pid;
}

/**
 * \brief Will just kill the son process if he's alive
 */
void Process::killSon() const
{
    if (pid != -1 && !isChildProcess())
        kill(pid, SIGKILL);
}

/**
 * \brief Check if the son process is still alive, update zombies state thanks to a waitpid
 *
 * \return a boolean, true if is alive, false either
 */
bool Process::isAlive() const
{
    if (!alive)
        return false;
    if (!isChildProcess() && getPid() != -1)
    {
        int stat;
        waitpid(getPid(), &stat, WNOHANG);
        alive = (kill(getPid(), 0) == 0);
    }
    return alive;
}

/**
 * \brief Set the standard input of the child process
 */
void Process::setStdIn(int fd)
{
    fdIn = fd;
}

/**
 * \brief Set the standard output of the child process
 */
void Process::setStdOut(int fd)
{
    fdOut = fd;
}

/**
 * \brief Set the error ouput of the child process
 */
void Process::setStdErr(int fd)
{
    fdErr = fd;
}

/**
 * \brief Get the standard input fd
 *
 * \return Standard input fd
 */
int Process::getStdIn() const
{
    return fdIn;
}

/**
 * \brief Get the standard output fd
 *
 * \return Standard output fd
 */
int Process::getStdOut() const
{
    return fdOut;
}

/**
 * \brief Get the error output fd
 *
 * \return Error output fd
 */
int Process::getStdErr() const
{
    return fdErr;
}

/**
 * \brief Will wait until the son exit properly
 *
 * \return The status given by the waitpid function
 */
int Process::waitSon() const
{
    int status = 0;

    if (!isChildProcess() && isAlive())
        waitpid(getPid(), &status, 0);
    return status;
}

/**
 * \brief Static function for getting the output of a command in a string
 *
 * \param command The command you want to get the output
 *
 * \return The output of the command
 */
std::string         Process::getOutput(const std::string &command)
{
    Pipe        pipe;
    pid_t       pid;
    std::string output;
    int         status;

    if ((pid = fork()) == -1)
        throw std::system_error();
    if (pid == 0)
    {
        pipe.linkOutTo(1);
        pipe.closeIn();
        exit(std::system(command.c_str()));
    }
    pipe.closeOut();
    output = pipe.readPipe();
    wait(&status);
    return output;
}
