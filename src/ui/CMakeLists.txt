# 用户界面模块 CMake 配置文件
# 包含主窗口、工具栏、属性面板和主函数

cmake_minimum_required(VERSION 3.16)

# 创建用户界面模块库
add_library(ConceptMapUI STATIC
    mainwindow.cpp
    toolbar.cpp
    propertypanel.cpp
)

# 设置包含目录
target_include_directories(ConceptMapUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/graphics
    ${CMAKE_SOURCE_DIR}/src/commands
    ${CMAKE_SOURCE_DIR}/src/models
    ${CMAKE_SOURCE_DIR}/src/managers
)

# 链接 Qt 库
target_link_libraries(ConceptMapUI PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
)

# 链接所有子模块
target_link_libraries(ConceptMapUI PUBLIC
    ConceptMapCore
    ConceptMapGraphics
    ConceptMapCommands
    ConceptMapModels
    ConceptMapManagers
)

# 设置编译选项
target_compile_features(ConceptMapUI PUBLIC cxx_std_17)
