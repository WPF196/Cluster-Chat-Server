cmake_minimum_required(VERSION 3.0)
project(main)

# 配置编译选项
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
# 配置需要编译的源文件列表
set(SRC_LIST test.cpp)
#设置可执行路径的存储路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


# 生成可执行文件server
add_executable(server ${SRC_LIST})

# 表示server需要连接的库
target_link_libraries(server muduo_net muduo_base pthread)