# CMakeLists.txt

# 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(SFMLDemo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 SFML 组件
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 链接 SFML 库
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)