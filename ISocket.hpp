//
// Created by veyrie_f on 4/12/16.
//

#ifndef CPP_PLAZZA_ISOCKET_HPP
#define CPP_PLAZZA_ISOCKET_HPP

#include <string>

class ISocketServer
{
public:
    ISocketServer()
    {}
    virtual ~ISocketServer()
    {}

public:
    virtual void Start() = 0;
    virtual std::string const& Listen() = 0;
};

class ISocketClient
{
public:
    ISocketClient()
    {}
    virtual ~ISocketClient()
    {}

public:
    virtual void Start() = 0;
    virtual void Talk(std::string const &toSay) const = 0;
};

#endif //CPP_PLAZZA_ISOCKET_HPP
