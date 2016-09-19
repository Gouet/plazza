//
// Created by veyrie_f on 4/12/16.
//

#ifndef CPP_PLAZZA_SOCKETUNIX_HPP
#define CPP_PLAZZA_SOCKETUNIX_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <system_error>
#include "ISocket.hpp"

namespace SocketUnix
{
    /*
     * \brief Defines the packet size used by the server and the client
     */
    size_t const PACKET_SIZE = 100;

    /*
     * \brief The unix server class. Supposed to be waiting for client.
     */
    class Server : public ISocketServer
    {
    public:
        Server(std::string const& socketName = "unix_socket");
        virtual ~Server();

    public:
        void Start();
        std::string const& Listen();

    public:
        Server &operator>>(std::string & str);

    private:
        std::string m_socketName;
        int m_socketID;
        struct sockaddr_un m_sockaddrLocal;
        struct sockaddr_un m_sockaddrRemote;
        std::string m_listened;
    };

    /*
     * \brief Unix client class. Connects to a server and sends messages.
     */
    class Client : public ISocketClient
    {
    public:
        Client(std::string const& socketName = "unix_socket");
        virtual ~Client();

    public:
        void Start();
        void Talk(std::string const &toSay) const;

    public:
        void operator<<(std::string const &toSay) const;

    private:
        std::string m_socketName;
        int m_socketID;
        struct sockaddr_un m_sockaddrRemote;
    };

    class SocketUnixException : public std::runtime_error
    {
    public:
        SocketUnixException(std::string const& log) : std::runtime_error(log)
        { }
    };
}


#endif //CPP_PLAZZA_SOCKETUNIX_HPP
