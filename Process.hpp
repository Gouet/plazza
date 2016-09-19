//
// Created by gaspar_q on 4/8/16.
//

#ifndef CPP_PLAZZA_PROCESS_HPP
#define CPP_PLAZZA_PROCESS_HPP

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdexcept>

#include "ActionHandler.hpp"
#include "Thread.hpp"
#include "FileDecryptor.hpp"
#include "NamedPipe.hpp"

/**
 * \brief Process encapsulation for an easier use of them
 */
class Process
{
public:
    /**
     * \brief Process error is a Nested class of exception
     */
    class ProcessError : public std::runtime_error
    {
    public:
        ProcessError(std::string const &err) :
                std::runtime_error("Process: " + err)
        {

        }

        ~ProcessError() throw()
        {

        }
    };

public:
    Process();

    Process(Process const &);

    Process &operator=(Process const &);

                       ~Process();

public:
    /**
     * \brief Will launch the son process, can take a call back with the args you want to send to it
     *
     * \param callBack The callback you want to call for the son
     * \param args The arguments you will send to the son
     */
    template<typename callBackType, typename ... Types>
    void launch(callBackType callBack, Types ... args) throw(ProcessError)
    {
        pid = fork();
        if (pid == -1)
            throw ProcessError(strerror(errno));
        if (isChildProcess())
        {
            if (fdIn != -1)
                dup2(fdIn, 0);
            if (fdOut != -1)
                dup2(fdOut, 1);
            if (fdErr != -1)
                dup2(fdErr, 2);
            callBack(args...);
            exit(0);
        }
        alive = true;
    }

    bool               isChildProcess() const;

    void               killSon() const;

    bool               isAlive() const;

    int                waitSon() const;

    static std::string getOutput(std::string const &);

public:
    pid_t const &getPid(void) const;

    void        setStdIn(int fd);

    void        setStdOut(int fd);

    void        setStdErr(int fd);

    int         getStdIn() const;

    int         getStdOut() const;

    int         getStdErr() const;

private:
    pid_t        pid;
    mutable bool alive;
    int          fdIn;
    int          fdOut;
    int          fdErr;
};

#endif //CPP_PLAZZA_PROCESS_HPP
