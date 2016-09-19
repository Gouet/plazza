//
// Created by gaspar-q on 20/04/16.
//

#include <unistd.h>
#include <system_error>
#include "Pipe.hpp"

Pipe::Pipe()
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        throw std::system_error();
    pipeIn = pipefd[0];
    pipeOut = pipefd[1];
    if ((saveIn = dup(pipeIn)) == -1 ||
        (saveOut = dup(pipeOut)) == -1)
        throw std::system_error();
}

Pipe::Pipe(Pipe const &ref)
{
    *this = ref;
}

Pipe::~Pipe()
{
    closeIn();
    closeOut();
}

Pipe &Pipe::operator=(Pipe const &ref)
{
    closeIn();
    closeOut();
    if ((pipeIn = dup(ref.pipeIn)) == -1 ||
        (saveIn = dup(ref.saveIn)) == -1 ||
        (pipeOut = dup(ref.pipeOut)) == -1 ||
        (saveOut = dup(ref.saveOut)) == -1)
        throw std::system_error();
    return *this;
}

void Pipe::linkInTo(int fd)
{
    if (dup2(pipeIn, fd) == -1)
        throw std::system_error();
}

void Pipe::linkOutTo(int fd)
{
    if (dup2(pipeOut, fd) == -1)
        throw std::system_error();
}

int Pipe::getIn() const
{
    return pipeIn;
}

int Pipe::getOut() const
{
    return pipeOut;
}

void Pipe::resetIn()
{
    if (dup2(saveIn, pipeIn) == -1)
        throw std::system_error();
}

void Pipe::resetOut()
{
    if (dup2(saveOut, pipeOut) == -1)
        throw std::system_error();
}

void Pipe::reset()
{
    resetIn();
    resetOut();
}

void Pipe::closeIn()
{
  if (pipeIn != -1)
    close(pipeIn);
  if (saveIn != -1)
    close(saveIn);
    pipeIn = -1;
    saveIn = -1;
}

void Pipe::closeOut()
{
  if (pipeOut != -1)
    close(pipeOut);
  if (saveOut != -1)
    close(saveOut);
    pipeOut = -1;
    saveOut = -1;
}

std::string Pipe::readPipe() const
{
    ssize_t ret;
    char buff[BUFSIZ];
    std::string result;

    do
    {
        if ((ret = read(pipeIn, buff, BUFSIZ - 1)) < 0)
            throw std::system_error();
	buff[ret] = '\0';
        result += buff;
    } while (ret == BUFSIZ - 1);
    return result;
}

int Pipe::writePipe(std::string const &toWrite) const
{
    if (write(pipeOut, toWrite.c_str(), toWrite.length()) != static_cast<int>(toWrite.length()))
        return (-1);
    return 0;
}






























