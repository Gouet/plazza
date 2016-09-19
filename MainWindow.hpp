//
// MainWindow.hpp for MAIN in /home/gouet_v/NCURSES/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Tue Apr 19 12:25:34 2016 Victor Gouet
// Last update Sat Apr 23 14:12:17 2016 Victor Gouet
//

#ifndef MAINWINDOW_HPP_
# define MAINWINDOW_HPP_

# include <map>
# include "MenuNcurses.hpp"
# include "ExploreDirectory.hpp"
# include "Pipe.hpp"
# include "NamedPipe.hpp"
# include "Mutex.hpp"

class	MainWindow
{
typedef int (MainWindow::*catchKeycode)();

public:
  MainWindow();
  ~MainWindow();

  static const int INVALID = 2;

public:
  enum Command
    {
      IP_ADDRESS = 1, // RED
      EMAIL_ADDRESS = 2, // GREEN
      PHONE_NUMBER = 3 // YELLOW
    };

public:
  void		operator+=(MenuNcurses *menu);

public:
  static void	        launch(NamedPipe &commandLine, NamedPipe &output, NamedPipe *log);

private:
  std::string		convertToCommandOutput() const;
  void			displayLegend();

public:
  std::string		run(Mutex &mutexGlobal);

public:
  std::vector<MenuNcurses *> const &	getMenuList() const;
  WINDOW				*getWindow() const;

public:
  void					setExploreDirectory(ExploreDirectory const &);

public:
  int			onLeft();
  int			onRight();
  int			onUp();
  int			onDown();
  int		        onSpace();
  int			onBackSpace();
  int			onControlLeft();
  int			onControlRight();
  int		        onEnter();

private:
  std::vector<MenuNcurses *>		_menuList;
  WINDOW				*window;
  int					_cursor;
  std::map<int, catchKeycode>		_keycodeMap;
  ExploreDirectory			explore;
};

#endif
