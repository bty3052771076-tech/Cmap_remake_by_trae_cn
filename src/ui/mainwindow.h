#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoStack>
#include <QSplitter>
#include "graphicsscene.h"
#include "graphicsview.h"
#include "mapmodel.h"
#include "stylemanager.h"
#include "filemanager.h"
#include "toolbar.h"
#include "propertypanel.h"

/**
 * @brief 主窗口类（继承 QMainWindow）
 *
 * 该类是应用程序的主窗口，包括：
 * - 菜单栏
 * - 工具栏
 * - 属性面板
 * - 画布
 * - 文件操作
 * - 编辑操作
 * - 视图操作
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建主窗口
     * @param parent 父窗口
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MainWindow();

    /**
     * @brief 获取图形场景
     * @return 图形场景
     */
    GraphicsScene* scene() const { return m_scene; }

    /**
     * @brief 获取图形视图
     * @return 图形视图
     */
    GraphicsView* view() const { return m_view; }

    /**
     * @brief 获取概念图模型
     * @return 概念图模型
     */
    MapModel* mapModel() { return &m_mapModel; }

    /**
     * @brief 获取撤销栈
     * @return 撤销栈
     */
    QUndoStack* undoStack() { return &m_undoStack; }

    /**
     * @brief 获取样式管理器
     * @return 样式管理器
     */
    StyleManager* styleManager() { return &m_styleManager; }

    /**
     * @brief 获取文件管理器
     * @return 文件管理器
     */
    FileManager* fileManager() { return &m_fileManager; }

protected:
    /**
     * @brief 关闭事件处理
     * @param event 关闭事件
     */
    void closeEvent(QCloseEvent* event) override;

private:
    /**
     * @brief 初始化UI
     */
    void initUI();

    /**
     * @brief 创建菜单栏
     */
    void createMenuBar();

    /**
     * @brief 创建工具栏
     */
    void createToolBar();

    /**
     * @brief 创建状态栏
     */
    void createStatusBar();

    /**
     * @brief 创建中央窗口
     */
    void createCentralWidget();

    /**
     * @brief 创建属性面板
     */
    void createPropertyPanel();

    /**
     * @brief 连接信号和槽
     */
    void connectSignals();

    // 文件操作
    /**
     * @brief 新建文件
     */
    void newFile();

    /**
     * @brief 打开文件
     */
    void openFile();

    /**
     * @brief 保存文件
     */
    void saveFile();

    /**
     * @brief 另存为
     */
    void saveAsFile();

    /**
     * @brief 导出为图片
     */
    void exportImage();

    /**
     * @brief 导出为PDF
     */
    void exportPDF();

    /**
     * @brief 导出为SVG
     */
    void exportSVG();

    // 编辑操作
    /**
     * @brief 撤销
     */
    void undo();

    /**
     * @brief 重做
     */
    void redo();

    /**
     * @brief 添加节点
     */
    void addNode();

    /**
     * @brief 添加连接线
     */
    void addEdge();

    /**
     * @brief 删除选中项
     */
    void deleteSelected();

    /**
     * @brief 全选
     */
    void selectAll();

    // 视图操作
    /**
     * @brief 放大
     */
    void zoomIn();

    /**
     * @brief 缩小
     */
    void zoomOut();

    /**
     * @brief 重置缩放
     */
    void resetZoom();

    /**
     * @brief 适应视图
     */
    void fitView();

    /**
     * @brief 自动排版
     */
    void autoLayout();

    // 导出操作
    /**
     * @brief 导出为Cmap格式
     */
    void exportCmap();

    // 帮助操作
    /**
     * @brief 显示关于对话框
     */
    void showAbout();

    /**
     * @brief 加载最近文件
     */
    void loadRecentFile();

    /**
     * @brief 更新最近文件菜单
     */
    void updateRecentFilesMenu();

    /**
     * @brief 更新窗口标题
     */
    void updateWindowTitle();

    /**
     * @brief 检查是否需要保存
     * @return 如果需要保存返回 true，否则返回 false
     */
    bool maybeSave();

    GraphicsScene* m_scene;              // 图形场景
    GraphicsView* m_view;                // 图形视图
    MapModel m_mapModel;                 // 概念图模型
    QUndoStack m_undoStack;              // 撤销栈
    StyleManager m_styleManager;         // 样式管理器
    FileManager m_fileManager;           // 文件管理器
    ToolBar* m_toolBar;                  // 工具栏
    PropertyPanel* m_propertyPanel;      // 属性面板
    QSplitter* m_splitter;               // 分割器
    QString m_currentFilePath;           // 当前文件路径
    bool m_isModified;                   // 是否已修改
    QMenu* m_recentFilesMenu;            // 最近文件菜单
    QLabel* m_zoomLabel;                // 缩放标签
};

#endif // MAINWINDOW_H
