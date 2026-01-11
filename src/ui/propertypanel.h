#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QColorDialog>
#include <QGroupBox>
#include "conceptnode.h"
#include "conceptedge.h"

/**
 * @brief 属性面板类（继承 QWidget）
 *
 * 该类用于显示和编辑选中项的属性，包括：
 * - 节点属性
 * - 连接线属性
 * - 属性更新
 */
class PropertyPanel : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建属性面板
     * @param parent 父窗口
     */
    explicit PropertyPanel(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~PropertyPanel();

    /**
     * @brief 设置节点
     * @param node 节点数据
     */
    void setNode(const ConceptNode& node);

    /**
     * @brief 设置连接线
     * @param edge 连接线数据
     */
    void setEdge(const ConceptEdge& edge);

    /**
     * @brief 清空属性面板
     */
    void clear();

signals:
    /**
     * @brief 节点文本变化信号
     * @param text 新的文本
     */
    void nodeTextChanged(const QString& text);

    /**
     * @brief 节点颜色变化信号
     * @param color 新的颜色
     */
    void nodeColorChanged(const QColor& color);

    /**
     * @brief 连接线标签变化信号
     * @param label 新的标签
     */
    void edgeLabelChanged(const QString& label);

    /**
     * @brief 连接线颜色变化信号
     * @param color 新的颜色
     */
    void edgeColorChanged(const QColor& color);

private:
    /**
     * @brief 初始化UI
     */
    void initUI();

    /**
     * @brief 创建节点属性组
     */
    void createNodeGroup();

    /**
     * @brief 创建连接线属性组
     */
    void createEdgeGroup();

    /**
     * @brief 连接信号和槽
     */
    void connectSignals();

    /**
     * @brief 更新节点属性
     */
    void updateNodeProperties();

    /**
     * @brief 更新连接线属性
     */
    void updateEdgeProperties();

    // 节点属性控件
    QGroupBox* m_nodeGroup;                // 节点属性组
    QLineEdit* m_nodeIdEdit;               // 节点ID编辑框
    QLineEdit* m_nodeTextEdit;             // 节点文本编辑框
    QDoubleSpinBox* m_nodeXSpinBox;         // 节点X坐标
    QDoubleSpinBox* m_nodeYSpinBox;         // 节点Y坐标
    QDoubleSpinBox* m_nodeWidthSpinBox;     // 节点宽度
    QDoubleSpinBox* m_nodeHeightSpinBox;    // 节点高度
    QPushButton* m_nodeColorButton;        // 节点颜色按钮
    QLabel* m_nodeColorLabel;              // 节点颜色标签

    // 连接线属性控件
    QGroupBox* m_edgeGroup;                // 连接线属性组
    QLineEdit* m_edgeIdEdit;               // 连接线ID编辑框
    QLineEdit* m_edgeSourceNodeIdEdit;     // 源节点ID编辑框
    QLineEdit* m_edgeTargetNodeIdEdit;     // 目标节点ID编辑框
    QLineEdit* m_edgeLabelEdit;            // 连接线标签编辑框
    QPushButton* m_edgeColorButton;        // 连接线颜色按钮
    QLabel* m_edgeColorLabel;              // 连接线颜色标签

    ConceptNode m_currentNode;            // 当前节点
    ConceptEdge m_currentEdge;             // 当前连接线
    bool m_isNodeSelected;                 // 是否选中节点
};

#endif // PROPERTYPANEL_H
