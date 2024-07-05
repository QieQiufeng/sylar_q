cmake_minimum_required(VERSION 3.22)

# 添加include目录到头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 收集源文件
file(GLOB SylarSources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# 设置源文件列表
#set(SYLAR_SRC
#   src/log.cpp
#   src/util.cpp
#)

# 生成静态库
add_library(sylar STATIC ${SylarSources})

add_executable(test_format ./src/test_format.cpp)
# 设置编译选项
target_compile_options(sylar PRIVATE -Wall)

# 如果有些实现是直接在 cpp 文件中的，可以考虑使用 -fwhole-program 选项
target_compile_options(sylar PRIVATE -fwhole-program)

# 安装头文件
install(DIRECTORY include/ DESTINATION include)
install(TARGETS sylar DESTINATION lib)

# 如果有些符号需要导出，可以使用 -fvisibility=default
set_target_properties(sylar PROPERTIES CXX_VISIBILITY_PRESET default)
