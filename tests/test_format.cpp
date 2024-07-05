#include"../sylar/include/util.h"
#include"../sylar/include/log.h"
#include <pthread.h>

int main() {
  Logger::ptr logger(new Logger);
  logger->addAppender(LogAppender::ptr(new StdOutAppender));
  FileLogappender::ptr fileappender(new FileLogappender("./test.log"));
  LogFormate::ptr fmt(new LogFormate("%d%T%p%T%m%n"));
  LogEvent::ptr event(new LogEvent(logger,LogLvl::INFO,__FILE__,__LINE__,12,13,3,time(0),"hello"));


    return 0;
}
