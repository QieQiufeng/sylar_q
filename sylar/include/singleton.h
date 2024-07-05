#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <locale>
/**
 * @brief 单例模式
 */
template<class T,class X=void,int N=0>
class Singleton{
  public:

  static T*GetInstance(){
    static T v;
    return &v;
  }
};



#endif // !_SINGLETON_H_
