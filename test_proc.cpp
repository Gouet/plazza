//
// test_proc.cpp for test process in /home/gaspar_q/rendu/semestre4/C++/cpp_plazza
//
// Made by Quentin Gasparotto
// Login   <gaspar_q@epitech.net>
//
// Started on  Fri Apr 22 23:00:12 2016 Quentin Gasparotto
// Last update Fri Apr 22 23:05:12 2016 Quentin Gasparotto
//

#include <thread>
#include "Process.hpp"

int main()
{
    std::string output;
    //  std::thread

    output = Process::getOutput("cat Process.hpp");
    std::cout << output << std::endl;
    return (0);
}
