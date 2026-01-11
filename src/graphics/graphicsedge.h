#ifndef GRAPHICSEDGE_H
#define GRAPHICSEDGE_H

#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "conceptedge.h"
#include "graphicsnode.h"

/**
 * @brief 图形连接线类（继承 QGraphicsObject）
 *
 * 该类用于在图形场景中渲染概念图中的连接线，包括：
 * - 连接线的绘制
 * - 鼠标事件处理（点击、悬停）
 * - 位置更新
 * - 连接线样式管理
 */
class GraphicsEdge : public QGraphicsObject
{
    Q_OBJECT
public:
    // 图形项类型常量
    static const int Type = QGraphicsItem::UserType + 2;

    /**
     * @brief 构造函数 - 创建一个图形连接线
     * @param edge 概念连接线数据
     * @param sourceNode 源图形节点
     * @param targetNode 目标图形节点
     * @param parent 父图形项
     */
    explicit GraphicsEdge(const ConceptEdge& edge, GraphicsNode* sourceNode, GraphicsNode* targetNode, QGraphicsItem* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~GraphicsEdge();

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
     * @brief 获取连接线ID
     * @return 连接线ID
     */
    QString id() const { return m_edge.id(); }

    /**
     * @brief 获取概念连接线数据
     * @return 概念连接线数据
     */
    ConceptEdge edge() const { return m_edge; }

    /**
     * @brief 获取源节点ID
     * @return 源节点ID
     */
    QString sourceNodeId() const { return m_edge.sourceNodeId(); }

    /**
     * @brief 获取目标节点ID
     * @return 目标节点ID
     */
    QString targetNodeId() const { return m_edge.targetNodeId(); }

    /**
     * @brief 获取连接线标签
     * @return 连接线标签
     */
    QString label() const { return m_edge.label(); }

    /**
     * @brief 获取连接线颜色
     * @return 连接线颜色
     */
    QColor color() const { return m_edge.color(); }

    /**
     * @brief 获取源图形节点
     * @return 源图形节点
     */
    GraphicsNode* sourceNode() const { return m_sourceNode; }

    /**
     * @brief 获取目标图形节点
     * @return 目标图形节点
     */
    GraphicsNode* targetNode() const { return m_targetNode; }

    /**
     * @brief 检查连接线是否被选中
     * @return 如果被选中返回 true，否则返回 false
     */
    bool isSelectedEdge() const { return m_isSelected; }

    // Setter 方法
    /**
     * @brief 设置连接线标签
     * @param label 新的标签
     */
    void setLabel(const QString& label);

    /**
     * @brief 设置连接线颜色
     * @param color 新的颜色
     */
    void setColor(const QColor& color);

    /**
     * @brief 设置连接线选中状态
     * @param selected 选中状态
     */
    void setSelectedEdge(bool selected);

    /**
     * @brief 设置源图形节点
     * @param node 源图形节点
     */
    void setSourceNode(GraphicsNode* node);

    /**
     * @brief 设置目标图形节点
     * @param node 目标图形节点
     */
    void setTargetNode(GraphicsNode* node);

    /**
     * @brief 更新连接线数据
     * @param edge 新的连接线数据
     */
    void updateEdge(const ConceptEdge& edge);

    /**
     * @brief 更新连接线位置
     */
    void updatePosition();

protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     */
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

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

private:
    /**
     * @brief 计算直线与节点的交点
     * @param rect 节点边界矩形
     * @param center 节点中心
     * @param target 目标点
     * @param shape 节点形状
     * @return 交点坐标
     */
    QPointF calculateIntersectionPoint(const QRectF& rect, const QPointF& center, const QPointF& target, NodeShape shape) const;
    
    /**
     * @brief 计算直线与矩形的交点
     * @param rect 矩形边界
     * @param center 矩形中心
     * @param target 目标点
     * @return 交点坐标
     */
    QPointF calculateRectangleIntersection(const QRectF& rect, const QPointF& center, const QPointF& target) const;
    
    /**
     * @brief 计算直线与椭圆的交点
     * @param rect 椭圆的外切矩形
     * @param center 椭圆中心
     * @param target 目标点
     * @return 交点坐标
     */
    QPointF calculateEllipseIntersection(const QRectF& rect, const QPointF& center, const QPointF& target) const;
    
    /**
     * @brief 计算直线与圆角矩形的交点
     * @param rect 圆角矩形的外切矩形
     * @param center 圆角矩形中心
     * @param target 目标点
     * @return 交点坐标
     */
    QPointF calculateRoundedRectIntersection(const QRectF& rect, const QPointF& center, const QPointF& target) const;
    
    /**
     * @brief 计算连接线起点
     * @return 起点坐标
     */
    QPointF calculateSourcePoint() const;
    
    /**
     * @brief 计算连接线终点
     * @return 终点坐标
     */
    QPointF calculateTargetPoint() const;
    
    /**
     * @brief 绘制连接线
     * @param painter 绘制器
     */
    void drawLine(QPainter* painter);
    
    /**
     * @brief 绘制箭头
     * @param painter 绘制器
     * @param startPoint 起点
     * @param endPoint 终点
     */
    void drawArrow(QPainter* painter, const QPointF& startPoint, const QPointF& endPoint);
    
    /**
     * @brief 绘制标签
     * @param painter 绘制器
     */
    void drawLabel(QPainter* painter);
    
    /**
     * @brief 绘制选中效果
     * @param painter 绘制器
     */
    void drawSelection(QPainter* painter);
    
    ConceptEdge m_edge;              // 概念连接线数据
    GraphicsNode* m_sourceNode;      // 源图形节点
    GraphicsNode* m_targetNode;      // 目标图形节点
    bool m_isSelected;               // 是否被选中
    bool m_isHovered;                // 是否被悬停
    qreal m_lineWidth;               // 线条宽度
    qreal m_arrowSize;               // 箭头大小
    QColor m_selectionColor;         // 选中颜色
    QColor m_hoverColor;             // 悬停颜色
};

#endif // GRAPHICSEDGE_H
