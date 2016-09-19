//
// MainWindow.cpp for MENU in /home/gouet_v/NCURSES/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Tue Apr 19 12:31:47 2016 Victor Gouet
// Last update Sat Apr 23 16:29:33 2016 Victor Gouet
//

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "MainWindow.hpp"
#include "NamedPipe.hpp"
#include "NCursesLoading.hpp"
#include "NcursesDisplayProcess.hpp"

MainWindow::MainWindow()
{
    window = stdscr;
    keypad(window, TRUE);
    _cursor = 0;
    _keycodeMap[KEY_LEFT] = &MainWindow::onLeft;
    _keycodeMap[KEY_RIGHT] = &MainWindow::onRight;
    _keycodeMap[KEY_DOWN] = &MainWindow::onDown;
    _keycodeMap[KEY_UP] = &MainWindow::onUp;
    _keycodeMap[' '] = &MainWindow::onSpace;
    _keycodeMap[KEY_BACKSPACE] = &MainWindow::onBackSpace;
    _keycodeMap[KEY_SLEFT] = &MainWindow::onControlLeft;
    _keycodeMap[KEY_SRIGHT] = &MainWindow::onControlRight;
    _keycodeMap['\n'] = &MainWindow::onEnter;
}

void        MainWindow::launch(NamedPipe &commandLine, NamedPipe &outputChannel, NamedPipe *log)
{
    std::vector<t_file> list;

    ExploreDirectory explore;

    explore.openDirectory();

    for (std::list<struct dirent>::const_iterator it = explore.getFiles().begin(),
                 end = explore.getFiles().end(); it != end; ++it)
    {
        std::string path = explore.getPathDirectoryName() + (*it).d_name;

        struct stat stats;

        if (stat(path.c_str(), &stats) == -1)
            break;

        if (S_IFDIR & stats.st_mode)
            list.push_back(t_file((*it).d_name, 4, "./"));
        else
            list.push_back(t_file((*it).d_name, 0, "./"));
    }

    MainWindow mainWindow;

    MenuNcurses *menu = new MenuNcurses(list,
                                        1, 3, 30, 20,
                                        mainWindow.getWindow(), "Browse files");

    std::vector<t_file> newList;


    MenuNcurses *containersSelectedFile = new MenuNcurses(newList,
                                                          menu->getWidth() + 3, 3, 30, 20,
                                                          mainWindow.getWindow(), "Files");


    MenuNcurses *displayOutput = new MenuNcurses(newList,
                                                 2, 24, 60, 20,
                                                 mainWindow.getWindow(), "Output");

    mainWindow.setExploreDirectory(explore);


    mainWindow += menu;
    mainWindow += containersSelectedFile;
    mainWindow += displayOutput;

    mainWindow.displayLegend();

    std::vector<int>	colors;

    colors.push_back(3);
    colors.push_back(1);
    colors.push_back(0);
    colors.push_back(0);

    Mutex		mutexGloabal;

    NcursesLoading	loading(31, 27, colors, mutexGloabal);

    NcursesDisplayProcess	displayProcess(70, 1, mutexGloabal, log);

    displayProcess.run();

    std::vector<t_file> outputList;
    std::string output;

    // mutexGloabal.Lock();
    while ((output = mainWindow.run(mutexGloabal)) != "")
    {
      // mutexGloabal.Unlock();

        commandLine << output;

        /**
         * Read on output pipe while a thread is running
         */
	outputList.clear();
        output.clear();

	std::string eof;
	eof += EOF;

	loading.run();

        bool hastoQuit = false;
        size_t posEOF;

        while (!hastoQuit)
        {
            outputChannel >> output;

            if (output.empty())
                break;
            if ((posEOF = output.find(EOF)) != std::string::npos)
            {
                hastoQuit = true;
                output.erase(posEOF);
            }
	    size_t pos = 0;
	    while ((pos = output.find('\n')) != std::string::npos)
	      {
		std::string order;

		order = output.substr(0, pos);
		output.erase(0, pos + 1);
		outputList.push_back(t_file(order, 0, ""));
	      }
        }
	loading.stop();

	mutexGloabal.Lock();

	displayOutput->clear();
        displayOutput->newList(outputList);
	mutexGloabal.Unlock();
    }
    //mutexGloabal.Unlock();
    displayProcess.cancel();
    loading.cancel();

    delete(containersSelectedFile);
    delete(displayOutput);
    delete(menu);
}

MainWindow::~MainWindow()
{
    if (window)
    {
        wclear(window);
        delwin(window);
    }
}

void		MainWindow::displayLegend()
{
  wattron(stdscr, A_STANDOUT | COLOR_PAIR(IP_ADDRESS));
  mvwprintw(stdscr, 1, 1, " ");
  wattroff(stdscr, A_STANDOUT | COLOR_PAIR(IP_ADDRESS));
  wprintw(stdscr, " IP_ADDRESS | ");
  wattron(stdscr, A_STANDOUT | COLOR_PAIR(EMAIL_ADDRESS));
  wprintw(stdscr, " ");
  wattroff(stdscr, A_STANDOUT | COLOR_PAIR(EMAIL_ADDRESS));
  wprintw(stdscr, " EMAIL_ADDRESS | ");
  wattron(stdscr, A_STANDOUT | COLOR_PAIR(PHONE_NUMBER));
  wprintw(stdscr, " ");
  wattroff(stdscr, A_STANDOUT | COLOR_PAIR(PHONE_NUMBER));
  wprintw(stdscr, " PHONE_NUMBER");
}

int                MainWindow::onEnter()
{
    if (_menuList[1]->getContainers().size() == 0)
        return (0);
    this->_menuList[_cursor]->deselect();
    _cursor = 0;
    return (-1);
}

int            MainWindow::onControlRight()
{
    std::vector<t_file> list;
    int i = this->_menuList[_cursor]->getCursor();

    if (_cursor == INVALID)
        return (0);
    list = this->_menuList[_cursor]->getContainers();
    if (list.empty())
        return (0);
    if (this->_menuList[_cursor]->operator[](i).getColorIndex() == 4)
        return (0);
    if (_cursor == 0)
    {
        if (this->_menuList[_cursor]->operator[](i).getColorIndex() == 0)
            this->_menuList[_cursor]->operator[](i).setColorIndex(3);
        else
            this->_menuList[_cursor]->operator[](i).setColorIndex(
                    this->_menuList[_cursor]->operator[](i).getColorIndex() - 1);
    }
    else if (_cursor == 1)
    {
        this->_menuList[_cursor]->operator[](i).setColorIndex(
                this->_menuList[_cursor]->operator[](i).getColorIndex() - 1 == 0 ? 3 :
                this->_menuList[_cursor]->operator[](i).getColorIndex() - 1);
    }
    return (0);
}

int            MainWindow::onControlLeft()
{
    int i = this->_menuList[_cursor]->getCursor();

    if (_cursor == INVALID)
        return (0);
    std::vector<t_file> list = this->_menuList[_cursor]->getContainers();
    if (list.empty())
        return (0);

    if (this->_menuList[_cursor]->operator[](i).getColorIndex() == 4)
        return (0);
    if (_cursor == 0)
    {
        if (this->_menuList[_cursor]->operator[](i).getColorIndex() == PHONE_NUMBER)
            this->_menuList[_cursor]->operator[](i).setColorIndex(0);
        else
            this->_menuList[_cursor]->operator[](i).setColorIndex(
                    this->_menuList[_cursor]->operator[](i).getColorIndex() + 1);
    }
    else if (_cursor == 1)
    {
        this->_menuList[_cursor]->operator[](i).setColorIndex(
                this->_menuList[_cursor]->operator[](i).getColorIndex() + 1 == 4 ? 1 :
                this->_menuList[_cursor]->operator[](i).getColorIndex() + 1);
    }
    return (0);
}

int            MainWindow::onBackSpace()
{
    std::vector<t_file> list;

    if (_cursor == INVALID)
        return (0);
    if (_cursor == 1)
    {
        list = this->_menuList[_cursor]->getContainers();
        int i = 0;
        for (std::vector<t_file>::iterator it = list.begin(),
                     end = list.end(); it != end; ++it)
        {
            if (i == this->_menuList[_cursor]->getCursor())
            {
                list.erase(it);
                break;
            }
            ++i;
        }
        this->_menuList[_cursor]->newList(list);
    }
    return (0);
}

int            MainWindow::onLeft()
{
    this->_menuList[_cursor]->deselect();
    this->_menuList[_cursor]->display();
    _cursor = _cursor - 1 < 0 ? _menuList.size() - 1 : _cursor - 1;
    this->_menuList[_cursor]->select();
    return (0);
}

int            MainWindow::onRight()
{
    this->_menuList[_cursor]->deselect();
    this->_menuList[_cursor]->display();
    _cursor = ((_cursor + 1) >= static_cast<int>(_menuList.size()))
              ? 0 : _cursor + 1;
    this->_menuList[_cursor]->select();
    return (0);
}

int            MainWindow::onUp()
{
    --(*this->_menuList[_cursor]);
    return (0);
}

int            MainWindow::onDown()
{
    ++(*this->_menuList[_cursor]);
    return (0);
}

int            MainWindow::onSpace()
{
    std::vector<t_file> list;

    if (_cursor == INVALID)
        return (0);
    list = this->_menuList[_cursor]->getContainers();
    if (_cursor == 0)
    {
        std::string file = list[this->_menuList[_cursor]->getCursor()].getFileName();
        if ((explore += file) == false)
        {
            std::vector<t_file> list0 = this->_menuList[0]->getContainers();
            list = this->_menuList[1]->getContainers();
            list.push_back(t_file(file,
                                  list0[this->_menuList[_cursor]->getCursor()].getColorIndex() ==
                                  0 ? 1 : list0[this->_menuList[_cursor]->getCursor()].getColorIndex(),
                                  this->explore.getPathDirectoryName()));

            this->_menuList[1]->newList(list);
            this->_menuList[1]->display();
            this->_menuList[_cursor]->operator[](this->_menuList[_cursor]->getCursor()).setColorIndex(0);

            return (0);
        }
        explore.openDirectory();
        this->_menuList[_cursor]->clear();

        list.clear();

        for (std::list<struct dirent>::const_iterator it = explore.getFiles().begin(),
                     end = explore.getFiles().end(); it != end; ++it)
        {
            std::string path = explore.getPathDirectoryName() + (*it).d_name;

            struct stat stats;

            if (stat(path.c_str(), &stats) == -1)
                break;

            if (S_IFDIR & stats.st_mode)
                list.push_back(t_file((*it).d_name, 4, this->explore.getPathDirectoryName()));
            else
                list.push_back(t_file((*it).d_name, 0, this->explore.getPathDirectoryName()));

        }
        this->_menuList[_cursor]->newList(list);
    }
    return (0);
}

void        MainWindow::operator+=(MenuNcurses *menu)
{
    this->_menuList.push_back(menu);
}

WINDOW *MainWindow::getWindow() const
{
    return (window);
}

void                    MainWindow::setExploreDirectory(ExploreDirectory
                                                        const &explore)
{
    this->explore = explore;
}

std::string        MainWindow::convertToCommandOutput() const
{
    std::string output;
    std::string command[3] = {
            "IP_ADDRESS",
            "EMAIL_ADDRESS",
            "PHONE_NUMBER"
    };

    for (std::vector<t_file>::const_iterator it = _menuList[1]->getContainers().begin(), end = _menuList[1]->getContainers().end();
         it != end; ++it)
    {
        output += (*it).getPath() + (*it).getFileName();
        output += " ";
        if ((*it).getColorIndex() >= 1 && (*it).getColorIndex() <= 3)
            output += command[(*it).getColorIndex() - 1];
        output += ";";
    }
    return (output);
}

std::string        MainWindow::run(Mutex &mutexGlobal)
{
    int keycode = 0;

    if (_menuList.empty())
    {
        std::cerr << "MainWindow: there is no list" << std::endl;
        return "";
    }
    int i = 0;
    mutexGlobal.Lock();
    for (std::vector<MenuNcurses *>::iterator it = this->_menuList.begin(), end = this->_menuList.end();
         it != end; ++it)
    {
        if (i == 0)
            (*it)->select();
        (*it)->display();
        ++i;
    }
    mutexGlobal.Unlock();
    while ((keycode = getch()) != 27)
    {
      mutexGlobal.Lock();
        std::map<int, catchKeycode>::iterator itMap = _keycodeMap.find(keycode);
        if (itMap != _keycodeMap.end())
        {
            if ((this->*(itMap->second))() == -1)
	      {
		mutexGlobal.Unlock();
                return convertToCommandOutput();
	      }
        }
        (this->_menuList[_cursor])->display();
	mutexGlobal.Unlock();
    }
    return ("");
}

std::vector<MenuNcurses *> const &MainWindow::getMenuList() const
{
    return (this->_menuList);
}
