//
// ProcessInfo.hpp for PROCESS in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 20 23:09:23 2016 Victor Gouet
// Last update Sat Apr 23 12:37:35 2016 Victor Gouet
//

#ifndef PROCESSINFO_HPP_
# define PROCESSINFO_HPP_

# include <sys/types.h>
# include <unistd.h>
# include <map>
# include <string>
# include <fstream>
# include <iostream>

class	ProcessInfo
{
public:
  ProcessInfo(pid_t pid, bool isThread = false);
  ~ProcessInfo();

public:
  bool		open();
  void		close();
  void		extractData();
  void	        clear();

public:
  std::map<std::string, std::string>	const &getData() const;
  pid_t						getPid() const;
  std::string const &				getStatus() const;
  bool						getThread() const;

private:
  pid_t						_pid;
  std::ifstream					output;
  mutable std::map<std::string, std::string>	_data;
  std::string				const	_nothing;
  bool						_isthread;
};

#endif
