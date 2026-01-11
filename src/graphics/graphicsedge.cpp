#include "graphicsedge.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QLineF>
#include <QPainterPath>
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个图形连接线
 * @param edge 概念连接线数据
 * @param sourceNode 源图形节点
 * @param targetNode 目标图形节点
 * @param parent 父图形项
 */
GraphicsEdge::GraphicsEdge(const ConceptEdge& edge, GraphicsNode* sourceNode, GraphicsNode* targetNode, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , m_edge(edge)
    , m_sourceNode(sourceNode)
    , m_targetNode(targetNode)
    , m_isSelected(false)
    , m_isHovered(false)
    , m_lineWidth(2.0)
    , m_arrowSize(10.0)
    , m_selectionColor(QColor(0, 120, 215))
    , m_hoverColor(QColor(0, 120, 215, 100))
{
    // 启用鼠标悬停事件
    setAcceptHoverEvents(true);

    // 设置标志
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // 连接节点位置变化信号
    if (m_sourceNode) {
        connect(m_sourceNode, &GraphicsNode::positionChanged, this, &GraphicsEdge::updatePosition);
    }
    if (m_targetNode) {
        connect(m_targetNode, &GraphicsNode::positionChanged, this, &GraphicsEdge::updatePosition);
    }
}

/**
 * @brief 析构函数
 */
GraphicsEdge::~GraphicsEdge()
{
}

/**
 * @brief 返回图形项的边界矩形
 * @return 边界矩形
 */
QRectF GraphicsEdge::boundingRect() const
{
    if (!m_sourceNode || !m_targetNode) {
        return QRectF();
    }

    QPointF sourcePoint = calculateSourcePoint();
    QPointF targetPoint = calculateTargetPoint();

    // 计算包含连接线和箭头的边界矩形
    qreal padding = m_lineWidth + m_arrowSize + 5.0;
    qreal minX = qMin(sourcePoint.x(), targetPoint.x()) - padding;
    qreal minY = qMin(sourcePoint.y(), targetPoint.y()) - padding;
    qreal maxX = qMax(sourcePoint.x(), targetPoint.x()) + padding;
    qreal maxY = qMax(sourcePoint.y(), targetPoint.y()) + padding;

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

/**
 * @brief 绘制图形项
 * @param painter 绘制器
 * @param option 样式选项
 * @param widget 父窗口部件
 */
void GraphicsEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_sourceNode || !m_targetNode) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制连接线
    drawLine(painter);

    // 绘制箭头
    QPointF sourcePoint = calculateSourcePoint();
    QPointF targetPoint = calculateTargetPoint();
    drawArrow(painter, sourcePoint, targetPoint);

    // 绘制标签
    if (!m_edge.label().isEmpty()) {
        drawLabel(painter);
    }

    // 绘制选中效果
    if (isSelectedEdge() || m_isHovered) {
        drawSelection(painter);
    }
}

/**
 * @brief 返回图形项的类型
 * @return 类型标识符
 */
int GraphicsEdge::type() const
{
    return Type;
}

/**
 * @brief 设置连接线标签
 * @param label 新的标签
 */
void GraphicsEdge::setLabel(const QString& label)
{
    m_edge.setLabel(label);
    update();
}

/**
 * @brief 设置连接线颜色
 * @param color 新的颜色
 */
void GraphicsEdge::setColor(const QColor& color)
{
    m_edge.setColor(color);
    update();
}

/**
 * @brief 设置连接线选中状态
 * @param selected 选中状态
 */
void GraphicsEdge::setSelectedEdge(bool selected)
{
    m_isSelected = selected;
    update();
}

/**
 * @brief 设置源图形节点
 * @param node 源图形节点
 */
void GraphicsEdge::setSourceNode(GraphicsNode* node)
{
    if (m_sourceNode) {
        disconnect(m_sourceNode, &GraphicsNode::positionChanged, this, &GraphicsEdge::updatePosition);
    }
    m_sourceNode = node;
    if (m_sourceNode) {
        connect(m_sourceNode, &GraphicsNode::positionChanged, this, &GraphicsEdge::updatePosition);
    }
    updatePosition();
}

/**
 * @brief 设置目标图形节点
 * @param node 目标图形节点
 */
void GraphicsEdge::setTargetNode(GraphicsNode* node)
{
    if (m_targetNode) {
        disconnect(m_targetNode, &GraphicsNode::positionChanged, this, &GraphicsEdge::updatePosition);
    }
    m_targetNode = node;
    if (m_targetNode) {
        connect(m_targetNode, &GraphicsNode::positionChanged, this, &GraphicsEdge::updatePosition);
    }
    updatePosition();
}

/**
 * @brief 更新连接线数据
 * @param edge 新的连接线数据
 */
void GraphicsEdge::updateEdge(const ConceptEdge& edge)
{
    m_edge = edge;
    update();
}

/**
 * @brief 更新连接线位置
 */
void GraphicsEdge::updatePosition()
{
    prepareGeometryChange();
    update();
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件
 */
void GraphicsEdge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        setSelectedEdge(true);
    }
    QGraphicsItem::mousePressEvent(event);
}

/**
 * @brief 鼠标悬停进入事件处理
 * @param event 悬停事件
 */
void GraphicsEdge::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    m_isHovered = true;
    update();
}

/**
 * @brief 鼠标悬停离开事件处理
 * @param event 悬停事件
 */
void GraphicsEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    m_isHovered = false;
    update();
}

/**
 * @brief 计算直线与节点的交点
 * @param rect 节点边界矩形
 * @param center 节点中心
 * @param target 目标点
 * @param shape 节点形状
 * @return 交点坐标
 */
QPointF GraphicsEdge::calculateIntersectionPoint(const QRectF& rect, const QPointF& center, const QPointF& target, NodeShape shape) const
{
    switch (shape) {
    case NodeShape::Rectangle:
        return calculateRectangleIntersection(rect, center, target);
    case NodeShape::Ellipse:
        return calculateEllipseIntersection(rect, center, target);
    case NodeShape::RoundedRect:
        return calculateRoundedRectIntersection(rect, center, target);
    default:
        return center;
    }
}

/**
 * @brief 计算直线与矩形的交点
 * @param rect 矩形边界
 * @param center 矩形中心
 * @param target 目标点
 * @return 交点坐标
 */
QPointF GraphicsEdge::calculateRectangleIntersection(const QRectF& rect, const QPointF& center, const QPointF& target) const
{
    // 计算从中心到目标的直线
    QLineF line(center, target);

    // 计算直线与矩形的交点
    QPointF intersectionPoint;
    QLineF::IntersectionType intersection = line.intersects(QLineF(rect.topLeft(), rect.topRight()), &intersectionPoint);
    if (intersection == QLineF::BoundedIntersection) {
        return intersectionPoint;
    }

    intersection = line.intersects(QLineF(rect.topRight(), rect.bottomRight()), &intersectionPoint);
    if (intersection == QLineF::BoundedIntersection) {
        return intersectionPoint;
    }

    intersection = line.intersects(QLineF(rect.bottomRight(), rect.bottomLeft()), &intersectionPoint);
    if (intersection == QLineF::BoundedIntersection) {
        return intersectionPoint;
    }

    intersection = line.intersects(QLineF(rect.bottomLeft(), rect.topLeft()), &intersectionPoint);
    if (intersection == QLineF::BoundedIntersection) {
        return intersectionPoint;
    }

    // 如果没有交点，返回中心点
    return center;
}

/**
 * @brief 计算直线与椭圆的交点
 * @param rect 椭圆的外切矩形
 * @param center 椭圆中心
 * @param target 目标点
 * @return 交点坐标
 */
QPointF GraphicsEdge::calculateEllipseIntersection(const QRectF& rect, const QPointF& center, const QPointF& target) const
{
    // 计算椭圆的半长轴和半短轴
    qreal a = rect.width() / 2.0;
    qreal b = rect.height() / 2.0;

    // 如果椭圆退化为直线或点，返回中心
    if (a <= 0 || b <= 0) {
        return center;
    }

    // 计算直线方向向量
    qreal dx = target.x() - center.x();
    qreal dy = target.y() - center.y();

    // 如果目标点与中心重合，返回中心
    if (dx == 0 && dy == 0) {
        return center;
    }

    // 计算直线参数方程：x = center.x() + t*dx, y = center.y() + t*dy
    // 椭圆方程：(x-center.x())²/a² + (y-center.y())²/b² = 1
    // 代入得：(t*dx)²/a² + (t*dy)²/b² = 1
    // 整理得：t²*(dx²/a² + dy²/b²) = 1
    // 解得：t = 1/sqrt(dx²/a² + dy²/b²)
    qreal t = 1.0 / sqrt((dx*dx)/(a*a) + (dy*dy)/(b*b));

    // 计算交点坐标
    return QPointF(center.x() + t*dx, center.y() + t*dy);
}

/**
 * @brief 计算直线与圆角矩形的交点
 * @param rect 圆角矩形的外切矩形
 * @param center 圆角矩形中心
 * @param target 目标点
 * @return 交点坐标
 */
QPointF GraphicsEdge::calculateRoundedRectIntersection(const QRectF& rect, const QPointF& center, const QPointF& target) const
{
    // 圆角矩形的圆角半径（使用矩形宽度和高度的较小值的1/4）
    qreal radius = qMin(rect.width(), rect.height()) / 4.0;
    
    // 首先尝试与矩形部分相交
    QRectF rectPart = rect.adjusted(radius, radius, -radius, -radius);
    QPointF rectIntersection = calculateRectangleIntersection(rectPart, center, target);
    
    // 如果与矩形部分相交，直接返回交点
    if (rectPart.contains(rectIntersection)) {
        return rectIntersection;
    }
    
    // 否则，计算与圆角部分的交点
    // 这里使用简化算法：将圆角矩形视为矩形，返回与矩形的交点
    // 在实际应用中，可以实现更精确的圆角交点计算
    return calculateRectangleIntersection(rect, center, target);
}

/**
 * @brief 计算连接线起点
 * @return 起点坐标
 */
QPointF GraphicsEdge::calculateSourcePoint() const
{
    if (!m_sourceNode || !m_targetNode) {
        return QPointF();
    }

    // 计算源节点和目标节点的中心点
    QPointF sourceCenter = QPointF(m_sourceNode->x() + m_sourceNode->node().width() / 2.0,
                                  m_sourceNode->y() + m_sourceNode->node().height() / 2.0);
    QPointF targetCenter = QPointF(m_targetNode->x() + m_targetNode->node().width() / 2.0,
                                  m_targetNode->y() + m_targetNode->node().height() / 2.0);

    // 计算源节点的边界矩形
    QRectF sourceRect = QRectF(m_sourceNode->x(), m_sourceNode->y(),
                              m_sourceNode->node().width(), m_sourceNode->node().height());

    // 使用公共方法计算交点，考虑节点形状
    return calculateIntersectionPoint(sourceRect, sourceCenter, targetCenter, m_sourceNode->node().shape());
}

/**
 * @brief 计算连接线终点
 * @return 终点坐标
 */
QPointF GraphicsEdge::calculateTargetPoint() const
{
    if (!m_sourceNode || !m_targetNode) {
        return QPointF();
    }

    // 计算源节点和目标节点的中心点
    QPointF sourceCenter = QPointF(m_sourceNode->x() + m_sourceNode->node().width() / 2.0,
                                  m_sourceNode->y() + m_sourceNode->node().height() / 2.0);
    QPointF targetCenter = QPointF(m_targetNode->x() + m_targetNode->node().width() / 2.0,
                                  m_targetNode->y() + m_targetNode->node().height() / 2.0);

    // 计算目标节点的边界矩形
    QRectF targetRect = QRectF(m_targetNode->x(), m_targetNode->y(),
                              m_targetNode->node().width(), m_targetNode->node().height());

    // 使用公共方法计算交点，考虑节点形状
    return calculateIntersectionPoint(targetRect, targetCenter, sourceCenter, m_targetNode->node().shape());
}

/**
 * @brief 绘制连接线
 * @param painter 绘制器
 */
void GraphicsEdge::drawLine(QPainter* painter)
{
    QPointF sourcePoint = calculateSourcePoint();
    QPointF targetPoint = calculateTargetPoint();

    QPen pen(m_edge.color(), m_lineWidth);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QPainterPath path;
    path.moveTo(sourcePoint);
    path.lineTo(targetPoint);
    painter->drawPath(path);
}

/**
 * @brief 绘制箭头
 * @param painter 绘制器
 * @param startPoint 起点
 * @param endPoint 终点
 */
void GraphicsEdge::drawArrow(QPainter* painter, const QPointF& startPoint, const QPointF& endPoint)
{
    // 计算箭头角度
    QLineF line(startPoint, endPoint);
    double angle = std::atan2(-line.dy(), line.dx());

    // 计算箭头顶点
    QPointF arrowP1 = endPoint - QPointF(std::sin(angle + M_PI / 3) * m_arrowSize,
                                          std::cos(angle + M_PI / 3) * m_arrowSize);
    QPointF arrowP2 = endPoint - QPointF(std::sin(angle + M_PI - M_PI / 3) * m_arrowSize,
                                          std::cos(angle + M_PI - M_PI / 3) * m_arrowSize);

    // 绘制箭头
    QPen pen(m_edge.color(), m_lineWidth);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(m_edge.color()));

    QPolygonF arrowPolygon;
    arrowPolygon << endPoint << arrowP1 << arrowP2;
    painter->drawPolygon(arrowPolygon);
}

/**
 * @brief 绘制标签
 * @param painter 绘制器
 */
void GraphicsEdge::drawLabel(QPainter* painter)
{
    QPointF sourcePoint = calculateSourcePoint();
    QPointF targetPoint = calculateTargetPoint();

    // 计算标签位置（连接线中点）
    QPointF labelPos = (sourcePoint + targetPoint) / 2.0;

    // 设置字体
    QFont font("Arial", 9);
    painter->setFont(font);

    // 计算标签边界矩形
    QFontMetrics metrics(font);
    QRectF labelRect = metrics.boundingRect(m_edge.label());
    labelRect.moveCenter(labelPos);

    // 绘制标签背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 200));
    painter->drawRoundedRect(labelRect.adjusted(-3, -3, 3, 3), 3, 3);

    // 绘制标签文本
    painter->setPen(Qt::black);
    painter->drawText(labelRect, Qt::AlignCenter, m_edge.label());
}

/**
 * @brief 绘制选中效果
 * @param painter 绘制器
 */
void GraphicsEdge::drawSelection(QPainter* painter)
{
    QPointF sourcePoint = calculateSourcePoint();
    QPointF targetPoint = calculateTargetPoint();

    if (isSelectedEdge()) {
        // 绘制选中边框
        QPen pen(m_selectionColor, m_lineWidth + 2.0);
        pen.setStyle(Qt::DashLine);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        QPainterPath path;
        path.moveTo(sourcePoint);
        path.lineTo(targetPoint);
        painter->drawPath(path);
    } else if (m_isHovered) {
        // 绘制悬停效果
        QPen pen(m_hoverColor, m_lineWidth + 1.0);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        QPainterPath path;
        path.moveTo(sourcePoint);
        path.lineTo(targetPoint);
        painter->drawPath(path);
    }
}
