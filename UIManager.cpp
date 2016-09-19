//
// Created by gaspar-q on 20/04/16.
//

#include <unistd.h>
#include "UIManager.hpp"
#include "NCurses.hpp"
#include "MainWindow.hpp"
#include "SDL/WindowManagerSDL.hpp"

const std::string    UIManager::textMode = "text";
const std::string    UIManager::graphicMode = "graphic";
const std::string   UIManager::uiCommandline = "commandline.fifo";
const std::string   UIManager::uiOutput = "output.fifo";
const std::string   UIManager::uiLog = "log.fifo";

UIManager::UIManager(size_t threadNb) :
        threadNb(threadNb)
{

}

UIManager::~UIManager()
{
}

void UIManager::parallelPlazza()
{
    plazza::PlazzaCore  core(threadNb);
    std::string command;
    std::string eof;

    eof += EOF;
    commandLine = new NamedPipe(UIManager::uiCommandline, NamedPipe::IN);
    output = new NamedPipe(UIManager::uiOutput, NamedPipe::OUT);
    log = new NamedPipe(UIManager::uiLog, NamedPipe::OUT);
    dup2(output->getFd(), 1);
    while (1)
    {
      *commandLine >> command;
        if (command.find(EOF) != std::string::npos)
            break;
        core.resolveCommandLine(command, *output, log);
        *output << eof;
    }
    *output << eof;
    delete (commandLine);
    delete (output);
    delete (log);
}

void UIManager::launchUI(std::string const &dispMode)
{
    int     fd;

    signal(SIGPIPE, SIG_IGN);
    Semaphore::cleanSemaphora();
    if ((fd = open("errors.log", O_WRONLY | O_CREAT | O_TRUNC, 0644)) != -1)
        plazzaProcess.setStdErr(fd);
    plazzaProcess.launch(ActionHandler<UIManager>(*this, &UIManager::parallelPlazza));
    if (fd != -1)
        close(fd);
    std::string eof;

    eof += EOF;
    commandLine = new NamedPipe(UIManager::uiCommandline, NamedPipe::OUT);
    output = new NamedPipe(UIManager::uiOutput, NamedPipe::IN);
    output->setTimeout(10);
    log = new NamedPipe(UIManager::uiLog, NamedPipe::IN);
    log->setTimeout(1);
    if (dispMode == UIManager::textMode)
    {
        NCurses::init();
        MainWindow::launch(*commandLine, *output, log);
        NCurses::destroy();
    }
    else if (dispMode == UIManager::graphicMode)
    {
        try
        {
            WindowManagerSDL win;
            win.Loop(commandLine, output);
        }
        catch (WindowManagerSDL::SdlException e)
        {
            std::cerr << e.what() << std::endl;
        }

    }
    *commandLine << eof;
    plazzaProcess.waitSon();
    commandLine->unlinkPipe();
    output->unlinkPipe();
    log->unlinkPipe();
    delete(commandLine);
    delete(output);
    delete(log);
}







