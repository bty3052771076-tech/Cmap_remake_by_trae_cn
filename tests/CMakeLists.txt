# 测试模块 CMake 配置文件

cmake_minimum_required(VERSION 3.16)

# 启用测试
enable_testing()

# 查找 Qt Test 组件
find_package(Qt6 REQUIRED COMPONENTS Test)

# 添加测试可执行文件（如果需要）
# add_executable(ConceptMapTests test_main.cpp)
# target_link_libraries(ConceptMapTests PRIVATE
#     Qt6::Test
#     Qt6::Core
#     Qt6::Gui
#     Qt6::Widgets
#     ConceptMapCore
#     ConceptMapGraphics
#     ConceptMapCommands
#     ConceptMapModels
#     ConceptMapManagers
# )

# 添加测试
# add_test(NAME ConceptMapTests COMMAND ConceptMapTests)

message(STATUS "测试模块已配置")
