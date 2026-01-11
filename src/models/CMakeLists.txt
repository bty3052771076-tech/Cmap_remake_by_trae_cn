# 数据模型模块 CMake 配置文件
# 包含节点模型、连接线模型和概念图模型

cmake_minimum_required(VERSION 3.16)

# 创建数据模型模块库
add_library(ConceptMapModels STATIC
    nodemodel.cpp
    edgemodel.cpp
    mapmodel.cpp
)

# 设置包含目录
target_include_directories(ConceptMapModels PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
)

# 链接 Qt 库
target_link_libraries(ConceptMapModels PUBLIC
    Qt6::Core
    Qt6::Gui
)

# 链接核心模块
target_link_libraries(ConceptMapModels PUBLIC
    ConceptMapCore
)

# 设置编译选项
target_compile_features(ConceptMapModels PUBLIC cxx_std_17)
