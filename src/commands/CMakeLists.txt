# 命令模式模块 CMake 配置文件
# 包含添加/删除节点和连接线的命令类

cmake_minimum_required(VERSION 3.16)

# 创建命令模式模块库
add_library(ConceptMapCommands STATIC
    addnodecommand.cpp
    addedgecommand.cpp
    deletenodecommand.cpp
    deleteedgecommand.cpp
    movenodecommand.cpp
)

# 设置包含目录
target_include_directories(ConceptMapCommands PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/graphics
)

# 链接 Qt 库
target_link_libraries(ConceptMapCommands PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# 链接核心模块和图形渲染模块
target_link_libraries(ConceptMapCommands PUBLIC
    ConceptMapCore
    ConceptMapGraphics
)

# 设置编译选项
target_compile_features(ConceptMapCommands PUBLIC cxx_std_17)
