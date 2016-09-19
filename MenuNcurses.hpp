//
// MenuNcurses.hpp for MNU in /home/gouet_v/NCURSES/MENU
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Mon Apr 18 21:45:42 2016 Victor Gouet
// Last update Wed Apr 20 14:44:45 2016 Victor Gouet
//

#ifndef MENUNNCURSES_HPP_
# define MENUNNCURSES_HPP_

# include <iostream>
# include <ncurses.h>
# include <vector>
# include <fcntl.h>

typedef struct s_file
{
public:
  s_file(std::string const &fileName = "", int newColorIndex = 0, std::string const &pathName = "")
  {
    _file = fileName;
    colorIndex = newColorIndex; 
    this->pathName = pathName;
  }

public:
  int		getColorIndex() const
  {
    return (colorIndex);
  }

  std::string const &getFileName() const
  {
    return (_file);
  }

  std::string const &getPath() const
  {
    return (pathName);
  }

  void		setColorIndex(int index)
  {
    colorIndex = index;
  }

private:
  std::string	_file;
  int		colorIndex;
  std::string	pathName;
}		t_file;

class   MenuNcurses
{
public:
  MenuNcurses(std::vector<t_file> const &choiceVector, int x = 1, int y = 1,
	      int width = 4, int height = 4, WINDOW *parrentWindow = stdscr,
	      std::string const &title = "TITLE");
  ~MenuNcurses();

  /*
   *	display the choiceVector
   */
public:
  void			display();

  /*
   *	method to move the cursor
   */
public:
  void			operator++();
  void			operator--();
  void			setCursor(int);

public:
  void		        newList(std::vector<t_file> const &);
  void			clear();
  void			select();
  void			deselect();

private:
  int			getMaxLenghtFromVector() const;

public:
  int			getX() const;
  int			getY() const;
  int			getWidth() const;
  int			getHeight() const;
  int			getCursor() const;
  std::vector<t_file> const  &getContainers() const;

public:
  t_file		&operator[](int);

public:
  int			getKey() const;

private:
  WINDOW			*win;
  std::vector<t_file>	_choiceVector;
  int				_x;
  int				_y;
  int				_width;
  int				_height;
  int				_cursor;
  WINDOW			*_parrentWindow;
  bool				isUp;
  int				_refHeight;
  bool				isSelected;
  std::string			_title;
};

#endif
