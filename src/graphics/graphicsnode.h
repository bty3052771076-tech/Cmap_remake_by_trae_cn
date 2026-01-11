#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "conceptnode.h"

/**
 * @brief 图形节点类（继承 QGraphicsObject）
 *
 * 该类用于在图形场景中渲染概念节点，包括：
 * - 节点的绘制
 * - 鼠标事件处理（点击、拖拽、悬停）
 * - 位置更新
 * - 节点样式管理
 */
class GraphicsNode : public QGraphicsObject
{
    Q_OBJECT

public:
    // 图形项类型常量
    static const int Type = QGraphicsItem::UserType + 1;

    /**
     * @brief 构造函数 - 创建一个图形节点
     * @param node 概念节点数据
     * @param parent 父图形项
     */
    explicit GraphicsNode(const ConceptNode& node, QGraphicsItem* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~GraphicsNode();

    // QGraphicsItem 接口实现
    /**
     * @brief 返回图形项的边界矩形
     * @return 边界矩形
     */
    QRectF boundingRect() const override;

    /**
     * @brief 绘制图形项
     * @param painter 绘制器
     * @param option 样式选项
     * @param widget 父窗口部件
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    /**
     * @brief 返回图形项的类型
     * @return 类型标识符
     */
    int type() const override;

    // Getter 方法
    /**
     * @brief 获取节点ID
     * @return 节点ID
     */
    QString id() const { return m_node.id(); }

    /**
     * @brief 获取概念节点数据
     * @return 概念节点数据
     */
    ConceptNode node() const { return m_node; }

    /**
     * @brief 获取节点文本
     * @return 节点文本
     */
    QString text() const { return m_node.text(); }

    /**
     * @brief 获取节点颜色
     * @return 节点颜色
     */
    QColor color() const { return m_node.color(); }

    /**
     * @brief 检查节点是否被选中
     * @return 如果被选中返回 true，否则返回 false
     */
    bool isSelectedNode() const { return m_isSelected; }

    // Setter 方法
    /**
     * @brief 设置节点文本
     * @param text 新的文本
     */
    void setText(const QString& text);

    /**
     * @brief 设置节点颜色
     * @param color 新的颜色
     */
    void setColor(const QColor& color);

    /**
     * @brief 设置节点选中状态
     * @param selected 选中状态
     */
    void setSelectedNode(bool selected);

    /**
     * @brief 更新节点数据
     * @param node 新的节点数据
     */
    void updateNode(const ConceptNode& node);

    /**
     * @brief 更新节点位置
     * @param pos 新的位置
     */
    void updatePosition(const QPointF& pos);

signals:
    /**
     * @brief 位置变化信号
     */
    void positionChanged(const QPointF& pos);

protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     */
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief 鼠标移动事件处理
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief 鼠标悬停进入事件处理
     * @param event 悬停事件
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    /**
     * @brief 鼠标悬停离开事件处理
     * @param event 悬停事件
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    /**
     * @brief 项位置变化事件处理
     * @param change 变化类型
     * @param value 新值
     * @return 新值
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    /**
     * @brief 计算文本边界矩形
     * @return 文本边界矩形
     */
    QRectF calculateTextRect() const;

    /**
     * @brief 绘制节点背景
     * @param painter 绘制器
     */
    void drawBackground(QPainter* painter);

    /**
     * @brief 绘制节点边框
     * @param painter 绘制器
     */
    void drawBorder(QPainter* painter);

    /**
     * @brief 绘制节点文本
     * @param painter 绘制器
     */
    void drawText(QPainter* painter);

    /**
     * @brief 绘制选中效果
     * @param painter 绘制器
     */
    void drawSelection(QPainter* painter);

    ConceptNode m_node;              // 概念节点数据
    bool m_isSelected;               // 是否被选中
    bool m_isHovered;                // 是否被悬停
    QPointF m_dragStartPos;          // 拖拽起始位置
    qreal m_cornerRadius;            // 圆角半径
    qreal m_borderWidth;             // 边框宽度
    QColor m_borderColor;            // 边框颜色
    QColor m_selectionColor;         // 选中颜色
    QColor m_hoverColor;             // 悬停颜色
};

#endif // GRAPHICSNODE_H
