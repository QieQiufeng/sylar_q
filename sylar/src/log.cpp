#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
#include "../include/log.h"
#include "../include/util.h"




const char* LogLvl::toString(LogLvl::Level level)
{
  switch (level) {
#define XX(name)\
    case LogLvl::name:\
      return #name;\
      break;
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
    #undef XX
    default:
      return "UNKNOW";
      break;
  } 
  return "UNKNOW";
}

LogLvl::Level LogLvl::fromString(const std::string & str)
{
#define XX(level,name)\
if(str==#name){\
  return LogLvl::level;\
}
  XX(DEBUG,debug);
  XX(INFO,info);
  XX(WARN,warn);
  XX(ERROR,error);
  XX(FATAL,fatal);

  XX(DEBUG,DEBUG);
  XX(INFO,INFO);
  XX(WARN,WARN);
  XX(ERROR,ERROR);
  XX(FATAL,FATAL);
  return LogLvl::UNKNOW;
  #undef  XX
}

void MessageFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os << event->getContent();
}
void LevelFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os<<LogLvl::toString(level);
}
void ElapseFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os<<event->getElapse();
}
void NameFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os<<event->getLoger()->getName();
}
void ThreadIdFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os<<event->getThreadId();
}
void NewLineFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{ 
  os<<std::endl;
}
DateTimeFormateItem::DateTimeFormateItem(const std::string& fmt) 
:m_format(fmt)
{
  if(m_format.empty()){
    m_format = "%Y-%m-%d  %H-%M-%S";
  }

}
void DateTimeFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  struct tm tm;
  time_t time = event->getTime();
  localtime_r(&time, &tm);
  char buf[64];
  strftime(buf, sizeof(buf), m_format.c_str(), &tm);
  os << buf;
  
}
void FileNameFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os << event->getFile();
}
void LineFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os <<event->getLine();
}
void TabFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os << "\t";

}
void FiberIdFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os << event->getFiberId();
}
ThreadNameFormateItem::ThreadNameFormateItem(const std::string& str)
{
}
void ThreadNameFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os << event->getThreadName();
}
void StringFormateItem::formate(std::ostream & os,LogLvl::Level level,LogEvent::ptr event)
{
  os << m_str;
}

MessageFormateItem::MessageFormateItem(const std::string& str)
{
}
MessageFormateItem::~MessageFormateItem()
{
}
LevelFormateItem::LevelFormateItem(const std::string& str)
{
}
LevelFormateItem::~LevelFormateItem()
{
}
ElapseFormateItem::ElapseFormateItem(const std::string& str)
{
}
ElapseFormateItem::~ElapseFormateItem()
{
}
NameFormateItem::NameFormateItem(const std::string& str)
{
}
NameFormateItem::~NameFormateItem()
{
}
ThreadIdFormateItem::ThreadIdFormateItem(const std::string& str)
{
}
ThreadIdFormateItem::~ThreadIdFormateItem()
{
}
NewLineFormateItem::NewLineFormateItem(const std::string& str)
{
}
NewLineFormateItem::~NewLineFormateItem()
{
}
DateTimeFormateItem::~DateTimeFormateItem()
{
}
FileNameFormateItem::FileNameFormateItem(const std::string& str)
{
}
FileNameFormateItem::~FileNameFormateItem()
{
}
LineFormateItem::LineFormateItem(const std::string& str)
{
}
LineFormateItem::~LineFormateItem()
{
}
TabFormateItem::TabFormateItem(const std::string& str)
{
}
TabFormateItem::~TabFormateItem()
{
}
FiberIdFormateItem::FiberIdFormateItem(const std::string& str)
{
}
FiberIdFormateItem::~FiberIdFormateItem()
{
}
ThreadNameFormateItem::~ThreadNameFormateItem()
{
}
StringFormateItem::~StringFormateItem()
{
}

/**
 * @brief 日志事件构造初始化 
 *
 * @param logger 日志器
 * @param level 日志级别 
 * @param filename 当前函数所在文件名 
 * @param line  日志记录所在行号 
 * @param elapse 程序运行事件 
 * @param threadId 程序id 
 * @param fiberId 协程id 
 * @param time 当前日期  
 * @param threadName 进程名 
 */
LogEvent::LogEvent(std::shared_ptr<Logger> logger,LogLvl::Level level, const char* filename, 
         const u16& line,const u16& elapse,const u16& threadId,const u16& fiberId,
         const u64& time, const std::string& threadName)
  :m_file(filename)
  ,m_line(line)
  ,m_elapse(elapse)
  ,m_threadId(threadId)
  ,m_fiberId(fiberId)
  ,m_time(time)
  ,m_threadName(threadName)
  ,m_loggerptr(logger)
  ,m_level(level)

{
}

/**
 * @brief 获取文件名
 *
 * @return 
 */
inline const char* LogEvent::getFile() const
{
  return  m_file;  
}
inline u16 LogEvent::getLine() const
{
  return m_line;
}
inline u16 LogEvent::getElapse() const
{
  return  m_elapse;
}
inline u16 LogEvent::getThreadId() const
{
  return m_threadId;
}
inline u16 LogEvent::getFiberId() const
{
  return m_fiberId;
}
inline u64 LogEvent::getTime() const
{
  return m_time;
}
inline std::string LogEvent::getThreadName() const
{
  return m_threadName;
}
inline std::string LogEvent::getContent() const
{
  return m_ss.str();
}
inline std::stringstream &LogEvent::getSS() 
{
  return m_ss;
}
inline LogLvl::Level LogEvent::getLevel() const
{
  return  m_level;
}
inline std::shared_ptr<Logger> LogEvent::getLoger() const
{
  return m_loggerptr ;
}
/**
 * @brief 
 *
 * @param fmt 格式化字符串
 * @param ... 可变参列表   
 * @param  "%d",3; fmt 就是 "%d"; 可变参就是3； 
 */
void LogEvent::format(const char* fmt,...)
{
  va_list al;
  va_start( al, fmt);//初始化 al为 fmt后的第一个可变参（指针）
  format(fmt,al);
  va_end(al);
}

void LogEvent::format(const char* fmt,va_list va)
{
  char* buf;
  int len=vasprintf(&buf, fmt, va);//vasprintf 返回字符串的长度；
  if(len!=-1)
  {
    m_ss<<std::string(buf,len);//
    free(buf);
  }
 
}
  
LogEventWrap::LogEventWrap(LogEvent::ptr ptr)
:m_event(ptr)
{
}
LogEventWrap::~LogEventWrap()
{
  m_event->getLoger()->log(m_event->getLevel(), m_event);
}
std::stringstream& LogEventWrap::getSS() 
{
  return m_event->getSS() ;
}
void LogAppender::setFormatter(LogFormate::ptr val)
{
  m_formatter = val;
}
LogFormate::ptr LogAppender::getFormatter() const
{

 return m_formatter; 
}


/**
 * @brief 日志格式化构造
 *
 * @param pattren 
 */
LogFormate::LogFormate(const std::string &pattren)
:m_pattern(pattren)
{
  init();
}
std::string LogFormate::format(LogLvl::Level level,LogEvent::ptr event)
{
  std::stringstream ss;
 for(auto it:m_items){
    it->formate(ss,level,event);
  }
  return  ss.str();
}
std::ostream& LogFormate::format(std::ostream & ofs,LogLvl::Level level,LogEvent::ptr event)
{
  for(auto itr:m_items){
    itr->formate(ofs,  level, event);
  }
  return  ofs;
}


/**
 * @brief 日志格式化初始化
 */
//%xxx %xxx{xxx} %%
//"%Y-%m-%d %H:%M:%S"
void LogFormate::init()
{
    //str, format, type
   //- 第一个元素 `std::string` 是解析得到的字符串或者标识符。
   //- 第二个元素 `std::string` 是可能的格式化字符串。
   //- 第三个元素 `int` 表示解析状态，`0` 表示普通字符串，`1` 表示需要特殊处理的标识符。
  std::vector<std::tuple<std::string,std::string,int>> vecLogFormat;

  std::string strTmpMsg;//格式化初步过滤字符串
  //初步过滤 % 转义字符
  for(size_t  i =0;i<m_pattern.size();i++){
    if(m_pattern[i] != '%'){
      strTmpMsg.append(1,m_pattern[i]);
      continue;
    }  
    //如果下一个还是% 就写入一个%
    if(m_pattern[i+1] == '%'){
      strTmpMsg.append(1,m_pattern[i]);
      continue;
    }

    //如果存在{ } 则解析{ } 对应内容

    bool bIsFormatWord = false ;//1 解析字符
    std::string strFmt; 
    std::string strNormal;
    size_t n = i+1;
    size_t format_begin = 0;
    while(n < m_pattern.size()){
      //解析i对应的下一个字段
      //非字母 、{ 、} 等 属于普通字符串 比如  --  空格等
      if(!bIsFormatWord && !isalpha(m_pattern[n]) && 
        m_pattern[n]!= '{'&& m_pattern[n]!= '}'){

        strNormal = m_pattern.substr(i+1,n-i-1);
        break;
      }

      //解析 { } 内容
      //
      if(!bIsFormatWord && m_pattern[n]== '{'){

        strNormal = m_pattern.substr(i+1,n-i-1);
        bIsFormatWord = true;
        format_begin = n;
        ++n;
        continue;
      } 

      if(bIsFormatWord && m_pattern[n] == '}' ){

        strFmt = m_pattern.substr(format_begin+1,n-format_begin-1);
        bIsFormatWord = false;
        ++n;
        break;
      }
      ++n;
      if(n == m_pattern.size()){
        if(strNormal.empty()){
          strNormal = m_pattern.substr(i+1);
        }
      }

    }


    if(!bIsFormatWord){
      if(!strTmpMsg.empty()){
        vecLogFormat.emplace_back(std::make_tuple(strTmpMsg,std::string(),0));
        strTmpMsg.clear();
      }
      vecLogFormat.emplace_back(std::make_tuple(strNormal,strFmt,1));
      i = n-1;
    }
    else if(bIsFormatWord){

      std::cerr<<"Formate err!\n";
      vecLogFormat.emplace_back(std::make_tuple("pattren err",strFmt,0));
    }
  }  

  if(!strTmpMsg.empty()){
    vecLogFormat.emplace_back(std::make_tuple(strTmpMsg," ",0));

  }
  
  static std::map<std::string,std::function<FormateItem::ptr(const std::string & str)>> s_formate_item ={
  #define XX(str,C)\
        {#str, [](const std::string& fmt) { return FormateItem::ptr(new C(fmt));}}

      XX(m,MessageFormateItem),
      XX(p,LevelFormateItem),
      XX(r,ElapseFormateItem),
      XX(c,NameFormateItem),
      XX(t,ThreadIdFormateItem),
      XX(n,NewLineFormateItem),
      XX(d,DateTimeFormateItem),
      XX(f,FileNameFormateItem),
      XX(l,LineFormateItem),
      XX(T,TabFormateItem),
      XX(F,FileNameFormateItem),
      XX(N,ThreadNameFormateItem),
    #undef XX
  };

  for(auto &i : vecLogFormat){
    if(std::get<2>(i) == 0){
      m_items.emplace_back(FormateItem::ptr(new StringFormateItem(std::get<0>(i))));
    }else{
      auto iter = s_formate_item.find(std::get<0>(i));
      if(iter == s_formate_item.end()){
        m_items.emplace_back(FormateItem::ptr(new StringFormateItem("<<errformate %"+ std::get<0>(i)+ "<<")));

      }else{
       m_items.emplace_back(iter->second(std::get<1>(i))); 

      }
    }
  }
}




void LogAppender::setLevel(const LogLvl::Level &level)
{
  m_level = level;
}

Logger::Logger(const std::string &name)
:m_name(name),
m_level(LogLvl::DEBUG){
  m_formatter.reset(new LogFormate("%d{%Y-%m-%d  %H-%M-%S}%T%t%T%N%T%F%T[%p]%T%f:%l%T%m%n"));
}

void Logger::log(const LogLvl::Level &level,LogEvent::ptr event)
{
 if(level >= m_level){
    auto self = shared_from_this();
    MutexType::Lock lock(m_mutx);
    if(m_appenders.empty()){
      for(auto &i :m_appenders){
        i->log(self, level, event);
      }
    }else if (m_root) {
        m_root->log(level, event);
      }
    }
  }
void Logger::debug(LogEvent::ptr event)
{
  log(LogLvl::DEBUG,event);
}
void Logger::info(LogEvent::ptr event)
{
  log(LogLvl::INFO,event);
}
void Logger::warn(LogEvent::ptr event)
{
  log(LogLvl::WARN,event);
}
void Logger::error(LogEvent::ptr event)
{
  log(LogLvl::ERROR,event);
}
void Logger::fatal(LogEvent::ptr event)
{
  log(LogLvl::FATAL,event);
}
/**
 * @brief  添加输出目的地
 *@brief   同一个logger 可能在跨线程使用 需要加锁添加
 *@brief   同一个addAppender 智能指针 也可能存在跨线程使用，需要加锁
 * @param addAppender 
 */
void Logger::addAppender(LogAppender::ptr addAppender)
{
  MutexType::Lock lock(m_mutx);//该锁用于Logger 写入addAppender 
  if(!addAppender->getFormatter()){
    MutexType::Lock ll(addAppender->m_mutx);//该锁用于LogAppender 写入 m_formatter
    addAppender->m_formatter=m_formatter;
  }
  m_appenders.push_back(addAppender);
}
void Logger::delAppender(LogAppender::ptr addAppender )
{
  MutexType::Lock lock(m_mutx);
  for(auto iter = m_appenders.begin();iter!=m_appenders.end();++iter){
    if(*iter == addAppender ){
      m_appenders.erase(iter);
      return;
    }

  }
}
void Logger::clearAppender(LogAppender::ptr addAppender)
{
  MutexType::Lock lock(m_mutx);
  m_appenders.clear();

}
std::string Logger::toYmalString()
{
  MutexType::Lock ll(m_mutx);
  YAML::Node node;
  node["name"]= m_name;
  if(m_level!= LogLvl::UNKNOW){
    node["level"]=LogLvl::toString(m_level);

  }
  if(m_formatter){
    node["formatter"]= m_formatter->getPattern();
  }
  for(auto &i:m_appenders){
    node["appenders"].push_back(YAML::Load(i->toYmalString()));
  }
  std::stringstream ss;
  ss<<node;
  return ss.str(); 
}
void StdOutAppender::log(Logger::ptr logger,const LogLvl::Level &level,LogEvent::ptr event)
{
  if(level >= m_level){
  MutexType::Lock lock(m_mutx);
    m_formatter->format(level,event);
  }
}
std::string StdOutAppender::toYmalString()
{
  MutexType::Lock ll(m_mutx);
  YAML::Node node;
  node["type"]= "StdOutAppender";
  if(m_level!= LogLvl::UNKNOW){
    node["level"]=LogLvl::toString(m_level);

  }
  if(m_hasFormatter&&m_formatter){
    node["formatter"]= m_formatter->getPattern();
  }
  std::stringstream ss;
  ss<<node;
  return  ss.str();
}

StdOutAppender::~StdOutAppender(){}

FileLogappender::~FileLogappender(){}
FileLogappender::FileLogappender(const std::string & filename)
:m_filename(filename){
  reopen();
}
void FileLogappender::log(Logger::ptr logger,const LogLvl::Level & level,LogEvent::ptr event)
{
  if(m_level >= level){
    u64 dwNowTime = event->getTime();
    if(dwNowTime >= m_lastTime+3){
      reopen();
      m_lastTime = dwNowTime;
    }
    MutexType::Lock mutx(m_mutx);
    if(!m_formatter->format(m_filestream,level,event)){
      std::cout<<"[FileLogappender]log init format error!\n";
    }
  }

}

/**
 * @brief 
 *
 * @return 
 */
std::string FileLogappender::toYmalString()
{
  MutexType::Lock mutx(m_mutx);
  YAML::Node node;
  node["type"] = "FileLogappender";
  node["file"]= "filename";

  if(m_level != LogLvl::UNKNOW){
    node["level"] = LogLvl::toString(m_level);
  }
 
  if(m_formatter&&m_hasFormatter){
    node["formatter"] = m_formatter->getPattern();
  }
 
  std::stringstream ss;
  ss<<node;
  return ss.str();
}
/**
 * @brief 
 */
void FileLogappender::reopen()
{
  MutexType::Lock mutx(m_mutx);
  //先看看有没有打开
  if(m_filestream){
    m_filestream.close();
  }
  FSUtil::OpenForWrite(m_filestream,m_filename,std::ios_base::app);//追加模式写入日志
}



LoggerMgr::LoggerMgr()
{

}
Logger::ptr LoggerMgr::getLoger(const std::string & name)
{
  for(auto &i:m_logger){
    if(name == i.first){
      return i.second;
    }
  }
  
  return Logger::ptr();
}
void LoggerMgr::init()
{
}
std::string LoggerMgr::toYmalString()
{
  return std::string();
}

