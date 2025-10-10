cmake_minimum_required(VERSION 3.10)
project(SFMLDemo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

option(ENABLE_DEBUG_INFO "Enable debug information (-g)" ON)

# 如果启用了调试信息开关，则添加 -g 编译选项
if(ENABLE_DEBUG_INFO)
    add_compile_options(-g)
endif()

# 查找 SFML 库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 创建可执行文件，包含所有源文件
add_executable(sfml-demo 
    main.cpp
    window.cpp  # 假设这是 GameWindow 类的实现文件
    snake_game.cpp
)

# 链接 SFML 库
target_link_libraries(sfml-demo 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# 如果 window.h 和 window.cpp 不在同一个目录，需要包含相应目录
# target_include_directories(sfml-demo PRIVATE /path/to/window/header)