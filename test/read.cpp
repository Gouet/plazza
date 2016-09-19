//
// main.cpp for main in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza/test
//
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
//
// Started on  Mon Apr 18 10:06:05 2016 Victor Gouet
// Last update Mon Apr 18 19:39:30 2016 Quentin Gasparotto
//

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <termios.h>

#include "../NamedPipe.hpp"


int	main(int ac, char **av)
{
  // if (ac != 2)
  //   {
  //     std::cerr << "av[1] missing" << std::endl;
  //     return (0);
  //   }
  // mkfifo("test", 0644);

  // std::cout << "Wait open" << std::endl;

  // int fd = open("test", O_RDONLY);

  // std::cout << "Opened" << std::endl;

  // char	buff[4096];

  // sleep(5);

  // std::cout << "Start read" << std::endl;

  // int stp = read(fd, &buff, 4096);

  // buff[stp] = 0;

  // std::cout << "stop read: " << buff << std::endl;

  NamedPipe	namedPipe("test", NamedPipe::IN, true);

  std::string test;

  namedPipe.setTimeout(5);
  for (int i = 0; i < 3; i++)
    {
      std::cout << "bef" << std::endl;
      namedPipe >> test;
      std::cout << "aft" << std::endl;
      std::cout << test << std::endl;
    }

  // while (1)
  //   {
  //     std::string test;
  //     namedPipe >> test;
  //     if (!test.empty())
  // 	std::cout << test << std::endl;
  //     // else
  // 	// std::cout << "FAILED" << std::endl;
  //   }
  return (0);
}
