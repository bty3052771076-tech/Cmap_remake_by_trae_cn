#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QButtonGroup>

/**
 * @brief 工具栏类（继承 QToolBar）
 *
 * 该类用于提供常用操作的快捷访问，包括：
 * - 添加节点
 * - 添加连接线
 * - 选择工具
 * - 保存/打开
 * - 导出
 * - 撤销/重做
 */
class ToolBar : public QToolBar
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建工具栏
     * @param parent 父窗口
     */
    explicit ToolBar(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ToolBar();

    /**
     * @brief 设置当前工具
     * @param tool 工具名称
     */
    void setCurrentTool(const QString& tool);

    /**
     * @brief 获取当前工具
     * @return 工具名称
     */
    QString currentTool() const { return m_currentTool; }

signals:
    /**
     * @brief 添加节点点击信号
     */
    void addNodeClicked();

    /**
     * @brief 添加连接线点击信号
     */
    void addEdgeClicked();

    /**
     * @brief 选择点击信号
     */
    void selectClicked();

    /**
     * @brief 保存点击信号
     */
    void saveClicked();

    /**
     * @brief 打开点击信号
     */
    void openClicked();

    /**
     * @brief 导出点击信号
     */
    void exportClicked();

    /**
     * @brief 撤销点击信号
     */
    void undoClicked();

    /**
     * @brief 重做点击信号
     */
    void redoClicked();

private:
    /**
     * @brief 创建工具栏动作
     */
    void createActions();

    /**
     * @brief 创建工具按钮组
     */
    void createToolButtons();

    /**
     * @brief 创建分隔符
     */
    void createSeparators();

    QAction* m_addNodeAction;       // 添加节点动作
    QAction* m_addEdgeAction;       // 添加连接线动作
    QAction* m_selectAction;        // 选择动作
    QAction* m_saveAction;          // 保存动作
    QAction* m_openAction;          // 打开动作
    QAction* m_exportAction;        // 导出动作
    QAction* m_undoAction;          // 撤销动作
    QAction* m_redoAction;          // 重做动作
    QButtonGroup* m_toolGroup;      // 工具按钮组
    QString m_currentTool;          // 当前工具
};

#endif // TOOLBAR_H
