//
// main.cpp for main in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza/test
//
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
//
// Started on  Mon Apr 18 10:06:05 2016 Victor Gouet
// Last update Mon Apr 18 19:51:46 2016 Quentin Gasparotto
//

#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../NamedPipe.hpp"

int	main(int ac, char **av)
{

  // mkfifo("test", 0644);

  // std::cout << "Wait open" << std::endl;

  // int fd = open("test", O_WRONLY);

  // std::cout << "Opened" << std::endl;

  // sleep(5);

  // std::cout << "Start write" << std::endl;

  // write(fd, "toto", 4);

  // std::cout << "stop read" << std::endl;
  // if (ac != 2)
  //   {
  //     std::cerr << "av[1] missing" << std::endl;
  //     return (0);
  //   }
  NamedPipe	namedPipe("test", NamedPipe::OUT, true);

  signal(SIGPIPE, SIG_IGN);

  sleep(2);

  namedPipe << av[1];

  while(1);
  // while ((namedPipe << av[1] + std::to_string(i)) == false)
  //   {
  //     // while (!check && (check = (namedPipe << av[1] + std::to_string(i))) == false)
  //     // 	{
  //     // 	  ++i;
  //     // 	}
  //     // std::cout << "FAILED" << std::endl;
  //     ++i;
  //     // return (1);
  //   }
  // while ((namedPipe << av[1] + std::to_string(i)) == false)
  //   {
  //     ++i;
  //   }
  // while (1);
  return (0);
}
