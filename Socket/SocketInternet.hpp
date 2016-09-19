//
// Created by veyrie_f on 4/12/16.
//

#ifndef CPP_PLAZZA_INTERNETSOCKET_HPP
#define CPP_PLAZZA_INTERNETSOCKET_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <system_error>
#include "ISocket.hpp"

namespace SocketInternet
{
    /*
     * \brief Defines the packet size used by the server and the client
     */
    size_t const PACKET_SIZE = 100;

    /*
     * \brief The internet server class. Supposed to be waiting for client.
     */
    class Server : public ISocketServer
    {
    public:
        // Default tcp/ip port
        Server(uint16_t port = 51717);
        virtual ~Server();

    public:
        void Start();
        std::string const& Listen();

    public:
        Server &operator>>(std::string & str);

    private:
        int m_socketID;
        uint16_t m_port;
        socklen_t m_sockLen;
        struct sockaddr_in m_sockaddrLocal;
        struct sockaddr_in m_sockaddrRemote;
        std::string m_listened;
    };

    /*
     * \brief Unix client class. Connects to a server and sends messages.
     */
    class Client : public ISocketClient
    {
    public:
        Client(std::string const& hostName = "localhost", uint16_t port = 51717);
        virtual ~Client();

    public:
        void Start();
        void Talk(std::string const &toSay) const;

    public:
        void operator<<(std::string const& toSay) const;

    private:
        std::string m_hostName;
        uint16_t m_port;
        int m_socketID;
        struct sockaddr_in m_sockaddrRemote;
    };

    class SocketInternetException : public std::runtime_error
    {
    public:
        SocketInternetException(std::string const& log) : std::runtime_error(log)
        { }
    };
}


#endif //CPP_PLAZZA_INTERNETSOCKET_HPP
