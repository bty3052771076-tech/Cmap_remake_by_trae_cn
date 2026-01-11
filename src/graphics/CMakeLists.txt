# 图形渲染模块 CMake 配置文件
# 包含图形节点、连接线、场景和视图的图形渲染类

cmake_minimum_required(VERSION 3.16)

# 创建图形渲染模块库
add_library(ConceptMapGraphics STATIC
    graphicsnode.cpp
    graphicsedge.cpp
    graphicsscene.cpp
    graphicsview.cpp
)

# 设置包含目录
target_include_directories(ConceptMapGraphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
)

# 链接 Qt 库
target_link_libraries(ConceptMapGraphics PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# 链接核心模块
target_link_libraries(ConceptMapGraphics PUBLIC
    ConceptMapCore
)

# 设置编译选项
target_compile_features(ConceptMapGraphics PUBLIC cxx_std_17)
