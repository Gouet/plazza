//
// Created by veyrie_f on 4/8/16.
//

#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <strings.h>
#include <sys/poll.h>
#include <stdexcept>
#include <termios.h>
#include <string.h>
#include "NamedPipe.hpp"

NamedPipe::NamedPipe(std::string const &name, NamedPipe::IOMode mode, bool blocking, __mode_t rights) : m_name(name), m_mode(mode), m_timeout(0)
{
    if (access(name.c_str(), F_OK) != 0)
        mkfifo(name.c_str(), rights);
    m_fd = open(m_name.c_str(), mode | (blocking ? O_NONBLOCK : 0));
}

NamedPipe::~NamedPipe()
{
    close(m_fd);
    unlink(m_name.c_str());
}

/*
 * \brief writes to a fifo file.
 * \return false if open failed, true otherwise
 */
bool NamedPipe::writePipe(std::string const &toWrite) const throw(IOException)
{
    if (m_mode == IN)
        throw NamedPipe::IOException("You can't write on input named pipe");
    if (!isOpen())
        throw NamedPipe::IOException("File open fail");
    return write(m_fd, toWrite.c_str(), toWrite.size()) != -1;
}

std::string NamedPipe::readPipe() const throw(IOException)
{
    if (m_mode == OUT)
        throw NamedPipe::IOException("You can't read on output named pipe");
    if (!isOpen())
        throw NamedPipe::IOException("File open fail");
    char buf[BUFSIZ];
    std::string ret;
    ssize_t readRet;

    if (m_timeout != 0)
    {
        fd_set  fds;
        struct timeval timeout;
        size_t  timeGot = m_timeout;

        do
        {
            FD_ZERO(&fds);
            FD_SET(m_fd, &fds);
            timeout.tv_sec = (timeGot > 5 ? 5 : timeGot);
            timeGot -= timeout.tv_sec;
            timeout.tv_usec = 0;
            if (select(m_fd + 1, &fds, 0, 0, &timeout) != 0)
            {
                timeGot = 5;
                break;
            }
        } while (timeGot > 0);
        if (timeGot == 0)
            return "";
    }
    do
    {
        if ((readRet = read(m_fd, buf, BUFSIZ - 1)) <= 0)
            break;
        buf[readRet] = '\0';
        ret += buf;
    } while (readRet == BUFSIZ - 1);
    return ret;
}

NamedPipe &NamedPipe::operator<<(std::string const &toWrite)
{
    writePipe(toWrite);
    return *this;
}

NamedPipe &NamedPipe::operator>>(std::string &string)
{
    string = readPipe();
    return *this;
}

bool NamedPipe::isOpen(void) const
{
    return m_fd >= 0;
}

void NamedPipe::unsetTimeout(void)
{
    int flag = fcntl(m_fd, F_GETFL, 0);
    fcntl(m_fd, F_SETFL, flag & ~O_NONBLOCK);
    m_timeout = 0;
}

void NamedPipe::setTimeout(size_t timeout)
{
    int flag = fcntl(m_fd, F_GETFL, 0);
    fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);
    m_timeout = timeout;
}

int NamedPipe::getFd() const
{
    return m_fd;
}

void NamedPipe::unlinkPipe() const
{
    unlink(m_name.c_str());
}
