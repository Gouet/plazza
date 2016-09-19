//
// testxor.cpp for test xor in /home/gaspar_q/rendu/semestre4/C++/cpp_plazza/test
//
// Made by Quentin Gasparotto
// Login   <gaspar_q@epitech.net>
//
// Started on  Thu Apr 21 10:29:16 2016 Quentin Gasparotto
// Last update Thu Apr 21 10:38:38 2016 Quentin Gasparotto
//

#include <fstream>
#include <sstream>
#include "../FileDecryptor.hpp"
#include "../XORDecrypt.hpp"

int	main(int ac, char **av)
{
  XORDecrypt	decr;
  Regex		reg("");//[a-zA-Z0-9\\_\\.\\-]+\\@[a-zA-Z0-9\\_\\.\\-]+
  FileDecryptor	fdec;

  if (ac != 2)
    return (1);
  fdec.setFilename(av[1]);
  fdec.tryDecryption(reg);
  std::cout << fdec.getUncryptedFile() << std::endl;
  return (0);
}
