//
// main.cpp for test in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
//
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
//
// Started on  Thu Apr  7 14:14:49 2016 Victor Gouet
// Last update Sun Apr 24 13:06:58 2016 veyrie_f
//

#include "PlazzaCore.hpp"

int main(int ac, char **av)
{
    size_t threadNbr;

    if (ac != 2)
    {
        std::cerr << "./plazza [thread's number >= 0]" << std::endl;
        return (1);
    }
    threadNbr = std::strtoul(av[1], NULL, 0);
    if (threadNbr <= 0)
    {
        std::cerr << "./plazza [thread's number >= 0]" << std::endl;
        return (1);
    }
    signal(SIGPIPE, SIG_IGN);
    Semaphore::cleanSemaphora();
    plazza::PlazzaCore(threadNbr).run();
    return 0;
}
