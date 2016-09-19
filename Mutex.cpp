//
// mutex.cpp for MUTEX in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Fri Apr  8 13:22:37 2016 Victor Gouet
// Last update Mon Apr 11 10:18:44 2016 Victor Gouet
//

#include "Mutex.hpp"

Mutex::Mutex()
{
  if (pthread_mutex_init(&_mutex, NULL) != 0)
    {
      throw std::runtime_error("pthread_mutex_init failed");
    }    
}

Mutex::~Mutex()
{
  pthread_mutex_destroy(&_mutex);
}

int		Mutex::Lock() const
{
  return (pthread_mutex_lock(&_mutex));
}

int		Mutex::Unlock() const
{
  return (pthread_mutex_unlock(&_mutex));
}

int		Mutex::trylock()
{
  return (pthread_mutex_trylock(&_mutex));
}

pthread_mutex_t const &Mutex::getCMut() const
{
  return _mutex;
}

pthread_mutex_t &Mutex::operator*()
{
  return _mutex;
}
