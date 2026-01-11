#include "graphicsnode.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个图形节点
 * @param node 概念节点数据
 * @param parent 父图形项
 */
GraphicsNode::GraphicsNode(const ConceptNode& node, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , m_node(node)
    , m_isSelected(false)
    , m_isHovered(false)
    , m_cornerRadius(8.0)
    , m_borderWidth(2.0)
    , m_borderColor(QColor(80, 80, 80))
    , m_selectionColor(QColor(0, 120, 215))
    , m_hoverColor(QColor(0, 120, 215, 100))
{
    // 设置节点位置
    setPos(node.pos());

    // 启用鼠标悬停事件
    setAcceptHoverEvents(true);

    // 设置标志
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

/**
 * @brief 析构函数
 */
GraphicsNode::~GraphicsNode()
{
}

/**
 * @brief 返回图形项的边界矩形
 * @return 边界矩形
 */
QRectF GraphicsNode::boundingRect() const
{
    // 返回包含节点、边框和选中效果的边界矩形
    qreal padding = m_borderWidth + 5.0;
    return QRectF(-padding, -padding,
                  m_node.width() + 2 * padding,
                  m_node.height() + 2 * padding);
}

/**
 * @brief 绘制图形项
 * @param painter 绘制器
 * @param option 样式选项
 * @param widget 父窗口部件
 */
void GraphicsNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制节点背景
    drawBackground(painter);

    // 绘制节点边框
    drawBorder(painter);

    // 绘制节点文本
    drawText(painter);

    // 绘制选中效果
    if (isSelectedNode() || m_isHovered) {
        drawSelection(painter);
    }
}

/**
 * @brief 返回图形项的类型
 * @return 类型标识符
 */
int GraphicsNode::type() const
{
    return Type;
}

/**
 * @brief 设置节点文本
 * @param text 新的文本
 */
void GraphicsNode::setText(const QString& text)
{
    m_node.setText(text);
    update();
}

/**
 * @brief 设置节点颜色
 * @param color 新的颜色
 */
void GraphicsNode::setColor(const QColor& color)
{
    m_node.setColor(color);
    update();
}

/**
 * @brief 设置节点选中状态
 * @param selected 选中状态
 */
void GraphicsNode::setSelectedNode(bool selected)
{
    m_isSelected = selected;
    update();
}

/**
 * @brief 更新节点数据
 * @param node 新的节点数据
 */
void GraphicsNode::updateNode(const ConceptNode& node)
{
    m_node = node;
    setPos(node.pos());
    update();
}

/**
 * @brief 更新节点位置
 * @param pos 新的位置
 */
void GraphicsNode::updatePosition(const QPointF& pos)
{
    setPos(pos);
    m_node.setPos(pos);
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件
 */
void GraphicsNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
        setSelectedNode(true);
    }
    QGraphicsItem::mousePressEvent(event);
}

/**
 * @brief 鼠标移动事件处理
 * @param event 鼠标事件
 */
void GraphicsNode::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件
 */
void GraphicsNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

/**
 * @brief 鼠标悬停进入事件处理
 * @param event 悬停事件
 */
void GraphicsNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    m_isHovered = true;
    update();
}

/**
 * @brief 鼠标悬停离开事件处理
 * @param event 悬停事件
 */
void GraphicsNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    m_isHovered = false;
    update();
}

/**
 * @brief 项位置变化事件处理
 * @param change 变化类型
 * @param value 新值
 * @return 新值
 */
QVariant GraphicsNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && scene()) {
        // 更新节点数据中的位置
        m_node.setPos(value.toPointF());
    }
    if (change == ItemPositionHasChanged && scene()) {
        // 发射位置变化信号
        emit positionChanged(pos());
    }
    return QGraphicsItem::itemChange(change, value);
}

/**
 * @brief 计算文本边界矩形
 * @return 文本边界矩形
 */
QRectF GraphicsNode::calculateTextRect() const
{
    QFont font("Arial", 10);
    QFontMetrics metrics(font);
    QRectF textRect = metrics.boundingRect(m_node.text());

    // 居中文本
    qreal x = (m_node.width() - textRect.width()) / 2.0;
    qreal y = (m_node.height() - textRect.height()) / 2.0;

    return QRectF(x, y, textRect.width(), textRect.height());
}

/**
 * @brief 绘制节点背景
 * @param painter 绘制器
 */
void GraphicsNode::drawBackground(QPainter* painter)
{
    QRectF rect(0, 0, m_node.width(), m_node.height());

    // 创建渐变背景
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0.0, m_node.color().lighter(120));
    gradient.setColorAt(1.0, m_node.color());

    painter->setBrush(QBrush(gradient));
    painter->setPen(Qt::NoPen);

    // 根据节点形状绘制
    switch (m_node.shape()) {
        case NodeShape::Rectangle:
            painter->drawRect(rect);
            break;
        case NodeShape::Ellipse:
            painter->drawEllipse(rect);
            break;
        case NodeShape::RoundedRect:
            painter->drawRoundedRect(rect, m_cornerRadius, m_cornerRadius);
            break;
    }
}

/**
 * @brief 绘制节点边框
 * @param painter 绘制器
 */
void GraphicsNode::drawBorder(QPainter* painter)
{
    QRectF rect(0, 0, m_node.width(), m_node.height());

    QPen pen(m_borderColor, m_borderWidth);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rect, m_cornerRadius, m_cornerRadius);
}

/**
 * @brief 绘制节点文本
 * @param painter 绘制器
 */
void GraphicsNode::drawText(QPainter* painter)
{
    QFont font("Arial", 10);
    painter->setFont(font);

    QRectF textRect = calculateTextRect();

    // 设置文本颜色
    QColor textColor = (m_node.color().lightness() < 128) ? Qt::white : Qt::black;
    painter->setPen(textColor);

    // 绘制文本
    painter->drawText(textRect, Qt::AlignCenter, m_node.text());
}

/**
 * @brief 绘制选中效果
 * @param painter 绘制器
 */
void GraphicsNode::drawSelection(QPainter* painter)
{
    QRectF rect(0, 0, m_node.width(), m_node.height());

    if (isSelectedNode()) {
        // 绘制选中边框
        QPen pen(m_selectionColor, m_borderWidth + 1.0);
        pen.setStyle(Qt::DashLine);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rect.adjusted(-3, -3, 3, 3), m_cornerRadius + 1, m_cornerRadius + 1);
    } else if (m_isHovered) {
        // 绘制悬停效果
        QPen pen(m_hoverColor, m_borderWidth);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rect, m_cornerRadius, m_cornerRadius);
    }
}
