//
// NCurses.hpp for NCURSES in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 20 12:36:57 2016 Victor Gouet
// Last update Wed Apr 20 12:44:40 2016 Victor Gouet
//

#ifndef NCURSES_HPP_
# define NCURSES_HPP_

# include <ncurses.h>

class	NCurses
{
private:
  NCurses();

public:
  static int init();
  static int destroy();
};

#endif
