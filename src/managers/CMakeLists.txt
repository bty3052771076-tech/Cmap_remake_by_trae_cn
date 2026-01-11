# 管理器模块 CMake 配置文件
# 包含样式管理器和文件管理器

cmake_minimum_required(VERSION 3.16)

# 创建管理器模块库
add_library(ConceptMapManagers STATIC
    stylemanager.cpp
    filemanager.cpp
)

# 设置包含目录
target_include_directories(ConceptMapManagers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
)

# 链接 Qt 库
target_link_libraries(ConceptMapManagers PUBLIC
    Qt6::Core
    Qt6::Gui
)

# 链接核心模块
target_link_libraries(ConceptMapManagers PUBLIC
    ConceptMapCore
)

# 设置编译选项
target_compile_features(ConceptMapManagers PUBLIC cxx_std_17)
