# ConceptMap - 概念图软件

## 项目简介

ConceptMap 是一个基于 Qt 6.10.1 的概念图绘制软件，使用 qmake 构建系统开发。该软件提供了完整的概念图创建、编辑和管理功能。

## 系统要求

- Windows 11
- Qt 6.10.1 (安装路径: E:\Qt\6.10.1\mingw_64)
- MinGW 编译器
- C++17 编译器

## 项目结构

```
d:\AI\trae\prj_2\
├── ConceptMap.pro            # 主 qmake 项目文件
├── BUILD.md                 # 构建说明文件
├── README.md                # 项目说明文件
├── src/
│   ├── core/                   # 核心模块
│   │   ├── conceptnode.h/cpp   # 概念节点类
│   │   ├── conceptedge.h/cpp   # 连接线类
│   │   └── conceptmap.h/cpp    # 概念图类
│   ├── graphics/               # 图形渲染模块
│   │   ├── graphicsnode.h/cpp  # 图形节点类
│   │   ├── graphicsedge.h/cpp  # 图形连接线类
│   │   ├── graphicsscene.h/cpp # 图形场景类
│   │   └── graphicsview.h/cpp  # 图形视图类
│   ├── commands/               # 命令模式
│   │   ├── addnodecommand.h/cpp    # 添加节点命令
│   │   ├── addedgecommand.h/cpp    # 添加连接线命令
│   │   ├── deletenodecommand.h/cpp # 删除节点命令
│   │   ├── deleteedgecommand.h/cpp # 删除连接线命令
│   │   └── movenodecommand.h/cpp  # 移动节点命令
│   ├── models/                 # 数据模型
│   │   ├── nodemodel.h/cpp     # 节点模型
│   │   ├── edgemodel.h/cpp     # 连接线模型
│   │   └── mapmodel.h/cpp      # 概念图模型
│   ├── managers/               # 管理器
│   │   ├── stylemanager.h/cpp  # 样式管理器
│   │   └── filemanager.h/cpp   # 文件管理器
│   └── ui/                     # 用户界面
│       ├── mainwindow.h/cpp    # 主窗口
│       ├── toolbar.h/cpp       # 工具栏
│       ├── propertypanel.h/cpp # 属性面板
│       └── main.cpp            # 主函数
├── resources/                  # 资源文件
│   ├── styles/                 # 样式文件
│   │   ├── academic_blue.json  # 学术蓝配色
│   │   ├── nature_green.json   # 自然绿配色
│   │   └── vibrant_orange.json # 活力橙配色
│   └── icons/                  # 图标文件
│       └── icons.qrc           # 资源文件
└── tests/                      # 测试文件
```

## 构建步骤

### 1. 打开命令提示符或 PowerShell

在 Windows 11 中，按 `Win + X` 键，选择"Windows PowerShell"或"命令提示符"。

### 2. 进入项目目录

```powershell
cd d:\AI\trae\prj_2
```

### 3. 创建构建目录

```powershell
mkdir build
cd build
```

### 4. 运行 qmake 配置项目

```powershell
& "E:\Qt\6.10.1\mingw_64\bin\qmake.exe" ..\ConceptMap.pro
```

### 5. 编译项目

```powershell
mingw32-make
```

或者使用多线程编译（更快）：

```powershell
mingw32-make -j4
```

### 6. 运行程序

编译成功后，可执行文件位于：

```
build\release\ConceptMap.exe
```

直接运行：

```powershell
cd release
.\ConceptMap.exe
```

## 功能特性

### 核心功能

1. **节点管理**
   - 添加、删除、编辑节点
   - 设置节点文本、颜色、位置和尺寸
   - 节点拖拽移动
   - 多选节点（Ctrl+点击）
   - 全选节点（Ctrl+A）

2. **连接线管理**
   - 添加、删除、编辑连接线
   - 设置连接线标签和颜色
   - 自动连接线更新
   - 拖拽创建连接线（按住 Shift 键）

3. **视图操作**
   - 缩放（放大、缩小、重置、适应视图）
   - 平移（鼠标中键拖拽）
   - 网格显示

4. **编辑操作**
   - 撤销/重做（命令模式）
   - 删除选中项
   - 全选

5. **文件操作**
   - 新建、打开、保存、另存为
   - 导出为图片（PNG、JPG、BMP）
   - 导出为 PDF
   - 导出为 SVG
   - 最近文件管理

6. **样式管理**
   - 预设样式（学术蓝、自然绿、活力橙）
   - 自定义样式
   - 样式切换

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

## 使用说明

### 创建概念图

1. 点击工具栏的"添加节点"按钮，或使用菜单"编辑" -> "添加节点"
2. 在画布上点击即可创建新节点
3. 双击节点可以编辑节点文本
4. 在属性面板中可以修改节点属性（文本、颜色、位置、尺寸）

### 添加连接线

1. 按住 Shift 键
2. 点击并拖拽一个节点
3. 将鼠标拖拽到另一个节点上
4. 释放鼠标即可创建连接线
5. 在属性面板中可以修改连接线属性（标签、颜色）

### 编辑概念图

1. 点击选择节点或连接线
2. 拖拽节点可以移动位置
3. 按住 Ctrl 键点击节点可以多选
4. 在属性面板中修改属性
5. 使用 Delete 键删除选中项

### 保存和导出

1. 使用"文件" -> "保存"保存概念图
2. 使用"文件" -> "导出" -> "图片"导出为图片
3. 使用"文件" -> "导出" -> "PDF"导出为 PDF
4. 使用"文件" -> "导出" -> "SVG"导出为 SVG

## 技术栈

- **编程语言**: C++17
- **框架**: Qt 6.10.1
- **构建工具**: qmake
- **图形框架**: Qt Graphics View Framework
- **设计模式**: 命令模式、模型-视图模式

## 开发说明

### 代码规范

- 所有代码使用中文注释
- 遵循 Qt 编码规范
- 使用 C++17 特性
- 代码具有可读性和可维护性

### 模块说明

1. **核心模块 (core)**: 提供概念图的数据模型
2. **图形渲染模块 (graphics)**: 提供图形渲染功能
3. **命令模式模块 (commands)**: 提供撤销/重做功能
4. **数据模型模块 (models)**: 提供数据模型和持久化
5. **管理器模块 (managers)**: 提供样式和文件管理
6. **用户界面模块 (ui)**: 提供用户界面

### 扩展开发

如需添加新功能，请遵循以下步骤：

1. 在相应模块中添加新类
2. 更新 ConceptMap.pro 文件
3. 在主窗口中集成新功能
4. 添加必要的测试

## 常见问题

### 编译错误

如果遇到编译错误，请检查：

1. Qt 路径是否正确（E:\Qt\6.10.1\mingw_64）
2. MinGW 编译器是否正确安装
3. C++17 支持是否已启用
4. 项目文件路径是否正确

### 运行时错误

如果遇到运行时错误，请检查：

1. Qt 动态库是否在系统路径中
2. 资源文件是否正确加载
3. 样式文件是否存在

## 许可证

本项目仅供学习和研究使用。

## 联系方式

如有问题或建议，请联系开发者。

## 更新日志

### 版本 1.0.0 (2026-01-08)

- 初始版本发布
- 实现基本的概念图绘制功能
- 支持节点和连接线的创建、编辑和删除
- 支持文件保存和导出
- 支持撤销/重做功能
- 支持多种样式主题
- 实现拖拽创建连接线功能
- 实现多选节点功能
- 实现全选功能
- 添加节点形状支持（基础框架）
