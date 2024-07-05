#include"../include/util.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include<error.h>
#include<thread>

std::thread::id GetThreadIdFunc(std::thread &t){

  return t.get_id() ;

}

std::string GetThreadNameFunc(pthread_t thread) {
  char thread_name[16]={0};
  int ret = pthread_getname_np(thread,thread_name,sizeof(thread_name));
  if(ret !=0){
    return "";
  } 
  return std::string(thread_name);
}

/**
 * @brief 判断当前路径是否存在 
 *
 * @param file 
 * @param st 
 * @return 
 */
static int __lstat(const char * file,struct stat *st = nullptr){
  struct stat lst;
  int ret = lstat(file,&lst);
  if(st){
    *st = lst;
  }
  return ret;
}

/**
 * @brief 根据传入文件夹地址 判断当前文件是否存在
 *
 * @param pathptr 
 * @return bool
 */
static bool __isExist(const char* pathptr){
  struct stat st;
  if(stat(pathptr,&st)==0&&S_ISDIR(st.st_mode)){
    return true;
  }
  return false; 
}

/**
 * @brief  创建文件夹
 *
 * @param dirname
 * @return int 
 */
static int __mkdir(const char* dirname){
  //存在相同文件名 mk 失败
  if(0 == access(dirname, F_OK)){
  return 0;
  }
//用户读写执行.组 读写执行 ，其他 读 执行
  return mkdir(dirname,S_IRWXG|S_IRWXU|S_IROTH|S_IXOTH);
}

static int __rmdir(const char* dirname){
  if(0 == access(dirname,F_OK)){
    return rmdir(dirname);
  }
  return false;
}

void FSUtil::ListAllFile(std::vector<std::string>& filename,
                          const std::string & path,
                         const std::string & subfix)
{
  // access 函数说明:
  // int access(const char *filenpath, int mode);
  // mode 的值和含义如下所示：
  // 00——只检查文件是否存在
  // 02——写权限
  // 04——读权限
  // 06——读写权限
  // 

  if(0 != access(path.c_str(),F_OK)){
    return;
  }
  DIR * dir = opendir(path.c_str());
  if(nullptr == dir){
  return;
  }
  struct dirent*dp = nullptr;
  while((dp = readdir(dir)) != nullptr){
    if(DT_DIR == dp->d_type ){
      //查找非节点目录
      if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
        continue;
      }
      //递归查找目录下子目录
      //当前查找目录为path dp.d_name 也是个文件夹 那么该文件夹目录 path  
      //path + "/"+dp.d_name
      ListAllFile(filename, path+"/"+dp->d_name, subfix);
    }else if(DT_REG == dp->d_type){
      std::string tmpfilename(dp->d_name);
    if(subfix.empty()){
        filename.emplace_back(path+"/"+tmpfilename);
      }else {
        if(tmpfilename.size()<subfix.size()){
          continue;
        }else if(tmpfilename.substr(tmpfilename.length()-subfix.size()) == subfix){
          filename.emplace_back(path+"/"+tmpfilename);
        }
      }
    }
  }


}
bool FSUtil::IsRunningPidFile(const std::string & pidfile)
{
  if(__lstat(pidfile.c_str()) != 0){
    return false;
  }
  //读取pidfile 每行记录一条pid信息
  std::ifstream ifs(pidfile);
  std::string line;
  if(!ifs || !std::getline(ifs,line)){
    return false;
  }
  if(line.empty()){
    return false;
  }
  
  //获取后，检验pid是否正常
  pid_t pid = atoi(line.c_str());
  if(pid <= 1){
    return false;
  }
  //pid正常 检查进程是否存在  kill pid -0 操作
  if(kill(pid,0) != 0){
    return  false;
  }

  return true;
}

bool FSUtil::Mkdir(const std::string &dirname)
{
  //已有同名文件
  if(__isExist(dirname.c_str())){
    return true;
  }
  char *path = strdup(dirname.c_str());
  char* ptrpath = strchr(path+1,'/');
  do{
    for(;ptrpath;*ptrpath = '/',ptrpath=strchr(ptrpath+1,'/')){
      *ptrpath = '\n' ;
      if(__mkdir(path)!= 0){
        if(__isExist(path) )
        {
          *ptrpath = '/';
          continue;
        }
        free(path);
        return false;
      }
      *ptrpath = '/';
    }
    if(ptrpath!= nullptr){
      break;
    }else if(__mkdir(path)!=0){
      if(__isExist(path)){

        free(path);
        return true;
      } 

      free(path);
      return false;
    }

    free(path);
    return true;
  }while(0);
// path 为空
  free(path);
  return false;
}

bool FSUtil::UnLink(const std::string &filename,const bool& exit)
{
 if(!exit && !__isExist(filename.c_str())){
    return  true;
  }
  return unlink(filename.c_str()) == 0;
}

bool FSUtil::Rm(const std::string & path)
{
  
  bool bret =false;
  if(!__isExist(path.c_str())){
    return bret;
  }

  struct stat st;
  if(0 ==  __lstat(path.c_str(),&st)&& S_ISLNK(st.st_mode)){
    return UnLink(path.c_str());
    
  }

  DIR *Dir_ptr = opendir(path.c_str());
  if(Dir_ptr == nullptr)
  {
    std::cerr<<"[RM] ERROR Dir_Ptr Is nullptr:<"<<path.c_str()<<">\n";
    return  bret;
  }

  struct dirent *dir =nullptr;
  while((dir = readdir(Dir_ptr))!= nullptr){
    //. .. 直接跳过
    if(!strcmp(dir->d_name,".")||!strcmp(dir->d_name, "..")){
      continue;
    }

    //递归删除子文件
    std::string strSubPath = path+ "/"+dir->d_name;
    if(0 == stat(strSubPath.c_str(),&st)){
      if(S_ISDIR(st.st_mode)){
        if(!Rm(strSubPath)){
          bret = false;
          std::cerr<<"[Rm] strSubPath is Rm err! PATH:"<<strSubPath<<"\n";
          continue;
        }
      }
      else{

        if(0 != UnLink(strSubPath.c_str())){

          bret = false;
          std::cerr<<"[Rm] strSubPath is Unlink err! PATH:"<<strSubPath<<"\n";
          continue;
        }
      }
    }
    else{
      std::cerr<<"[Rm] strSubPath stat is ERR!\n";
      continue;
    }
  }
  closedir(Dir_ptr);
  if(0 != ::rmdir(path.c_str())){
    std::cerr<<"[Rm] Rm path:"<<path<<"itself err! please check path is right!\n";
    bret = false;
  }
  return  bret;
}

bool FSUtil::Mv(const std::string& fromPath,const std::string& toPath)
{
  if(!Rm(toPath)){
    std::cerr<<"[Mv] Mv FALSE,Det PATH:"<<toPath<<"Is Not Exit! Please Cheack it!\n";
    return false;
  }
  return rename(fromPath.c_str(), toPath.c_str()) == 0;
}

/**
 * @brief 获取绝对地址
 *
 * @param path 
 * @param rpath 
 * @return 
 */
bool FSUtil::RealPath(const std::string& path,std::string& rpath)
{
  if(!__isExist(path.c_str())){
    std::cerr<<"[RealPath] The Path Not Exit,Please Cheack It,And Maybe Sure It`s Rigtht!\n";
    return false;
  }
  char* ptr_rpath = ::realpath(path.c_str(),nullptr);
  if(ptr_rpath == nullptr){
    std::cerr<<"[RealPath] The Path Con`t Get RealPath,Please Cheack Cin Path!\n";
    return false;
  }

  std::string(ptr_rpath).swap(rpath);
  free(ptr_rpath);
  return true;
}

bool FSUtil::SymLink(const std::string& fromPath,const std::string &toPath)
{
  if(!Rm(toPath)){
    std::cerr<<"[SymLink] The Path Is Con`t rm!\n";
    return false;
  }
  return ::symlink(fromPath.c_str(), toPath.c_str()) == 0;
}

/**
 * @brief 获取文件路径名
 *
 * @param filename 
 */
std::string FSUtil::DirName(const std::string&filename)
{
  if(filename.empty()){
    std::cerr<<"[DirName] cin filename is empty!\n";
    return ".";
  }
  auto pos = filename.rfind("/");
  if(pos == 0){
    return "/";
  }else if(pos == std::string::npos){
    return filename;
  }else{
    return filename.substr(0,pos);
  }

}
/**
 * @brief 获取文件名
 *
 * @param filename 
 */
std::string FSUtil::BaseName(const std::string &filename)
{
  if(filename.empty()){
    std::cerr<<"[BaseName] FileName Is empty!\n";
    return ".";
  }
  auto pos = filename.rfind("/");
  if(pos == std::string::npos){
    return filename;
  }
  else {
    return filename.substr(pos+1);
  }
}

/**
 * @brief 只读式打开
 *
 * @param ifs 
 * @param fileName 
 * @param mode 
 * @return 
 */
bool FSUtil::OpenForRead(std::ifstream & ifs,const std::string & fileName,
                         const  std::ios_base::openmode &mode)
{
  ifs.open(fileName.c_str(),mode);
  return ifs.is_open();
}
/**
 * @brief 只写式打开
 *
 * @param ofs 
 * @param FileName 
 * @param mode 
 * @return 
 */
bool FSUtil::OpenForWrite(std::ofstream& ofs,const std::string& FileName,
                           const std::ios_base::openmode& mode)
{
  ofs.open(FileName.c_str(),mode);
  if(!ofs.is_open()){
    std::string strDir = DirName(FileName);
    Mkdir(strDir);
    ofs.open(FileName.c_str(),mode);
  }
  return ofs.is_open();

}
