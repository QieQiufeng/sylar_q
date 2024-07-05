/******************************************************************

常用工具封装：



********************************************************************/


#ifndef _UTIL_H_
#define _UTIL_H_
//

#include <fstream>
#include <string>
#include <vector>
#include<thread>


std::thread::id GetThreadIdFunc(std::thread &t);
std::string GetThreadNameFunc(pthread_t thread = pthread_self());
 


/**********************************************************************
 *
 *
 *                      文件操作类
 *
 *
 **********************************************************************/
class FSUtil{
  public:
  /**
   * @brief 获取指定目录下的所有文件列表
   *
   * @param filename 对应目录下的所有文件
   * @param path  文件目录
   * @param subfix  文件后缀
   */
  static void ListAllFile(std::vector<std::string>& filename,
                          const std::string & path,
                          const std::string & subfix);

  /**
   * @brief 
   *
   * @param pidfile 
   */
  static bool IsRunningPidFile(const std::string & pidfile);
  /**
   * @brief 创建文件夹
   *
   * @param dirname  文件夹名称
   */
  static bool Mkdir(const std::string &dirname);
  /**
   * @brief  删除文件
   *
   * @param path  绝对路径
   */
  static bool Rm(const std::string & path);
  /**
   * @brief 移动文件
   *
   * @param fromPath  原来文件路径
   * @param toPath   要移动文件路径
   */
  static bool Mv(const std::string& fromPath,const std::string& toPath);
  /**
   * @brief 
   *
   * @param path  文件名
   * @param rpath 传入传出 真实地址 
   */
  static bool RealPath(const std::string& path,std::string& rpath);
  /**
   * @brief 软连接
   *
   * @param frmPath 
   * @param toPath 
   */
  static bool SymLink(const std::string& frmPath,const std::string &toPath);
  /**
   * @brief 移除软连接
   *
   * @param filename 
   * @param exit 
   */
  static bool UnLink(const std::string &filename,const bool& exit = false);
  /**
   * @brief 
   *
   * @param filename 
   */
  static std::string DirName(const std::string&filename);
  static std::string BaseName(const std::string &filename);
  /**
   * @brief 以读的方式打开
   *
   * @param ifs 
   * @param fileName 
   * @param mode  
   * @return 
   */
  static bool OpenForRead(std::ifstream & ifs,const std::string & fileName,
                         const  std::ios_base::openmode &mode);
  /**
   * @brief 以写的方式打开
   *
   * @param ofs 
   * @param FileName 
   * @param mode 
   * @return 
   */
  static bool OpenForWrite(std::ofstream& ofs,const std::string& FileName,
                           const std::ios_base::openmode& mode);
};

#endif // !_UTIL_H_
#define _UTIL_H_



