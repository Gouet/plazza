//
// Created by veyrie_f on 4/12/16.
//

#include <stdio.h>
#include <netdb.h>
#include "SocketInternet.hpp"

SocketInternet::Server::Server(uint16_t port) :
        m_port(htons(port))
{

}

SocketInternet::Server::~Server()
{
    close(m_socketID);
}

void SocketInternet::Server::Start()
{
    if ((m_socketID = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw SocketInternetException("Socket");
    }
    bzero((char*)&m_sockaddrLocal, sizeof(m_sockaddrLocal));
    m_sockaddrLocal.sin_family = AF_INET;
    m_sockaddrLocal.sin_addr.s_addr = INADDR_ANY;
    m_sockaddrLocal.sin_port = m_port;
    if (bind(m_socketID, (struct sockaddr *) &m_sockaddrLocal, sizeof(m_sockaddrLocal)) == -1)
    {
        throw SocketInternetException("Socket bind");
    }

    if (listen(m_socketID, 1) == -1)
    {
        throw SocketInternetException("Socket listen");
    }
}

std::string const &SocketInternet::Server::Listen()
{
    int sockfd;
    char buff[PACKET_SIZE] = {0};
    ssize_t n;

    m_listened.clear();

    m_sockLen = sizeof(m_sockaddrRemote);
    if ((sockfd = accept(m_socketID, (struct sockaddr *) &m_sockaddrRemote, &m_sockLen)) < 0)
    {
        throw SocketInternetException("Socket accept");
    }

    while ((n = read(sockfd, buff, PACKET_SIZE - 1)) > 0)
    {
        if (n < 0)
        {
            throw SocketInternetException("Socket read");
        }
        m_listened += buff;
        bzero(buff, PACKET_SIZE);
    }
    return m_listened;
}

SocketInternet::Server &SocketInternet::Server::operator>>(std::string &str)
{
    str = Listen();
    return *this;
}

SocketInternet::Client::Client(const std::string &hostName, uint16_t port) :
        m_hostName(hostName),
        m_port(htons(port))
{

}

SocketInternet::Client::~Client()
{
    shutdown(m_socketID, SHUT_RDWR);
    close(m_socketID);
}

void SocketInternet::Client::Start()
{
    struct hostent *server;

    if ((m_socketID = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw SocketInternetException("Socket");
    }
    server = gethostbyname(m_hostName.c_str());
    // if no server is found
    if (server == NULL)
    {
        throw SocketInternetException("Socket server not found");
    }
    bzero((char*)&m_sockaddrRemote, sizeof(m_sockaddrRemote));
    m_sockaddrRemote.sin_family = AF_INET;
    bcopy(server->h_addr, (char*)&m_sockaddrRemote.sin_addr.s_addr, server->h_length);

    m_sockaddrRemote.sin_port = m_port;

    if (connect(m_socketID, (struct sockaddr *) &m_sockaddrRemote, sizeof(m_sockaddrRemote)) == -1)
    {
        throw SocketInternetException("Socket connect");
    }
}

void SocketInternet::Client::Talk(std::string const &toSay) const
{
    ssize_t n;

    n = write(m_socketID, toSay.c_str(), toSay.size());

    if (n < 0)
    {
        throw SocketInternetException("Socket write");
    }
}

void SocketInternet::Client::operator<<(std::string const &toSay) const
{
    Talk(toSay);
}
