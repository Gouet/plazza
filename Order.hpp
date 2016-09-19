//
// Order.hpp for Order in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 13 09:42:48 2016 Victor Gouet
// Last update Wed Apr 13 12:58:32 2016 Victor Gouet
//

#ifndef ORDER_HPP_
# define ORDER_HPP_

# include <list>
# include <iostream>
# include <stdexcept>
# include <iterator>

class Order
{
public:
    enum Information
    {
        DEFAULT,
        PHONE_NUMBER,
        EMAIL_ADDRESS,
        IP_ADDRESS
    };

public:
    Order();

    ~Order();

    Order(Order const &);

public:
    Order &operator=(Order const &);

public:
    void addFile(std::string const &);

    void setInformation(Information info);

    void evalutation(std::string &);

    void clear();

public:
    std::list<std::string> const &getFileContainers() const;

    Information getInfo() const;

private:
    std::list<std::string> _fileContainers;
    Information _info;

};

std::ostream &operator<<(std::ostream &ostream, Order const &order);

#endif
