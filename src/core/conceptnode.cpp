#include "conceptnode.h"
#include <QUuid>

/**
 * @brief 构造函数 - 创建一个空的概念节点
 */
ConceptNode::ConceptNode()
    : m_id(generateId())
    , m_text("新节点")
    , m_x(0.0)
    , m_y(0.0)
    , m_width(120.0)
    , m_height(60.0)
    , m_color(200, 220, 240)
    , m_style("default")
{
}

/**
 * @brief 构造函数 - 创建一个带有指定参数的概念节点
 * @param text 节点文本内容
 * @param x 节点X坐标
 * @param y 节点Y坐标
 * @param width 节点宽度
 * @param height 节点高度
 * @param color 节点颜色
 */
ConceptNode::ConceptNode(const QString& text, qreal x, qreal y, qreal width, qreal height, const QColor& color)
    : m_id(generateId())
    , m_text(text)
    , m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_color(color)
    , m_style("default")
    , m_shape(NodeShape::Rectangle)
{
}

/**
 * @brief 构造函数 - 创建一个带有指定参数的概念节点（包含形状）
 * @param text 节点文本内容
 * @param x 节点X坐标
 * @param y 节点Y坐标
 * @param width 节点宽度
 * @param height 节点高度
 * @param color 节点颜色
 * @param shape 节点形状
 */
ConceptNode::ConceptNode(const QString& text, qreal x, qreal y, qreal width, qreal height, const QColor& color, NodeShape shape)
    : m_id(generateId())
    , m_text(text)
    , m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_color(color)
    , m_style("default")
    , m_shape(shape)
{
}

/**
 * @brief 析构函数
 */
ConceptNode::~ConceptNode()
{
}

/**
 * @brief 生成新的唯一标识符
 * @return 新的UUID字符串
 */
QString ConceptNode::generateId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
