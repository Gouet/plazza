//
// Created by veyrie_f on 4/12/16.
//

#include "SocketUnix.hpp"

SocketUnix::Server::Server(const std::string &socketName) :
        m_socketName(socketName)
{
}

/*
 * \brief Dtor : deletes the socket file
 */
SocketUnix::Server::~Server()
{
    unlink(m_sockaddrLocal.sun_path);
    //remove(m_socketName.c_str());
}

void SocketUnix::Server::Start()
{
    int len;

    if ((m_socketID = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        throw SocketUnixException("Socket");
    }
    m_sockaddrLocal.sun_family = AF_UNIX;
    if ((strcpy(m_sockaddrLocal.sun_path, m_socketName.c_str())) == NULL)
    {
        throw SocketUnixException("Socket alloc");
    }
    len = strlen(m_sockaddrLocal.sun_path) + sizeof(m_sockaddrLocal.sun_family);
    if (bind(m_socketID, (struct sockaddr *) &m_sockaddrLocal, len) == -1)
    {
        throw SocketUnixException("Socket bind");
    }

    if (listen(m_socketID, 1) == -1)
    {
        throw SocketUnixException("Socket listen");
    }
}

/*
 * \brief Can ONLY accept one client !
 */
std::string const &SocketUnix::Server::Listen()
{
    unsigned t;
    int s2;
    char str[PACKET_SIZE];
    bool done;
    ssize_t n;

    m_listened.clear();
    t = sizeof(m_sockaddrRemote);
    if ((s2 = accept(m_socketID, (struct sockaddr *) &m_sockaddrRemote, &t)) == -1)
    {
        throw SocketUnixException("Socket accept");
    }

    done = false;
    while (!done)
    {
        bzero(str, PACKET_SIZE);
        n = recv(s2, str, PACKET_SIZE, 0);
        for (int j = 0; j < n; j++)
        {
            m_listened += str[j];
        }

        if (n <= 0)
        {
            if (n < 0)
            {
                throw SocketUnixException("Socket recv");
            }
            done = true;
        }
    };
    close(s2);
    return m_listened;
}

SocketUnix::Client::Client(const std::string &socketName) :
        m_socketName(socketName)
{

}

/*
 * \brief DTOR: notifies the remote server client is leaving
 */
SocketUnix::Client::~Client()
{
    shutdown(m_socketID, SHUT_RDWR);

    close(m_socketID);
}

void SocketUnix::Client::Start()
{
    int len;

    if ((m_socketID = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        throw SocketUnixException("Socket connect");
    }
    m_sockaddrRemote.sun_family = AF_UNIX;
    strcpy(m_sockaddrRemote.sun_path, m_socketName.c_str());
    len = strlen(m_sockaddrRemote.sun_path) + sizeof(m_sockaddrRemote.sun_family);
    if (connect(m_socketID, (struct sockaddr *) &m_sockaddrRemote, len) == -1)
    {
        throw SocketUnixException("Socket connect");
    }
}

void SocketUnix::Client::Talk(std::string const &toSay) const
{
    std::string buf;
    int i = 0;

    while (!toSay.empty() && toSay[i])
    {
        buf += toSay[i];
        ++i;
        if ((i != 0 && i % PACKET_SIZE == 0) || !toSay[i])
        {
            if (send(m_socketID, buf.c_str(), buf.size(), 0) == -1)
            {
                throw SocketUnixException("Socket send");
            }
            buf.clear();
        }
    }
}

SocketUnix::Server &SocketUnix::Server::operator>>(std::string & str)
{
    str = Listen();
    return *this;
}

void SocketUnix::Client::operator<<(std::string const &toSay) const
{
    Talk(toSay);
}
