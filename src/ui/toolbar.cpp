#include "toolbar.h"
#include <QToolButton>
#include <QAction>
#include <QIcon>

/**
 * @brief 构造函数 - 创建工具栏
 * @param parent 父窗口
 */
ToolBar::ToolBar(QWidget* parent)
    : QToolBar(parent)
    , m_toolGroup(nullptr)
    , m_currentTool("select")
{
    // 设置工具栏属性
    setWindowTitle("工具栏");
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    setMovable(true);
    setFloatable(true);

    // 创建工具栏动作
    createActions();

    // 创建工具按钮组
    createToolButtons();

    // 创建分隔符
    createSeparators();
}

/**
 * @brief 析构函数
 */
ToolBar::~ToolBar()
{
}

/**
 * @brief 设置当前工具
 * @param tool 工具名称
 */
void ToolBar::setCurrentTool(const QString& tool)
{
    m_currentTool = tool;

    // 更新工具按钮状态
    if (tool == "addNode") {
        m_addNodeAction->setChecked(true);
    } else if (tool == "addEdge") {
        m_addEdgeAction->setChecked(true);
    } else if (tool == "select") {
        m_selectAction->setChecked(true);
    }
}

/**
 * @brief 创建工具栏动作
 */
void ToolBar::ToolBar::createActions()
{
    // 添加节点动作
    m_addNodeAction = new QAction("添加节点", this);
    m_addNodeAction->setCheckable(true);
    m_addNodeAction->setToolTip("添加节点 (Ctrl+N)");
    m_addNodeAction->setStatusTip("在画布上添加一个新节点");
    connect(m_addNodeAction, &QAction::triggered, this, [this]() {
        setCurrentTool("addNode");
        emit addNodeClicked();
    });

    // 添加连接线动作
    m_addEdgeAction = new QAction("添加连接线", this);
    m_addEdgeAction->setCheckable(true);
    m_addEdgeAction->setToolTip("添加连接线 (Ctrl+E)");
    m_addEdgeAction->setStatusTip("在两个节点之间添加连接线");
    connect(m_addEdgeAction, &QAction::triggered, this, [this]() {
        setCurrentTool("addEdge");
        emit addEdgeClicked();
    });

    // 选择动作
    m_selectAction = new QAction("选择", this);
    m_selectAction->setCheckable(true);
    m_selectAction->setChecked(true);
    m_selectAction->setToolTip("选择工具 (Esc)");
    m_selectAction->setStatusTip("选择节点或连接线");
    connect(m_selectAction, &QAction::triggered, this, [this]() {
        setCurrentTool("select");
        emit selectClicked();
    });

    // 保存动作
    m_saveAction = new QAction("保存", this);
    m_saveAction->setToolTip("保存 (Ctrl+S)");
    m_saveAction->setStatusTip("保存当前概念图");
    connect(m_saveAction, &QAction::triggered, this, &ToolBar::saveClicked);

    // 打开动作
    m_openAction = new QAction("打开", this);
    m_openAction->setToolTip("打开 (Ctrl+O)");
    m_openAction->setStatusTip("打开一个概念图文件");
    connect(m_openAction, &QAction::triggered, this, &ToolBar::openClicked);

    // 导出动作
    m_exportAction = new QAction("导出", this);
    m_exportAction->setToolTip("导出 (Ctrl+P)");
    m_exportAction->setStatusTip("导出概念图为图片或PDF");
    connect(m_exportAction, &QAction::triggered, this, &ToolBar::exportClicked);

    // 撤销动作
    m_undoAction = new QAction("撤销", this);
    m_undoAction->setToolTip("撤销 (Ctrl+Z)");
    m_undoAction->setStatusTip("撤销上一步操作");
    connect(m_undoAction, &QAction::triggered, this, &ToolBar::undoClicked);

    // 重做动作
    m_redoAction = new QAction("重做", this);
    m_redoAction->setToolTip("重做 (Ctrl+Y)");
    m_redoAction->setStatusTip("重做上一步撤销的操作");
    connect(m_redoAction, &QAction::triggered, this, &ToolBar::redoClicked);
}

/**
 * @brief 创建工具按钮组
 */
void ToolBar::createToolButtons()
{
    // 创建工具按钮组
    m_toolGroup = new QButtonGroup(this);

    // 添加工具按钮
    addWidget(new QToolButton(this));
    addAction(m_addNodeAction);
    addAction(m_addEdgeAction);
    addAction(m_selectAction);
}

/**
 * @brief 创建分隔符
 */
void ToolBar::createSeparators()
{
    // 添加分隔符
    addSeparator();

    // 添加文件操作按钮
    addAction(m_openAction);
    addAction(m_saveAction);
    addAction(m_exportAction);

    // 添加分隔符
    addSeparator();

    // 添加编辑操作按钮
    addAction(m_undoAction);
    // addAction(m_redoAction);  // TODO: 添加重做按钮
}
