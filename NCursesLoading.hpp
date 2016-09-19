//
// NCursesLoading.hpp for LOAD in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Thu Apr 21 21:08:31 2016 Victor Gouet
// Last update Fri Apr 22 14:11:08 2016 Victor Gouet
//

#ifndef NCURSESLOADING_HPP_
# define NCURSESLOADING_HPP_

# include <vector>
# include <ncurses.h>
# include "Thread.hpp"
# include "Mutex.hpp"

class	NcursesLoading
{
public:
  NcursesLoading(int x, int y, std::vector<int> const &colors,
		 Mutex &mutex);
  ~NcursesLoading();

public:
  void			stop();
  void			run();
  void			cancel();

public:
  thread::State		display(void *);

public:
  bool		        isContinue() const;

private:
  class	        LoadingCase
  {
  public:
    LoadingCase(int x = 0, int y = 0) : _x(x), _y(y), _index(0)
    {
    }

  public:
    int		getX() const { return (_x); }
    int		getY() const { return (_y); }
    int		getColorIndex() const { return _index; }
    void        setColor(int index) { _index = index; }
    void        setY(int y) { _y = y; }
    void        setX(int x) { _x = x; }
    

  private:
    int	_x;
    int _y;
    int _index;
  };

private:
  bool				_continue;
  Mutex				_mutex;
  int				_x;
  int				_y;
  std::vector<int>		_colors;
  LoadingCase			loadingCase[8];
  Thread<NcursesLoading>	*loadThread;//(NULL, loading);
  Mutex				&_mutexGlobal;
};

#endif
