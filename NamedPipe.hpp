//
// Created by veyrie_f on 4/8/16.
//

#ifndef CPP_PLAZZA_NAMEDPIPE_HPP
#define CPP_PLAZZA_NAMEDPIPE_HPP

#include <string>
#include <fcntl.h>
#include <stdexcept>

/*
 * \brief Class encapsulation for named pipes handling
 */
class NamedPipe
{
public:
    enum IOMode
    {
        IN = O_RDONLY,
        OUT = O_WRONLY
    };

    class IOException : public std::runtime_error
    {
    public:
        IOException(std::string const &error) throw() :
                std::runtime_error("IO: " + error)
        {

        }

        virtual ~IOException() throw()
        {

        }
    };

public:
    NamedPipe(std::string const &name, NamedPipe::IOMode mode, bool nonblock = false, __mode_t right = 0666);

    ~NamedPipe();

    bool writePipe(std::string const &toWrite) const throw(IOException);

    std::string readPipe() const throw(IOException);

    NamedPipe &operator<<(std::string const &toWrite);

    NamedPipe &operator>>(std::string &string);

    bool isOpen(void) const;

    void setTimeout(size_t timeout);

    void unsetTimeout(void);

    int getFd() const;

    void unlinkPipe() const;

private:
    std::string m_name;
    IOMode m_mode;
    size_t m_timeout;
    int m_fd;
};

#endif //CPP_PLAZZA_NAMEDPIPE_HPP
