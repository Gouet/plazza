//
// NcursesLoading.cpp for LOAD in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Thu Apr 21 21:14:01 2016 Victor Gouet
// Last update Fri Apr 22 15:15:40 2016 Victor Gouet
//

# include <unistd.h>
# include "NCursesLoading.hpp"

NcursesLoading::NcursesLoading(int x, int y, std::vector<int> const &colors, Mutex &mutex)
  : _continue(false), _x(x), _y(y), _colors(colors), _mutexGlobal(mutex)
{
  loadingCase[0].setX(x - 1);
  loadingCase[0].setY(y - 1);
  loadingCase[0].setColor(0 % _colors.size());

  loadingCase[1].setX(x);
  loadingCase[1].setY(y - 1);
  loadingCase[1].setColor(1 % _colors.size());

  loadingCase[2].setX(x + 1);
  loadingCase[2].setY(y - 1);
  loadingCase[2].setColor(2 % _colors.size());

  loadingCase[3].setX(x + 1);
  loadingCase[3].setY(y);
  loadingCase[3].setColor(3 % _colors.size());

  loadingCase[4].setX(x + 1);
  loadingCase[4].setY(y + 1);
  loadingCase[4].setColor(4 % _colors.size());
  
  loadingCase[5].setX(x);
  loadingCase[5].setY(y + 1);
  loadingCase[5].setColor(5 % _colors.size());

  loadingCase[6].setX(x - 1);
  loadingCase[6].setY(y + 1);
  loadingCase[6].setColor(6 % _colors.size());

  loadingCase[7].setX(x - 1);
  loadingCase[7].setY(y);
  loadingCase[7].setColor(7 % _colors.size());

  loadThread = new Thread<NcursesLoading>(NULL, *this);

  *loadThread += &NcursesLoading::display;
}

NcursesLoading::~NcursesLoading()
{
  delete loadThread;
}

void			NcursesLoading::run()
{
  loadThread->run();
}

void			NcursesLoading::cancel()
{
  loadThread->cancel();
}

void			NcursesLoading::stop()
{
  _mutex.Lock();
  _continue = false;
  _mutex.Unlock();
  int	i = 0;

  while (i < 8)
    {
      mvwprintw(stdscr, this->loadingCase[i].getY(), this->loadingCase[i].getX(), " ");
      ++i;
    }
}

thread::State		NcursesLoading::display(void *)
{
  _mutex.Lock();
  _continue = true;
  _mutex.Unlock();

  while (1)
    {
      _mutex.Lock();
      if (_continue == false)
	{
	  _mutex.Unlock();
	  break;
	}
      _mutex.Unlock();

      int	i = 0;

      if (_mutexGlobal.trylock() == 0)
	{
	  while (i < 8)
	    {
	      if (this->_colors[this->loadingCase[i].getColorIndex()] != 0)
		wattron(stdscr, A_STANDOUT | COLOR_PAIR(this->_colors[this->loadingCase[i].getColorIndex()]));
	      mvwprintw(stdscr, this->loadingCase[i].getY(), this->loadingCase[i].getX(), " ");
	      wattroff(stdscr, A_STANDOUT | COLOR_PAIR(this->_colors[this->loadingCase[i].getColorIndex()]));
	      this->loadingCase[i].setColor((this->loadingCase[i].getColorIndex() + 1) % this->_colors.size());
	      ++i;
	    }

	  wrefresh(stdscr);
	  _mutexGlobal.Unlock();
	}
 
      usleep(90000);
    }

  return (thread::Restart);
}

bool		        NcursesLoading::isContinue() const
{
  return (_continue);
}
