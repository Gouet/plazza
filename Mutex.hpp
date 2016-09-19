//
// mutex.hpp for MUTEX in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Fri Apr  8 13:22:58 2016 Victor Gouet
// Last update Fri Apr  8 13:31:58 2016 Victor Gouet
//

#ifndef MUTEX_HPP_
# define MUTEX_HPP_

# include <pthread.h>
# include <iostream>
# include <stdexcept>
#include "ILockable.hpp"

class	Mutex : public ILockable
{
public:
  Mutex();
  ~Mutex();

public:
  int		Lock() const;
  int		Unlock() const;
  int		trylock();
    pthread_mutex_t const &getCMut() const;
    pthread_mutex_t &operator*();

private:
  mutable pthread_mutex_t	_mutex;
};

#endif
