//
// ExploreFile.cpp for EXPLORE FILE in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Mon Apr 18 16:03:36 2016 Victor Gouet
// Last update Tue Apr 19 09:25:24 2016 Victor Gouet
//

#include "ExploreDirectory.hpp"

ExploreDirectory::ExploreDirectory(std::string const &directoryName) :
  pathDirectory(directoryName), directory(directoryName)
{  
}

ExploreDirectory::~ExploreDirectory()
{

}

void		ExploreDirectory::openDirectory()
{
  DIR		*dir;
  struct dirent	*entry;

  if ((dir = opendir(pathDirectory.c_str())) != NULL)
    {
      files.clear();
      while ((entry = readdir(dir)) != NULL)
	{
	  files.push_back(*entry);
	}
      closedir(dir);
    }
}

bool	        ExploreDirectory::operator+=(std::string const &directoryName)
{
  std::string	newDirectory = pathDirectory + directoryName + "/";
  DIR		*dir;

  if ((dir = opendir(newDirectory.c_str())) != NULL)
    {
      pathDirectory += directoryName;
      pathDirectory += "/";
      directory = directoryName;
      closedir(dir);
      return (true);
    }
  return (false);
}

std::string const		&ExploreDirectory::getPathDirectoryName() const
{
  return (pathDirectory);
}

std::list<struct dirent> const &ExploreDirectory::getFiles() const
{
  return (files);
}
