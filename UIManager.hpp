//
// Created by gaspar-q on 20/04/16.
//

#ifndef CPP_PLAZZA_UIMANAGER_HPP
#define CPP_PLAZZA_UIMANAGER_HPP

#include "PlazzaCore.hpp"
#include "Pipe.hpp"

class UIManager
{
public:
    static const std::string    textMode;
    static const std::string    graphicMode;
    static const std::string    uiCommandline;
    static const std::string    uiOutput;
    static const std::string    uiLog;

public:
    UIManager(size_t);
    ~UIManager();

public:
    void    launchUI(std::string const &);
    void    parallelPlazza();

private:
    size_t              threadNb;
    Process             plazzaProcess;
    NamedPipe           *commandLine;
    NamedPipe           *output;
    NamedPipe           *log;
};

#endif //CPP_PLAZZA_UIMANAGER_HPP
