#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_


class Noncopyable{
  public:
  /**
   * @brief 默认构造
   */
  Noncopyable()=default;
  /**
   * @brief 默认析构
   */
  ~Noncopyable()=default;
  /**
   * @brief 禁止拷贝构造
   */
  Noncopyable(const Noncopyable&)=delete;
  /**
   * @brief 禁止赋值
   *
   * @return 
   */
  Noncopyable & operator = (const Noncopyable&)=delete;
  
};

#endif // !_NONCOPYABLE_H_ 
