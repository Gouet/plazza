//
// Order.cpp for ORDER in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 13 09:48:02 2016 Victor Gouet
// Last update Thu Apr 21 00:05:17 2016 Victor Gouet
//

#include "Order.hpp"

Order::Order()
{
  _info = DEFAULT;
}

Order::~Order()
{
}

Order::Order(Order const &other)
{
  *this = other;
}

Order				&Order::operator=(Order const &other)
{
  this->_fileContainers = other._fileContainers;
  this->_info = other._info;
  return (*this);
}

void		Order::evalutation(std::string &eval)
{
  size_t	pos = 0;
  std::string   command;
  std::string	information[3] = {"PHONE_NUMBER", "EMAIL_ADDRESS", "IP_ADDRESS"};
  Information	info[3] = {PHONE_NUMBER, EMAIL_ADDRESS, IP_ADDRESS};

  while ((pos = eval.find(' ')) != std::string::npos || (pos = eval.find('\t')) != std::string::npos  || !eval.empty())
    {
      if (pos == std::string::npos)
	pos = eval.size();
      command = eval.substr(0, pos);
      eval.erase(0, pos + 1);
      if (!command.empty())
	{
	  size_t	i = 0;
	  while (i < 3)
	    {
	      if (command == information[i])
		{
		  if (_info != DEFAULT)
		    {
		      throw std::runtime_error("two times INFORMATION");
		    }
		  else
		    {
		      _info = info[i];
		      break;
		    }
		}
	      ++i;
	    }
	  if (i == 3)
	    {
	      addFile(command);
	    }
	}
    }
  if (_info == DEFAULT || _fileContainers.size() == 0)
    {
      throw std::runtime_error("something is missing");
    }
}

void		Order::setInformation(Information info)
{
  _info = info;
}

void		Order::addFile(std::string const &file)
{
  _fileContainers.push_back(file);
}

std::list<std::string> const	&Order::getFileContainers() const
{
  return (_fileContainers);
}

Order::Information		Order::getInfo() const
{
  return (_info);
}

std::ostream			&operator<<(std::ostream &ostream, Order const &order)
{
  std::list<std::string>	fileContainers = order.getFileContainers();
  std::string			info[4] = {"DEFAULT", "PHONE_NUMBER", "EMAIL_ADDRESS", "IP_ADDRESS" };

  ostream << "File containers : {" << std::endl;
  for (std::list<std::string>::const_iterator	it = fileContainers.begin(),
	 end = fileContainers.end(); it != end ; ++it)
    {
      if (std::next(it, 1) == end)
	{
	  ostream << *it << std::endl;
	}
      else
	ostream << *it << "," << std::endl;
    }
  ostream << "}" << std::endl;
  ostream << "Information :" << std::endl;
  ostream << info[order.getInfo()] << std::endl;
  return (ostream);
}

void Order::clear()
{
    _info = Order::DEFAULT;
    _fileContainers.clear();
}
