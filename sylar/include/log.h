#ifndef __LOG_H_
#define __LOG_H_

//日志级别
/**
 * @class LogLvl
 * @brief 
 *
 */
#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include "mutex.h"
#include "sylartype.h"
#include"singleton.h"

class Logger;
class LoggerMgr;


class LogLvl{
  public:
       
   /**
    * @brief 日志几倍枚举
    */
  enum Level{
    UNKNOW =0,
    DEBUG  =1,
    INFO   =2,
    WARN   =3,
    ERROR  =4,
    FATAL  =5

  };

  /**
   * @brief 将日志级别转为文本输出
   *
   * @param level 日志级别
   * @return 
   */
  static const char* toString(LogLvl::Level level);

  /**
   * @brief 将文本转为日志级别
   *
   * @param str 日志级别文本
   */
  static LogLvl::Level fromString(const std::string & str);
  
};

//日志事件

/**
 * @class LogEvent
 * @brief 
 *
 */
class LogEvent{
public:
  typedef std::shared_ptr<LogEvent> ptr; //日志事件指针
  LogEvent(std::shared_ptr<Logger> logger,LogLvl::Level level, const char* filename, 
         const u16& line,const u16& elapse,const u16& threadId,const u16& fiberId,
         const u64& time, const std::string& threadName);

  /**
   * @brief 获取文件路径
   *
   * @return 
   */
  inline  const char* getFile()const;
  /**
   * @brief 获取行号
   *
   * @return 
   */
  inline  u16 getLine() const;
  /**
   * @brief 获取启动时间
   *
   * @return 
   */
  inline  u16 getElapse()const;
  /**
   * @brief 获取线程id
   *
   * @return 
   */
  inline  u16 getThreadId() const;
  /**
   * @brief 获取协程id
   *
   * @return 
   */
  inline  u16 getFiberId()const;
  /**
   * @brief 获取时间戳
   *
   * @return 
   */
  inline  u64 getTime() const;
  /**
   * @brief 获取线程名
   */
  inline  std::string getThreadName()const;
  /**
   * @brief 获取日志内容
   */
  inline  std::string getContent()const;
  /**
   * @brief 获取日志内容流
   */
  inline  std::stringstream &getSS();
  /**
   * @brief 获取日志级别
   */
  inline  LogLvl::Level getLevel()const;
  /**
   * @brief 获取日志器
   *
   * @return 
   */
  inline  std::shared_ptr<Logger> getLoger()const;
  
  /**
   * @brief  日志格式 
   *
   * @param fmt 
   */
  void format(const char* fmt,...);
  void format(const char* fmt,va_list va);


private:
  //日志需要输出的事件内容私有变量
  const char* m_file = nullptr;     //文件名
  u16 m_line = 0;                   //行号
  u16 m_elapse = 0;                 //程序启动到现在运行的毫秒数
  u16 m_threadId = 0;               //线程id    
  u16 m_fiberId = 0;                //协程id 
  u64 m_time = 0;                   //时间戳
  std::string m_threadName;         //线程名称
  std::stringstream  m_ss;          //日志中的看内容流
  std::shared_ptr<Logger> m_loggerptr; //日志器
  LogLvl::Level m_level;               //日志级别
};


//日志事件包装器

class LogEventWrap{
  public:
  LogEventWrap(LogEvent::ptr ptr);
  ~LogEventWrap();
  /**
   * @brief 获取日志时间
   *
   * @return 
   */
  LogEvent::ptr getEvent()const{return m_event;};
  /**
   * @brief 获取日志时间内容流
   */
  std::stringstream &getSS();
  private:
  LogEvent::ptr m_event;
};
//日志格式化


class LogFormate{
  public:
  typedef std::shared_ptr<LogFormate> ptr;
  LogFormate(const std::string &pattren);
  /**
   * @brief 返回格式化日志文本
   *
   * @param logger 
   * @param level 
   * @param event 
   */
  std::string format(LogLvl::Level level,LogEvent::ptr event);
  std::ostream &format(std::ostream & ofs,LogLvl::Level level,LogEvent::ptr event);
public:

  /**
   * @brief 日志内容格式化
   */
  class FormateItem{
    public:
    typedef  std::shared_ptr<FormateItem> ptr;
    virtual ~FormateItem(){}
    virtual void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)=0;

  };
public:
  /**
   * @brief 初始化
   */
  void init();
  /**
   * @brief 是否有错误
   *
   * @return bool  
   */
  inline bool isERR()const{return m_error;}
  /**
   * @brief 获取日志模板
   *
   * @return std::string  
   */
  inline const std::string getPattern()const{return m_pattern;}
  private:
  /**
   * @brief 日志格式模板
   */
  std::string m_pattern;
  /**
   * @brief 日志解析后的格式
   */
  std::vector<FormateItem::ptr> m_items;
  /**
   * @brief 是否有错误
   */
  bool m_error = false; 
};
class MessageFormateItem : public LogFormate::FormateItem {
public:

  MessageFormateItem(const std::string& str = " ");
  ~MessageFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class LevelFormateItem
 * @brief 日志级别格式化子类
 *
 */
class LevelFormateItem : public LogFormate::FormateItem {
public:
  LevelFormateItem(const std::string& str = " ");
  ~LevelFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class ElapseFormateItem
 * @brief 累计启动毫秒数格式化子类
 *
 */
class ElapseFormateItem : public LogFormate::FormateItem {
public:
  ElapseFormateItem(const std::string& str = " ");
  ~ElapseFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class NameFormateItem
 * @brief 日志名格式化子类
 *
 */
class NameFormateItem : public LogFormate::FormateItem {
public:
  NameFormateItem(const std::string& str = " ");
  ~NameFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class ThreadIdFormateItem
 * @brief 线程id格式化子类
 *
 */
class ThreadIdFormateItem : public LogFormate::FormateItem {
public:
  ThreadIdFormateItem(const std::string& str = " ");
  ~ThreadIdFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class NewLineFormateItem
 * @brief 换行格式化子类
 *
 */
class NewLineFormateItem : public LogFormate::FormateItem {
public:
  NewLineFormateItem(const std::string& str = " ");
  ~NewLineFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class DateTimeFormateItem
 * @brief 日期格式化子类
 *
 */
class DateTimeFormateItem : public LogFormate::FormateItem {
public:
  DateTimeFormateItem(const std::string& fmt = "%Y-%m-%d   %H-%M-%S ");
  ~DateTimeFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
  private:
  std::string m_format;
};
/**
 * @class FileNameFormateItem
 * @brief 文件名格式化子类
 *
 */
class FileNameFormateItem : public LogFormate::FormateItem {
public:
  FileNameFormateItem(const std::string& str = " ");
  ~FileNameFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class LineFormateItem
 * @brief 行号格式化子类
 *
 */
class LineFormateItem : public LogFormate::FormateItem {
public:
  LineFormateItem(const std::string& str = " ");
  ~LineFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class TabFormateItem
 * @brief Tab符 格式化子类
 *
 */
class TabFormateItem : public LogFormate::FormateItem {
public:
  TabFormateItem(const std::string& str = " ");
  ~TabFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class FiberIdFormateItem
 * @brief 协程ID格式化子类
 *
 */
class FiberIdFormateItem : public LogFormate::FormateItem {
public:
  FiberIdFormateItem(const std::string& str = " ");
  ~FiberIdFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
/**
 * @class ThreadNameFormateItem
 * @brief 线程名格式化子类
 *
 */
class ThreadNameFormateItem : public LogFormate::FormateItem {
public:
  ThreadNameFormateItem(const std::string& str = " ");
  ~ThreadNameFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
};
class StringFormateItem : public LogFormate::FormateItem {
public:
  StringFormateItem(const std::string& str )
  :m_str(str){}
  ~StringFormateItem()override;
  void formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event) override;
  std::string m_str;
};




//日志输出

class LogAppender{
friend class Logger;

  public:
  typedef std::shared_ptr<LogAppender> ptr;
  typedef Spinlock MutexType ;
  virtual ~LogAppender(){}
  virtual void log(std::shared_ptr<Logger>logger ,const LogLvl::Level &level,LogEvent::ptr event)=0;
  virtual std::string toYmalString()=0;
  void setFormatter(LogFormate::ptr val);
  LogFormate::ptr getFormatter()const;
  inline LogLvl::Level getLevel()const{return m_level;}
  void setLevel(const LogLvl::Level &level);

  protected:
  LogLvl::Level m_level= LogLvl::DEBUG;
  bool m_hasFormatter = false;
  MutexType m_mutx;
  LogFormate::ptr m_formatter;
  
};



/**
 * @class Logger
 * @brief 
 *
 */
class Logger:public std::enable_shared_from_this<Logger>
{
  friend class LoggerMgr;

  public:
  typedef std::shared_ptr<Logger> ptr ;
  typedef Spinlock MutexType ;
  
  Logger(const std::string &name = "root");
  void log(const LogLvl::Level &level,LogEvent::ptr event);
  void debug(LogEvent::ptr event);
  void info(LogEvent::ptr event);
  void warn(LogEvent::ptr event);
  void error(LogEvent::ptr event);
  void fatal(LogEvent::ptr event);
  void addAppender(LogAppender::ptr addAppender);
  void delAppender(LogAppender::ptr addAppender );
  void clearAppender(LogAppender::ptr addAppender);
  LogLvl::Level getLevel()const {return m_level;};
  std::string getName()const{ return m_name;}
  std::string toYmalString(); 
private:
  std::string m_name;
  LogLvl::Level m_level;
  MutexType m_mutx;
  std::list<LogAppender::ptr> m_appenders;
  LogFormate::ptr m_formatter;
  Logger::ptr m_root;


};

//
/**
 * @class StdOutAppender
 * @brief 控制台日志输出
 *
 */
class StdOutAppender :public LogAppender
{
  public:
   virtual ~StdOutAppender() override;
  typedef std::shared_ptr<StdOutAppender> ptr ;
  void log(Logger::ptr logger,const LogLvl::Level &level,LogEvent::ptr event)override;
  std::string toYmalString()override;
};


/**
 * @class FileLogappender
 * @brief 日志文件落盘
 *
 */
class FileLogappender:public LogAppender{
  public:
  virtual ~FileLogappender()override;
  typedef std::shared_ptr<FileLogappender> ptr ;
  FileLogappender(const std::string & filename);
  void log(Logger::ptr logger,const LogLvl::Level & level,LogEvent::ptr event)override;
  std::string toYmalString()override;
  void reopen();
  private:
  std::string m_filename;
  std::ofstream m_filestream;
  u64 m_lastTime = 0;

};

//日志管理
class LoggerMgr{
  public:
  typedef Spinlock MutexType ;
  LoggerMgr();
  Logger::ptr getLoger(const std::string & name);
  void init();
  inline Logger::ptr getRoot(){return m_root;}
  std::string toYmalString();
  private:
  MutexType m_mutx;
  std::map<std::string ,Logger::ptr > m_logger;
  Logger::ptr m_root;
};

typedef  Singleton<LoggerMgr> LogMgr ;

//
#endif //__LOG_H_
//
