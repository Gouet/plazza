//
// main.cpp for MAIN in /home/gouet_v/NCURSES/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Mon Apr 18 21:45:13 2016 Victor Gouet
// Last update Wed Apr 20 14:13:47 2016 Victor Gouet
//

#include "../MenuNcurses.hpp"
#include "../MainWindow.hpp"
#include "../ExploreDirectory.hpp"
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../NCurses.hpp"

int	main()
{

  NCurses::init();

  MainWindow::launch();

  NCurses::destroy();
}
