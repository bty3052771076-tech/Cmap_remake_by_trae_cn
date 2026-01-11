# ConceptMap 项目 - 主 CMake 配置文件
# 项目名称：ConceptMap
# Qt 版本：6.10.1
# 构建系统：CMake
# C++ 标准：C++17

cmake_minimum_required(VERSION 3.16)

# 设置项目名称和版本
project(ConceptMap VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置 Qt 6.10.1 路径
set(CMAKE_PREFIX_PATH "E:/Qt/6.10.1/mingw_64")

# 自动检测 Qt 组件
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
    Svg
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 启用 Qt 的自动 MOC、UIC 和 RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 添加源代码子目录
add_subdirectory(src/core)
add_subdirectory(src/graphics)
add_subdirectory(src/commands)
add_subdirectory(src/models)
add_subdirectory(src/managers)
add_subdirectory(src/ui)

# 创建可执行文件
add_executable(${PROJECT_NAME}
    src/ui/main.cpp
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Svg
)

# 链接所有子模块
target_link_libraries(${PROJECT_NAME} PRIVATE
    ConceptMapCore
    ConceptMapGraphics
    ConceptMapCommands
    ConceptMapModels
    ConceptMapManagers
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 添加资源文件
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    qt_add_resources(${PROJECT_NAME} "resources"
        ${CMAKE_SOURCE_DIR}/resources/icons/icons.qrc
    )
endif()

# 设置 Windows 特定属性
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 打印配置信息
message(STATUS "项目名称: ${PROJECT_NAME}")
message(STATUS "项目版本: ${PROJECT_VERSION}")
message(STATUS "C++ 标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt 版本: ${Qt6_VERSION}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
