//
// Created by gaspar-q on 20/04/16.
//

#include "UIManager.hpp"

int main(int ac, char **av)
{
    size_t threadNbr;

    if (ac != 2 && ac != 3)
    {
        std::cerr << "./plazza [thread's number >= 0] [-graphic -text]" << std::endl;
        return (1);
    }
    threadNbr = std::strtoul(av[1], NULL, 0);
    if (threadNbr <= 0)
    {
        std::cerr << "./plazza [thread's number >= 0] [-graphic -text]" << std::endl;
        return (1);
    }
    UIManager   manager(threadNbr);
    std::string graphMode(UIManager::textMode);

    if (ac == 3 && std::string(av[2]).find(UIManager::graphicMode) != std::string::npos)
    {
        graphMode = UIManager::graphicMode;
    }
    manager.launchUI(graphMode);
    return 0;
}