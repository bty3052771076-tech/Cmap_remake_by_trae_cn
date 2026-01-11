#include "mainwindow.h"
#include "deleteedgecommand.h"
#include "deletenodecommand.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QDockWidget>
#include <QQueue>

/**
 * @brief 构造函数 - 创建主窗口
 * @param parent 父窗口
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_scene(nullptr)
    , m_view(nullptr)
    , m_toolBar(nullptr)
    , m_propertyPanel(nullptr)
    , m_splitter(nullptr)
    , m_isModified(false)
    , m_recentFilesMenu(nullptr)
{
    // 设置窗口属性
    setWindowTitle("ConceptMap - 概念图软件");
    setMinimumSize(800, 600);
    resize(1200, 800);

    // 初始化UI
    initUI();

    // 创建新概念图（不调用 newFile，避免启动时弹出对话框）
    m_mapModel.newMap();
    m_scene->setConceptMap(m_mapModel.conceptMap());
    m_currentFilePath.clear();
    m_isModified = false;
    updateWindowTitle();
}

/**
 * @brief 析构函数
 */
MainWindow::~MainWindow()
{
}

/**
 * @brief 关闭事件处理
 * @param event 关闭事件
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

/**
 * @brief 初始化UI
 */
void MainWindow::initUI()
{
    // 创建菜单栏
    createMenuBar();

    // 创建工具栏
    createToolBar();

    // 创建状态栏
    createStatusBar();

    // 创建中央窗口
    createCentralWidget();

    // 创建属性面板
    createPropertyPanel();

    // 连接信号和槽
    connectSignals();
}

/**
 * @brief 创建菜单栏
 */
void MainWindow::createMenuBar()
{
    // 文件菜单
    QMenu* fileMenu = menuBar()->addMenu("文件(&F)");

    QAction* newAction = fileMenu->addAction("新建(&N)");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    QAction* openAction = fileMenu->addAction("打开(&O)...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    fileMenu->addSeparator();

    QAction* saveAction = fileMenu->addAction("保存(&S)");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    QAction* saveAsAction = fileMenu->addAction("另存为(&A)...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);

    fileMenu->addSeparator();

    // 最近文件菜单
    m_recentFilesMenu = fileMenu->addMenu("最近文件(&R)");
    updateRecentFilesMenu();

    fileMenu->addSeparator();

    // 导出子菜单
    QMenu* exportMenu = fileMenu->addMenu("导出(&E)");

    QAction* exportImageAction = exportMenu->addAction("图片(&I)...");
    connect(exportImageAction, &QAction::triggered, this, &MainWindow::exportImage);

    QAction* exportPDFAction = exportMenu->addAction("PDF(&P)...");
    connect(exportPDFAction, &QAction::triggered, this, &MainWindow::exportPDF);

    QAction* exportSVGAction = exportMenu->addAction("SVG(&S)...");
    connect(exportSVGAction, &QAction::triggered, this, &MainWindow::exportSVG);

    QAction* exportCmapAction = exportMenu->addAction("Cmap(&C)...");
    connect(exportCmapAction, &QAction::triggered, this, &MainWindow::exportCmap);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction("退出(&X)");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 编辑菜单
    QMenu* editMenu = menuBar()->addMenu("编辑(&E)");

    QAction* undoAction = editMenu->addAction("撤销(&U)");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    QAction* redoAction = editMenu->addAction("重做(&R)");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    editMenu->addSeparator();

    QAction* addNodeAction = editMenu->addAction("添加节点(&N)");
    addNodeAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(addNodeAction, &QAction::triggered, this, &MainWindow::addNode);

    QAction* addEdgeAction = editMenu->addAction("添加连接线(&E)");
    addEdgeAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(addEdgeAction, &QAction::triggered, this, &MainWindow::addEdge);

    editMenu->addSeparator();

    QAction* deleteAction = editMenu->addAction("删除(&D)");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelected);

    QAction* selectAllAction = editMenu->addAction("全选(&A)");
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, this, &MainWindow::selectAll);

    // 视图菜单
    QMenu* viewMenu = menuBar()->addMenu("视图(&V)");

    QAction* zoomInAction = viewMenu->addAction("放大(&I)");
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);

    QAction* zoomOutAction = viewMenu->addAction("缩小(&O)");
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);

    QAction* resetZoomAction = viewMenu->addAction("重置缩放(&R)");
    resetZoomAction->setShortcut(QKeySequence("Ctrl+0"));
    connect(resetZoomAction, &QAction::triggered, this, &MainWindow::resetZoom);

    QAction* fitViewAction = viewMenu->addAction("适应视图(&F)");
    fitViewAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(fitViewAction, &QAction::triggered, this, &MainWindow::fitView);

    QAction* autoLayoutAction = viewMenu->addAction("自动排版(&L)");
    autoLayoutAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(autoLayoutAction, &QAction::triggered, this, &MainWindow::autoLayout);

    // 帮助菜单
    QMenu* helpMenu = menuBar()->addMenu("帮助(&H)");

    QAction* aboutAction = helpMenu->addAction("关于(&A)...");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

/**
 * @brief 创建工具栏
 */
void MainWindow::createToolBar()
{
    m_toolBar = new ToolBar(this);
    addToolBar(m_toolBar);

    // 连接工具栏信号
    connect(m_toolBar, &ToolBar::addNodeClicked, this, &MainWindow::addNode);
    connect(m_toolBar, &ToolBar::addEdgeClicked, this, &MainWindow::addEdge);
    connect(m_toolBar, &ToolBar::selectClicked, this, &MainWindow::selectAll);
    connect(m_toolBar, &ToolBar::saveClicked, this, &MainWindow::saveFile);
    connect(m_toolBar, &ToolBar::openClicked, this, &MainWindow::openFile);
    connect(m_toolBar, &ToolBar::exportClicked, this, &MainWindow::exportImage);
    connect(m_toolBar, &ToolBar::undoClicked, this, &MainWindow::undo);
}

/**
 * @brief 创建状态栏
 */
void MainWindow::createStatusBar()
{
    QLabel* statusLabel = new QLabel("就绪");
    statusBar()->addWidget(statusLabel);

    m_zoomLabel = new QLabel("缩放: 100%");
    statusBar()->addPermanentWidget(m_zoomLabel);
}

/**
 * @brief 创建中央窗口
 */
void MainWindow::createCentralWidget()
{
    // 创建图形场景
    m_scene = new GraphicsScene(this);

    // 创建图形视图
    m_view = new GraphicsView(m_scene, this);

    // 设置视图为中央窗口
    setCentralWidget(m_view);

    // 连接视图缩放信号
    connect(m_view, &GraphicsView::zoomChanged, this, [this](qreal scale) {
        m_zoomLabel->setText(QString("缩放: %1%").arg(qRound(scale * 100)));
    });

    // 连接场景信号
    connect(m_scene, &GraphicsScene::sceneChanged, this, [this]() {
        m_isModified = true;
        updateWindowTitle();
    });
}

/**
 * @brief 创建属性面板
 */
void MainWindow::createPropertyPanel()
{
    m_propertyPanel = new PropertyPanel(this);

    // 添加到右侧停靠窗口
    QDockWidget* dockWidget = new QDockWidget("属性", this);
    dockWidget->setWidget(m_propertyPanel);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    // 连接属性面板信号
    connect(m_propertyPanel, &PropertyPanel::nodeTextChanged, [this](const QString& text) {
        // 更新选中节点的文本
        QList<GraphicsNode*> selectedNodes = m_scene->selectedNodes();
        if (!selectedNodes.isEmpty()) {
            GraphicsNode* node = selectedNodes.first();
            node->setText(text);
            m_isModified = true;
            updateWindowTitle();
        }
    });

    connect(m_propertyPanel, &PropertyPanel::nodeColorChanged, [this](const QColor& color) {
        // 更新选中节点的颜色
        QList<GraphicsNode*> selectedNodes = m_scene->selectedNodes();
        if (!selectedNodes.isEmpty()) {
            GraphicsNode* node = selectedNodes.first();
            node->setColor(color);
            m_isModified = true;
            updateWindowTitle();
        }
    });
}

/**
 * @brief 连接信号和槽
 */
void MainWindow::connectSignals()
{
    // 连接文件管理器信号
    connect(&m_fileManager, &FileManager::recentFilesChanged, this, &MainWindow::updateRecentFilesMenu);

    // 连接场景选中信号
    connect(m_scene, &GraphicsScene::nodeSelected, [this](const QString& nodeId) {
        GraphicsNode* node = m_scene->graphicsNodeById(nodeId);
        if (node) {
            m_propertyPanel->setNode(node->node());
        }
    });

    connect(m_scene, &GraphicsScene::edgeSelected, [this](const QString& edgeId) {
        GraphicsEdge* edge = m_scene->graphicsEdgeById(edgeId);
        if (edge) {
            m_propertyPanel->setEdge(edge->edge());
        }
    });
}

/**
 * @brief 新建文件
 */
void MainWindow::newFile()
{
    if (maybeSave()) {
        m_mapModel.newMap();
        m_scene->setConceptMap(m_mapModel.conceptMap());
        m_currentFilePath.clear();
        m_isModified = false;
        m_undoStack.clear();
        updateWindowTitle();
    }
}

/**
 * @brief 打开文件
 */
void MainWindow::openFile()
{
    if (maybeSave()) {
        QString filePath = QFileDialog::getOpenFileName(this, "打开概念图", "", m_fileManager.fileFilter());
        if (!filePath.isEmpty()) {
            if (m_mapModel.loadFromFile(filePath)) {
                m_scene->setConceptMap(m_mapModel.conceptMap());
                m_currentFilePath = filePath;
                m_isModified = false;
                m_undoStack.clear();
                m_fileManager.addToRecentFiles(filePath);
                updateWindowTitle();
                statusBar()->showMessage(QString("已打开: %1").arg(filePath), 3000);
            } else {
                QMessageBox::warning(this, "错误", "无法打开文件");
            }
        }
    }
}

/**
 * @brief 保存文件
 */
void MainWindow::saveFile()
{
    if (m_currentFilePath.isEmpty()) {
        saveAsFile();
    } else {
        // 同步场景数据到模型
        m_mapModel.setConceptMap(m_scene->conceptMap());
        if (m_mapModel.saveToFile(m_currentFilePath)) {
            m_isModified = false;
            updateWindowTitle();
            statusBar()->showMessage(QString("已保存: %1").arg(m_currentFilePath), 3000);
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件");
        }
    }
}

/**
 * @brief 另存为
 */
void MainWindow::saveAsFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "保存概念图", "", m_fileManager.fileFilter());
    if (!filePath.isEmpty()) {
        // 同步场景数据到模型
        m_mapModel.setConceptMap(m_scene->conceptMap());
        if (m_mapModel.saveToFile(filePath)) {
            m_currentFilePath = filePath;
            m_isModified = false;
            m_fileManager.addToRecentFiles(filePath);
            updateWindowTitle();
            statusBar()->showMessage(QString("已保存: %1").arg(filePath), 3000);
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件");
        }
    }
}

/**
 * @brief 导出为图片
 */
void MainWindow::exportImage()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出为图片", "", m_fileManager.imageFilter());
    if (!filePath.isEmpty()) {
        if (m_fileManager.exportToImage(filePath, m_scene)) {
            statusBar()->showMessage(QString("已导出: %1").arg(filePath), 3000);
        } else {
            QMessageBox::warning(this, "错误", "无法导出图片");
        }
    }
}

/**
 * @brief 导出为PDF
 */
void MainWindow::exportPDF()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出为PDF", "", "PDF 文件 (*.pdf)");
    if (!filePath.isEmpty()) {
        if (m_fileManager.exportToPDF(filePath, m_scene)) {
            statusBar()->showMessage(QString("已导出: %1").arg(filePath), 3000);
        } else {
            QMessageBox::warning(this, "错误", "无法导出PDF");
        }
    }
}

/**
 * @brief 导出为SVG
 */
void MainWindow::exportSVG()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出为SVG", "", "SVG 文件 (*.svg)");
    if (!filePath.isEmpty()) {
        if (m_fileManager.exportToSVG(filePath, m_scene)) {
            statusBar()->showMessage(QString("已导出: %1").arg(filePath), 3000);
        } else {
            QMessageBox::warning(this, "错误", "无法导出SVG");
        }
    }
}

/**
 * @brief 导出为Cmap格式
 */
void MainWindow::exportCmap()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出为Cmap", "", "Cmap 文件 (*.cmap)");
    if (!filePath.isEmpty()) {
        if (m_fileManager.exportToCmap(filePath, m_mapModel.conceptMap())) {
            statusBar()->showMessage(QString("已导出: %1").arg(filePath), 3000);
        } else {
            QMessageBox::warning(this, "错误", "无法导出Cmap");
        }
    }
}

/**
 * @brief 撤销
 */
void MainWindow::undo()
{
    m_undoStack.undo();
}

/**
 * @brief 重做
 */
void MainWindow::redo()
{
    m_undoStack.redo();
}

/**
 * @brief 添加节点
 */
void MainWindow::addNode()
{
    // 使用静态计数器跟踪已创建的节点数量
    static int nodeCount = 0;
    
    // 根据节点数量计算新节点的位置（网格布局）
    int col = nodeCount % 3;  // 每行3个节点
    int row = nodeCount / 3;  // 行号
    qreal x = 100 + col * 200;  // 每列间隔200像素
    qreal y = 100 + row * 150;  // 每行间隔150像素
    
    ConceptNode node("新节点", x, y, 120, 60);
    m_scene->addNode(node);
    m_isModified = true;
    updateWindowTitle();
    
    // 增加节点计数器
    nodeCount++;
}

/**
 * @brief 添加连接线
 */
void MainWindow::addEdge()
{
    QMessageBox::information(this, "添加连接线", 
        "使用方法：\n\n"
        "1. 按住 Shift 键\n"
        "2. 点击并拖拽一个节点\n"
        "3. 将鼠标拖拽到另一个节点上\n"
        "4. 释放鼠标即可创建连接线\n\n"
        "提示：连接线会自动连接两个节点的边缘。");
}

/**
 * @brief 删除选中项
 */
void MainWindow::deleteSelected()
{
    // 使用命令模式删除选中的项
    QList<GraphicsEdge*> selectedEdges = m_scene->selectedEdges();
    for (GraphicsEdge* edge : selectedEdges) {
        m_undoStack.push(new DeleteEdgeCommand(m_scene, edge->id()));
    }
    
    QList<GraphicsNode*> selectedNodes = m_scene->selectedNodes();
    for (GraphicsNode* node : selectedNodes) {
        m_undoStack.push(new DeleteNodeCommand(m_scene, node->id()));
    }
    
    m_isModified = true;
    updateWindowTitle();
}

/**
 * @brief 全选
 */
void MainWindow::selectAll()
{
    // 选中所有节点
    QList<GraphicsNode*> nodes = m_scene->allNodes();
    for (GraphicsNode* node : nodes) {
        node->setSelectedNode(true);
    }
    
    // 选中所有连接线
    QList<GraphicsEdge*> edges = m_scene->allEdges();
    for (GraphicsEdge* edge : edges) {
        edge->setSelectedEdge(true);
    }
}

/**
 * @brief 放大
 */
void MainWindow::zoomIn()
{
    if (m_view) {
        m_view->zoomIn();
    }
}

/**
 * @brief 缩小
 */
void MainWindow::zoomOut()
{
    if (m_view) {
        m_view->zoomOut();
    }
}

/**
 * @brief 重置缩放
 */
void MainWindow::resetZoom()
{
    if (m_view) {
        m_view->resetZoom();
    }
}

/**
 * @brief 适应视图
 */
void MainWindow::fitView()
{
    if (m_view) {
        m_view->fitInView();
    }
}

/**
 * @brief 自动排版
 */
void MainWindow::autoLayout()
{
    // 获取所有节点和连接线
    QVector<ConceptNode> nodes = m_mapModel.conceptMap().nodes();
    QVector<ConceptEdge> edges = m_mapModel.conceptMap().edges();
    
    if (nodes.isEmpty()) {
        QMessageBox::information(this, "自动排版", "没有节点需要排版");
        return;
    }
    
    // 构建邻接表：节点ID -> 子节点列表
    QMap<QString, QVector<QString>> adjacencyList;
    for (const ConceptEdge& edge : edges) {
        adjacencyList[edge.sourceNodeId()].append(edge.targetNodeId());
    }
    
    // 识别根节点（没有入边的节点）
    QVector<QString> rootNodes;
    QSet<QString> hasIncomingEdge;
    for (const ConceptEdge& edge : edges) {
        hasIncomingEdge.insert(edge.targetNodeId());
    }
    for (const ConceptNode& node : nodes) {
        if (!hasIncomingEdge.contains(node.id())) {
            rootNodes.append(node.id());
        }
    }
    
    if (rootNodes.isEmpty()) {
        QMessageBox::information(this, "自动排版", "无法识别根节点，请确保至少有一个节点没有入边");
        return;
    }
    
    // 使用BFS遍历节点，按层级组织
    QMap<QString, int> nodeLevel;  // 节点 -> 层级
    QList<QString> visitOrder;  // 访问顺序
    
    // 初始化所有节点的层级为-1（未访问）
    for (const ConceptNode& node : nodes) {
        nodeLevel[node.id()] = -1;
    }
    
    // 从根节点开始BFS
    for (const QString& rootId : rootNodes) {
        if (nodeLevel[rootId] == -1) {
            QList<QString> levelQueue;
            levelQueue.append(rootId);
            nodeLevel[rootId] = 0;
            visitOrder.append(rootId);
            
            // 按层级遍历
            int currentLevel = 0;
            while (!levelQueue.isEmpty()) {
                QString currentId = levelQueue.takeFirst();
                currentLevel = nodeLevel[currentId];
                
                // 遍历子节点
                QVector<QString> children = adjacencyList.value(currentId);
                for (const QString& childId : children) {
                    if (nodeLevel[childId] == -1) {
                        nodeLevel[childId] = currentLevel + 1;
                        levelQueue.append(childId);
                        visitOrder.append(childId);
                    }
                }
            }
        }
    }
    
    // 布局参数
    const qreal levelSpacing = 150;  // 层间距150像素
    const qreal nodeSpacing = 200;  // 节点间距200像素
    const qreal startX = 100;  // 起始X坐标
    const qreal startY = 100;  // 起始Y坐标
    
    // 计算每层的节点数量
    QMap<int, int> nodesPerLevel;
    for (int level : nodeLevel) {
        if (level >= 0) {
            nodesPerLevel[level]++;
        }
    }
    
    // 按层级排列节点
    for (const QString& nodeId : visitOrder) {
        int level = nodeLevel[nodeId];
        ConceptNode* node = m_mapModel.conceptMap().nodeById(nodeId);
        
        if (node) {
            // 计算该层中当前节点的位置
            int positionInLevel = 0;
            for (const QString& otherId : visitOrder) {
                if (otherId == nodeId) {
                    break;
                }
                if (nodeLevel[otherId] == level) {
                    positionInLevel++;
                }
            }
            
            // 计算该层的起始X坐标（居中排列）
            int count = nodesPerLevel.value(level, 0);
            qreal totalWidth = (count - 1) * nodeSpacing;
            qreal levelStartX = startX - totalWidth / 2;
            
            qreal newX = levelStartX + positionInLevel * nodeSpacing;
            qreal newY = startY + level * levelSpacing;
            
            // 更新数据模型中的节点位置
            node->setPos(QPointF(newX, newY));
            
            // 更新图形节点对象的位置
            GraphicsNode* graphicsNode = m_scene->graphicsNodeById(nodeId);
            if (graphicsNode) {
                graphicsNode->setPos(QPointF(newX, newY));
            }
        }
    }
    
    // 同步场景数据到模型
    m_mapModel.setConceptMap(m_scene->conceptMap());
    
    // 标记为已修改
    m_isModified = true;
    updateWindowTitle();
    
    // 刷新场景
    m_scene->update();
    
    QMessageBox::information(this, "自动排版", 
        QString("已按树形结构排列 %1 个节点，共 %2 层").arg(nodes.size()).arg(nodesPerLevel.keys().size()));
}

/**
 * @brief 显示关于对话框
 */
void MainWindow::showAbout()
{
    QMessageBox::about(this, "关于 ConceptMap",
        "<h2>ConceptMap 概念图软件</h2>"
        "<p>版本: 1.0.0</p>"
        "<p>一个功能强大的概念图绘制工具</p>"
        "<p>使用 Qt 6.10.1 开发</p>");
}

/**
 * @brief 加载最近文件
 */
void MainWindow::loadRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString filePath = action->data().toString();
        if (maybeSave()) {
            if (m_mapModel.loadFromFile(filePath)) {
                m_scene->setConceptMap(m_mapModel.conceptMap());
                m_currentFilePath = filePath;
                m_isModified = false;
                m_undoStack.clear();
                m_fileManager.addToRecentFiles(filePath);
                updateWindowTitle();
                statusBar()->showMessage(QString("已打开: %1").arg(filePath), 3000);
            } else {
                QMessageBox::warning(this, "错误", "无法打开文件");
            }
        }
    }
}

/**
 * @brief 更新最近文件菜单
 */
void MainWindow::updateRecentFilesMenu()
{
    m_recentFilesMenu->clear();

    QStringList recentFiles = m_fileManager.recentFiles();
    for (const QString& filePath : recentFiles) {
        QAction* action = m_recentFilesMenu->addAction(QFileInfo(filePath).fileName());
        action->setData(filePath);
        connect(action, &QAction::triggered, this, &MainWindow::loadRecentFile);
    }

    if (recentFiles.isEmpty()) {
        QAction* action = m_recentFilesMenu->addAction("无最近文件");
        action->setEnabled(false);
    }
}

/**
 * @brief 更新窗口标题
 */
void MainWindow::updateWindowTitle()
{
    QString title = "ConceptMap - 概念图软件";
    if (!m_currentFilePath.isEmpty()) {
        title += " - " + QFileInfo(m_currentFilePath).fileName();
    }
    if (m_isModified) {
        title += " *";
    }
    setWindowTitle(title);
}

/**
 * @brief 检查是否需要保存
 * @return 如果需要保存返回 true，否则返回 false
 */
bool MainWindow::maybeSave()
{
    if (m_isModified) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this, "ConceptMap",
            "文档已修改，是否保存？",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            saveFile();
            return !m_isModified;
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}
