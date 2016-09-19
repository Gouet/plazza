//
// NCurses.cpp for NCURSES in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 20 12:34:38 2016 Victor Gouet
// Last update Wed Apr 20 12:45:10 2016 Victor Gouet
//

#include "NCurses.hpp"

int	NCurses::init()
{
  if (initscr() == NULL)
    return (-1);
  if (noecho() == ERR)
    return (-1);
  curs_set(0);
  set_escdelay(0);  
  start_color();
  if (init_pair(1, COLOR_RED, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  if (init_pair(2, COLOR_GREEN, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  if (init_pair(3, COLOR_YELLOW, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  if (init_pair(4, COLOR_BLUE, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  if (init_pair(5, COLOR_MAGENTA, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  if (init_pair(6, COLOR_CYAN, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  if (init_pair(7, COLOR_WHITE, COLOR_BLACK) == ERR)
    return (endwin(), -1);
  raw();
  keypad(stdscr, true);
  return (0);
}

int	 NCurses::destroy()
{
  return (endwin());
}
