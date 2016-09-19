//
// MenuNCurses.cpp for MENU in /home/gouet_v/NCURSES/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Mon Apr 18 21:58:20 2016 Victor Gouet
// Last update Wed Apr 20 14:45:31 2016 Victor Gouet
//

#include "MenuNcurses.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

MenuNcurses::MenuNcurses(std::vector<t_file> const &choiceVector, int x, int y,
			 int width, int height, WINDOW *parrentWindow, std::string const &title)
  : _choiceVector(choiceVector), _x(x), _y(y), _width(width), _height(height), _cursor(0),
    _parrentWindow(parrentWindow), _refHeight(height), _title(title)
{
  _width = width;

  if ((_choiceVector.size() + 2) < static_cast<size_t>(_refHeight))
    _height = _choiceVector.size() + 2;
  else
    _height = _refHeight;

  int newX, newY;

  getmaxyx(parrentWindow, newY, newX);
  if (newY < _height)
    _height = newY - 1;

  if (newX < _width)
    _width = newX - 1;

  isUp = false;

  win = subwin(parrentWindow, _height, _width, _y, _x);

  keypad(win, TRUE);
  box(win, 0, 0);
  isSelected = false;
}

MenuNcurses::~MenuNcurses()
{
  if (win)
    {
      wclear(win);
      delwin(win);
    }
}

int			MenuNcurses::getMaxLenghtFromVector() const
{
  int		maxWidth = 0;

  for (std::vector<t_file>::const_iterator it = _choiceVector.begin(),
	 end = _choiceVector.end() ; it != end ; ++it)
    {
      if (maxWidth < static_cast<int>((*it).getFileName().size()))
	{
	  maxWidth = (*it).getFileName().size();
	}
    }
  return (maxWidth);
}

void			MenuNcurses::select()
{
  isSelected = true;
}

void			MenuNcurses::deselect()
{
  isSelected = false;
}

/*
 *	display the choiceVector
 */
void			MenuNcurses::display()
{
  int			i = 0;
  int			ref = 0;
  std::vector<t_file>::iterator it = _choiceVector.begin(),
    end = _choiceVector.end();

  if (!win)
    return ;
  wclear(win);
  wattroff(win, A_STANDOUT | COLOR_PAIR(6));
  if (isSelected)
    wattron(win, A_STANDOUT | COLOR_PAIR(6));
  box(win, 0, 0);
  mvwprintw(win, 0, _width / 2 - _title.size() / 2, _title.c_str());
  wattroff(win, A_STANDOUT | COLOR_PAIR(6));
  for (; it != end && (ref + _cursor) > (_height - 3); ++it)
    {
      --ref;
    }
  i = 0;
  for (; it != end ; ++it)
    {
      if ((*it).getColorIndex() != 0)
	wattron(win, COLOR_PAIR((*it).getColorIndex()));

      std::string str = (*it).getFileName();
      
      if (_cursor == i - ref)
	{
	  wattron(win, A_STANDOUT);
	}
      else
	{
	  wattroff(win, A_STANDOUT);
	}
      if (static_cast<int>(str.size()) > _width - 3)
	{
	  str.erase(_width - 7);
	  str += "...";
	}
      if (i + 1 < _height - 1)
	mvwprintw(win, i + 1, 2, str.c_str());
      
      // wattroff(win, COLOR_PAIR(0));
      wattroff(win, COLOR_PAIR((*it).getColorIndex()));

      ++i;
    }
  wattroff(win, A_STANDOUT | COLOR_PAIR(0));
  wrefresh(win);
}

void			MenuNcurses::newList(std::vector<t_file> const &choiceVector)
{
  _choiceVector = choiceVector;

  if ((_choiceVector.size() + 2) < static_cast<size_t>(_refHeight))
    _height = _choiceVector.size() + 2;
  else
    _height = _refHeight;

  int x, y;

  getmaxyx(_parrentWindow, y, x);
  if (y < _height)
    _height = y - 1;

  if (x < _width)
    _width = x - 1;

  wclear(win);
  wrefresh(win);
  wresize(win, _height, _width);
 
  box(win, 0, 0);
  _cursor = 0;
}

void			MenuNcurses::clear()
{
  _choiceVector.clear();
}

std::vector<t_file> 	const &MenuNcurses::getContainers() const
{
  return (this->_choiceVector);
}

/*
 *	method to move the cursor
 */
void			MenuNcurses::operator++()
{
  isUp = true;
  if (_choiceVector.size() == 0)
    return ;
  _cursor = ((_cursor + 1) % _choiceVector.size());
}

void			MenuNcurses::operator--()
{
  isUp = false;
  _cursor = (((_cursor - 1) < 0) ? _choiceVector.size() - 1 : _cursor - 1);
}

t_file		&MenuNcurses::operator[](int index)
{
  return (_choiceVector[index]);
}

void			MenuNcurses::setCursor(int cursor)
{
  _cursor = cursor;
}

int			MenuNcurses::getX() const
{
  return (_x);
}

int			MenuNcurses::getY() const
{
  return (_y);
}

int			MenuNcurses::getWidth() const
{
  return (_width);
}

int			MenuNcurses::getHeight() const
{
  return (_height);
}

int			MenuNcurses::getKey() const
{
  return (wgetch(win));
}

int			MenuNcurses::getCursor() const
{
  return (_cursor);
}
