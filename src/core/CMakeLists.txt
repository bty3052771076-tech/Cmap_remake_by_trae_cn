# 核心模块 CMake 配置文件
# 包含概念节点、连接线和概念图的核心数据类

cmake_minimum_required(VERSION 3.16)

# 创建核心模块库
add_library(ConceptMapCore STATIC
    conceptnode.cpp
    conceptedge.cpp
    conceptmap.cpp
)

# 设置包含目录
target_include_directories(ConceptMapCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接 Qt 库
target_link_libraries(ConceptMapCore PUBLIC
    Qt6::Core
    Qt6::Gui
)

# 设置编译选项
target_compile_features(ConceptMapCore PUBLIC cxx_std_17)
