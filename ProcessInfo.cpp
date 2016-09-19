//
// ProcessInfo.cpp for PROCESS INFO in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 20 23:14:44 2016 Victor Gouet
// Last update Sat Apr 23 12:38:27 2016 Victor Gouet
//

#include "ProcessInfo.hpp"

ProcessInfo::ProcessInfo(pid_t pid, bool isThread) : _pid(pid), _nothing("NOTHING"),
						     _isthread(isThread)
{
}

ProcessInfo::~ProcessInfo()
{
}

bool		ProcessInfo::open()
{
  std::string	file("/proc/" + std::to_string(static_cast<int>(_pid)) + "/status");

  output.open(file.c_str());
  return (output.is_open());
}

void		ProcessInfo::close()
{
  output.close();
}

void		ProcessInfo::clear()
{
  _data.clear();
}

void		ProcessInfo::extractData()
{
  std::string   command;

  while (std::getline(output, command))
    {
      size_t pos = 0;
      std::string key;

      if ((pos = command.find(':')) != std::string::npos)
	{
	  key = command.substr(0, pos);
	  command.erase(0, pos + 1);
	  _data[key] = command;
	}
    }
}

std::map<std::string, std::string>	const &ProcessInfo::getData() const
{
  return (_data);
}

pid_t						ProcessInfo::getPid() const
{
  return (_pid);
}

std::string const &				ProcessInfo::getStatus() const
{
  if (_data.find("State") != _data.end())
    return _data["State"];
  return (_nothing);
}

bool						ProcessInfo::getThread() const
{
  return (_isthread);
}
