//
// NcursesDisplayProcess.cpp for PROCESS in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Fri Apr 22 10:55:09 2016 Victor Gouet
// Last update Sat Apr 23 18:53:45 2016 Victor Gouet
//

#include <string>
#include <iostream>
#include "NcursesDisplayProcess.hpp"

NcursesDisplayProcess::NcursesDisplayProcess(int x, int y, Mutex &mutex, NamedPipe *log)
  : _x(x), _y(y), _mutexGlobal(mutex), _isRunning(false)
{
  thread = new Thread<NcursesDisplayProcess>((void *)log, *this);
  *thread += &NcursesDisplayProcess::display;

  int	y1;
  int	x1;

  getmaxyx(stdscr, y1, x1);

  _win = subwin(stdscr, y1 - y, x1 - x, y, x);
  // box(_win, 0, 0);
}

NcursesDisplayProcess::~NcursesDisplayProcess()
{
  delete thread;
}

void			NcursesDisplayProcess::run()
{
  _isRunning = true;
  thread->run();
}

void			NcursesDisplayProcess::cancel()
{
  _isRunning = false;
  thread->cancel();
}

void		        NcursesDisplayProcess::onDisplayProcessInfo(ProcessInfo *info,
								    int &posX,
								    int &posY)
{
  if (info->getThread())
    {
      wattron(_win, COLOR_PAIR(5));
      mvwprintw(_win, posY, posX, "   -> ==%d== State: %s", info->getPid(), info->getStatus().c_str());
    }
  else
    {
      wattron(_win, COLOR_PAIR(6));
      mvwprintw(_win, posY, posX, "==%d== State: %s", info->getPid(), info->getStatus().c_str());
    }
  if (info->getThread())
    {
      wattroff(_win, COLOR_PAIR(5));
    }
  else
    {
      wattroff(_win, COLOR_PAIR(6));
    }

  ++posY;
}

thread::State		NcursesDisplayProcess::display(void *logPipe)
{
    NamedPipe       *log = static_cast<NamedPipe *>(logPipe);

  std::list<ProcessInfo *>::iterator	it;
  std::string				output;

  while (_isRunning)
    {
      int					posX = 0;//_x;
      int					posY = 0;//_y;


      *log >> output;
      size_t pos = 0;

      while ((pos = output.find('\n')) != std::string::npos)
	{
	  int	firstTime = 0;
	  std::string pid = output.substr(0, pos);
	  try {
	    _process.push_back(new ProcessInfo(std::stoi(pid.c_str())));
	  } catch (std::exception &e)
	    {
	      // continue;
	    }
	  std::string brutData = Process::getOutput("pstree -p " + pid);

	  brutData.erase(brutData.size() - 1);

	  size_t posPIDThread = 0;
	  while ((posPIDThread = brutData.find(')')) != std::string::npos)
	    {

	      size_t posPIDThreadEnd = 0;
	      if ((posPIDThreadEnd = brutData.find('(')) != std::string::npos)
		{
		  std::string threadPid = brutData.substr(posPIDThreadEnd + 1,
							  posPIDThread - posPIDThreadEnd - 1);
		  if (firstTime != 0)
		    {
		      try {
			if (std::stoi(threadPid.c_str()) > getpid())
			  _process.push_back(new ProcessInfo(std::stoi(threadPid.c_str()), true));
		    } catch (std::exception &e)
			{
			  
			}
		    }
		  ++firstTime;
		}
	      brutData.erase(0, posPIDThread + 1);
	    }

	  output.erase(0, pos + 1);
	}

      _mutexGlobal.Lock();
      wclear(_win);
      it = _process.begin();
      while (it != _process.end())
	{
	  if (!(*it)->open() || (*it)->getStatus().find('Z') != std::string::npos)
	    {
	      delete *it;
	      it = _process.erase(it);
	    }
	  else
	    {

	      (*it)->clear();
	      (*it)->extractData();
	      if ((*it)->getStatus().find('Z') != std::string::npos)
		{
		  (*it)->close();
		  delete *it;
		  it = _process.erase(it);
		}
	      else
		{
		  onDisplayProcessInfo(*it, posX, posY);
		  (*it)->close();
		  ++it;
		}
	    }
	}
      wrefresh(_win);
      _mutexGlobal.Unlock();

    }
  return (thread::Restart);
}
