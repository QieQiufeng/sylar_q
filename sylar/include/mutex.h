#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>
#include "noncopyable.h"

template <class T>
struct ScopedLockImpl{
  public:
  ScopedLockImpl(T& mutx):
  m_mutx(mutx){
    mutx.lock();
    m_locked = true;
    
  }
  ~ScopedLockImpl(){
    unlock();
  }

  void lock(){
  if(!m_locked){
      m_mutx.lock();
      m_locked = true;
    }
  }
  void unlock(){
    if(m_locked){
      m_mutx.unlock();
      m_locked = false;
    }
  }
  private:
  T& m_mutx;
  bool m_locked;
};
/**
 * @class Spinlock
 * @brief 自旋锁
 *
 */
class Spinlock : Noncopyable{
public:
typedef ScopedLockImpl<Spinlock> Lock ;
Spinlock(){
    pthread_spin_init(&m_mutx,0);
  }
  ~Spinlock(){
    pthread_spin_destroy(&m_mutx);
  }
  void lock(){
    pthread_spin_lock(&m_mutx);
  }
void unlock(){
    pthread_spin_unlock(&m_mutx);
  }
private:
  pthread_spinlock_t m_mutx;
};
#endif // !_MUTEX_H_
