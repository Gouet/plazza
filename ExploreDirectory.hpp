//
// ExploreFile.hpp for EXPLORE FILE in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Mon Apr 18 16:03:14 2016 Victor Gouet
// Last update Tue Apr 19 09:30:58 2016 Victor Gouet
//

#ifndef EXPLOREFILE_HPP_
# define EXPLOREFILE_HPP_

# include <stdio.h>
# include <cstdlib>
# include <iostream>
# include <string.h>
# include <fstream>
# include <dirent.h>
# include <list>
# include <fcntl.h>
# include <unistd.h>

class   ExploreDirectory
{
public:
  ExploreDirectory(std::string const &directoryName = "./");
  ~ExploreDirectory();

public:
  void		openDirectory();

public:
  bool	        operator+=(std::string const &);

public:
  std::list<struct dirent> const	&getFiles() const;
  std::string const			&getPathDirectoryName() const;

private:
  std::list<struct dirent>	files;
  std::string			pathDirectory;
  std::string			directory;
};

#endif
