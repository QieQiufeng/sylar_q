cmake_minimum_required(VERSION 3.22)
project(sylar_learn)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置默认的构建类型为Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# 添加include目录到头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sylar/include)

# 查找YAML-CPP库
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/yaml-cpp")
find_package(yaml-cpp REQUIRED)

# 收集sylar源文件
file(GLOB SYLAR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sylar/src/*.cpp)

# 生成静态库
add_library(sylar STATIC ${SYLAR_SOURCES})

# 链接YAML-CPP库到sylar库
target_link_libraries(sylar PRIVATE yaml-cpp)

# 添加测试可执行文件
add_executable(test_format ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_format.cpp)

# 链接sylar库到测试可执行文件
target_link_libraries(test_format PRIVATE sylar)

# 设置编译选项
target_compile_options(sylar PRIVATE -Wall)

# 指定生成的库和可执行文件的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 安装头文件和静态库
install(DIRECTORY sylar/include/ DESTINATION include)
install(TARGETS sylar DESTINATION lib)

