# ConceptMap - Qt 6.10.1 概念图软件

一个功能强大的概念图绘制软件，基于 Qt 6.10.1 和 qmake 构建系统开发。

## 项目特点

- 🎨 **现代化界面**: 基于 Qt 6.10.1 的现代化图形用户界面
- 📐 **图形渲染**: 使用 Qt Graphics View Framework 实现高性能图形渲染
- 🔄 **撤销/重做**: 完整的命令模式实现，支持撤销和重做操作
- 💾 **文件支持**: 支持 JSON 和 XML 格式的文件读写
- 🖼️ **导出功能**: 支持导出为图片、PDF 和 SVG 格式
- 🎭 **样式主题**: 提供多种预设样式主题，支持自定义样式
- ⌨️ **快捷键**: 丰富的快捷键支持，提高工作效率
- 🌳 **自动排版**: 支持树形布局，按照箭头指向自动排列节点

## 系统要求

- **操作系统**: Windows 11
- **Qt 版本**: 6.10.1 (安装路径: E:\Qt\6.10.1\mingw_64)
- **构建工具**: qmake
- **编译器**: MinGW 13.1.0 (安装路径: E:\Qt\Tools\mingw1310_64\bin)
- **C++ 标准**: C++17

## 快速开始

### 构建项目

```powershell
# 进入项目目录
cd d:\AI\trae\prj_2

# 创建构建目录
mkdir build
cd build

# 运行 qmake 配置项目
& "E:\Qt\6.10.1\mingw_64\bin\qmake.exe" ..\ConceptMap.pro

# 编译项目（使用完整路径）
& "E:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe"

# 运行程序（使用绝对路径）
& "d:\AI\trae\prj_2\build\release\ConceptMap.exe"
```

详细的构建说明请参考 [BUILD.md](BUILD.md)。

## 功能特性

### 核心功能

- ✅ 节点管理（添加、删除、编辑、移动）
- ✅ 连接线管理（添加、删除、编辑）
- ✅ 视图操作（缩放、平移、适应视图）
- ✅ 编辑操作（撤销、重做、删除、全选）
- ✅ 文件操作（新建、打开、保存、另存为）
- ✅ 导出功能（图片、PDF、SVG）
- ✅ 样式管理（预设样式、自定义样式）
- ✅ 属性面板（实时编辑节点和连接线属性）
- ✅ 最近文件管理
- ✅ 自动排版（树形布局，按照箭头指向自动排列节点）

### 快捷键

| 操作 | 快捷键 |
|------|--------|
| 新建 | Ctrl+N |
| 打开 | Ctrl+O |
| 保存 | Ctrl+S |
| 另存为 | Ctrl+Shift+S |
| 撤销 | Ctrl+Z |
| 重做 | Ctrl+Y |
| 删除 | Delete / Backspace |
| 全选 | Ctrl+A |
| 放大 | Ctrl++ |
| 缩小 | Ctrl+- |
| 重置缩放 | Ctrl+0 |
| 适应视图 | Ctrl+F |
| 自动排版 | Ctrl+L |

## 项目结构

```
ConceptMap/
├── ConceptMap.pro              # 主 qmake 配置文件
├── BUILD.md                     # 构建说明文件
├── README.md                    # 项目说明文件
├── .gitignore                   # Git 忽略文件
├── src/
│   ├── core/                   # 核心模块
│   ├── graphics/               # 图形渲染模块
│   ├── commands/               # 命令模式模块
│   ├── models/                 # 数据模型模块
│   ├── managers/               # 管理器模块
│   └── ui/                     # 用户界面模块
├── resources/                  # 资源文件
│   ├── styles/                 # 样式文件
│   └── icons/                  # 图标文件
└── docs/                       # 文档目录
```

## 技术栈

- **编程语言**: C++17
- **框架**: Qt 6.10.1
- **构建工具**: qmake
- **编译器**: MinGW 13.1.0
- **图形框架**: Qt Graphics View Framework
- **设计模式**: 命令模式、模型-视图模式

## 模块说明

### 核心模块 (core)

提供概念图的数据模型，包括：

- `ConceptNode`: 概念节点数据类
- `ConceptEdge`: 连接线数据类
- `ConceptMap`: 概念图数据类

### 图形渲染模块 (graphics)

提供图形渲染功能，包括：

- `GraphicsNode`: 图形节点类（继承 QGraphicsItem）
- `GraphicsEdge`: 图形连接线类（继承 QGraphicsItem）
- `GraphicsScene`: 图形场景类（继承 QGraphicsScene）
- `GraphicsView`: 图形视图类（继承 QGraphicsView）

### 命令模式模块 (commands)

提供撤销/重做功能，包括：

- `AddNodeCommand`: 添加节点命令
- `AddEdgeCommand`: 添加连接线命令
- `DeleteNodeCommand`: 删除节点命令
- `DeleteEdgeCommand`: 删除连接线命令
- `MoveNodeCommand`: 移动节点命令

### 数据模型模块 (models)

提供数据模型和持久化，包括：

- `NodeModel`: 节点模型（继承 QAbstractTableModel）
- `EdgeModel`: 连接线模型（继承 QAbstractTableModel）
- `MapModel`: 概念图模型

### 管理器模块 (managers)

提供样式和文件管理，包括：

- `StyleManager`: 样式管理器
- `FileManager`: 文件管理器

### 用户界面模块 (ui)

提供用户界面，包括：

- `MainWindow`: 主窗口（继承 QMainWindow）
- `ToolBar`: 工具栏（继承 QToolBar）
- `PropertyPanel`: 属性面板（继承 QWidget）

## 开发规范

- 所有代码使用中文注释
- 遵循 Qt 编码规范
- 使用 C++17 特性
- 代码具有可读性和可维护性

## 使用说明

### 创建概念图

1. 点击工具栏的"添加节点"按钮
2. 在画布上点击即可创建新节点
3. 双击节点可以编辑节点文本
4. 在属性面板中可以修改节点属性

### 添加连接线

1. 按住 Shift 键
2. 点击并拖拽一个节点
3. 将鼠标拖拽到另一个节点上
4. 释放鼠标即可创建连接线
5. 在属性面板中可以修改连接线属性

### 自动排版

1. 创建多个节点和连接线
2. 点击"视图" -> "自动排版"或按Ctrl+L
3. 程序会自动按照箭头指向将节点排列成树形结构
4. 根节点在顶部，子节点按层级向下排列

### 保存和导出

1. 使用"文件" -> "保存"保存概念图
2. 使用"文件" -> "导出"导出为图片、PDF 或 SVG

## 许可证

本项目仅供学习和研究使用。

## 贡献

欢迎提交 Issue 和 Pull Request。

## 联系方式

如有问题或建议，请联系开发者。

## 更新日志

### 版本 1.2.0 (2026-01-11)

- 修复JSON保存功能，确保数据正确保存
- 修复JSON加载功能，修复clearScene方法
- 实现节点位置保存和加载功能，使用网格布局
- 实现自动排版功能，使用树形布局算法
- 更新README.md，添加自动排版功能说明
- 更新.gitignore文件

### 版本 1.1.0 (2026-01-08)

- 完成所有高优先级和中优先级优化（8/12 项，完成度 67%）
- 修复状态栏缩放标签连接时机问题
- 完成撤销/重做功能集成
- 消除 JSON 序列化代码重复
- 提取连接线交点计算公共方法
- 优化索引映射重建
- 实现节点形状绘制
- 完成属性面板功能
- 添加异常处理
- 创建 ConceptMapSerializer 类
- 编译测试通过
- 运行测试：程序启动后立即退出（待解决）

### 版本 1.0.0 (2026-01-08)

- 初始版本发布
- 实现基本的概念图绘制功能
- 支持节点和连接线的创建、编辑和删除
- 支持文件保存和导出
- 支持撤销/重做功能
- 支持多种样式主题
