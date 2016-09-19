//
// NcursesDisplayProcess.hpp for PROCESS in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Fri Apr 22 10:54:40 2016 Victor Gouet
// Last update Sat Apr 23 12:43:36 2016 Victor Gouet
//

#ifndef NCURSESDISPLAYPROCESS_HPP_
# define NCURSESDISPLAYPROCESS_HPP_

# include <ncurses.h>
# include <list>
# include "Thread.hpp"
# include "ProcessInfo.hpp"
# include "NamedPipe.hpp"
# include "Process.hpp"
# include <string>

class	NcursesDisplayProcess
{
public:
  NcursesDisplayProcess(int x, int y, Mutex &mutex, NamedPipe *log);
  ~NcursesDisplayProcess();

public:
  thread::State		display(void *);
  void			run();
  void		        cancel();

private:
  void		        onDisplayProcessInfo(ProcessInfo *,
					     int &posX,
					     int &posY);

private:
  int					_x;
  int					_y;
  std::list<ProcessInfo *>		_process;		
  Thread<NcursesDisplayProcess>		*thread;
  Mutex					&_mutexGlobal;
  bool					_isRunning;
  WINDOW				*_win;
};

#endif
