
# 添加可执行文件
add_executable(test_format ./test_format.cpp)

# 链接 sylar 库
target_link_libraries(test_format PRIVATE sylar)

# 如果遇到链接问题，可以尝试使用 whole-archive 选项
#target_link_libraries(test_format PRIVATE -Wl,--whole-archive sylar -Wl,--no-whole-archive)
