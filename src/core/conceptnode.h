#ifndef CONCEPTNODE_H
#define CONCEPTNODE_H

#include <QString>
#include <QColor>
#include <QSize>
#include <QPointF>
#include <QUuid>

/**
 * @brief 节点形状枚举
 */
enum class NodeShape {
    Rectangle,      // 矩形
    Ellipse,        // 椭圆
    RoundedRect     // 圆角矩形
};

/**
 * @brief 概念节点数据类
 *
 * 该类用于存储概念节点的所有数据信息，包括：
 * - 节点唯一标识符
 * - 节点文本内容
 * - 节点位置
 * - 节点尺寸
 * - 节点颜色
 * - 节点形状
 * - 节点样式
 */
class ConceptNode
{
public:
    /**
     * @brief 构造函数 - 创建一个空的概念节点
     */
    ConceptNode();

    /**
     * @brief 构造函数 - 创建一个带有指定参数的概念节点
     * @param text 节点文本内容
     * @param x 节点X坐标
     * @param y 节点Y坐标
     * @param width 节点宽度
     * @param height 节点高度
     * @param color 节点颜色
     */
    ConceptNode(const QString& text, qreal x, qreal y, qreal width, qreal height, const QColor& color = QColor(200, 220, 240));

    /**
     * @brief 构造函数 - 创建一个带有指定参数的概念节点
     * @param text 节点文本内容
     * @param x 节点X坐标
     * @param y 节点Y坐标
     * @param width 节点宽度
     * @param height 节点高度
     * @param color 节点颜色
     * @param shape 节点形状
     */
    ConceptNode(const QString& text, qreal x, qreal y, qreal width, qreal height, const QColor& color, NodeShape shape);

    /**
     * @brief 析构函数
     */
    ~ConceptNode();

    // Getter 方法
    /**
     * @brief 获取节点唯一标识符
     * @return 节点ID
     */
    QString id() const { return m_id; }

    /**
     * @brief 获取节点文本内容
     * @return 节点文本
     */
    QString text() const { return m_text; }

    /**
     * @brief 获取节点X坐标
     * @return X坐标
     */
    qreal x() const { return m_x; }

    /**
     * @brief 获取节点Y坐标
     * @return Y坐标
     */
    qreal y() const { return m_y; }

    /**
     * @brief 获取节点位置
     * @return 节点位置点
     */
    QPointF pos() const { return QPointF(m_x, m_y); }

    /**
     * @brief 获取节点宽度
     * @return 节点宽度
     */
    qreal width() const { return m_width; }

    /**
     * @brief 获取节点高度
     * @return 节点高度
     */
    qreal height() const { return m_height; }

    /**
     * @brief 获取节点尺寸
     * @return 节点尺寸
     */
    QSizeF size() const { return QSizeF(m_width, m_height); }

    /**
     * @brief 获取节点颜色
     * @return 节点颜色
     */
    QColor color() const { return m_color; }

    /**
     * @brief 获取节点样式
     * @return 节点样式
     */
    QString style() const { return m_style; }

    /**
     * @brief 获取节点形状
     * @return 节点形状
     */
    NodeShape shape() const { return m_shape; }

    // Setter 方法
    /**
     * @brief 设置节点文本内容
     * @param text 新的文本内容
     */
    void setText(const QString& text) { m_text = text; }

    /**
     * @brief 设置节点X坐标
     * @param x 新的X坐标
     */
    void setX(qreal x) { m_x = x; }

    /**
     * @brief 设置节点Y坐标
     * @param y 新的Y坐标
     */
    void setY(qreal y) { m_y = y; }

    /**
     * @brief 设置节点位置
     * @param pos 新的位置点
     */
    void setPos(const QPointF& pos) { m_x = pos.x(); m_y = pos.y(); }

    /**
     * @brief 设置节点宽度
     * @param width 新的宽度
     */
    void setWidth(qreal width) { m_width = width; }

    /**
     * @brief 设置节点高度
     * @param height 新的高度
     */
    void setHeight(qreal height) { m_height = height; }

    /**
     * @brief 设置节点尺寸
     * @param size 新的尺寸
     */
    void setSize(const QSizeF& size) { m_width = size.width(); m_height = size.height(); }

    /**
     * @brief 设置节点颜色
     * @param color 新的颜色
     */
    void setColor(const QColor& color) { m_color = color; }

    /**
     * @brief 设置节点样式
     * @param style 新的样式
     */
    void setStyle(const QString& style) { m_style = style; }

    /**
     * @brief 设置节点形状
     * @param shape 新的形状
     */
    void setShape(NodeShape shape) { m_shape = shape; }

    /**
     * @brief 设置节点ID
     * @param id 新的节点ID
     */
    void setId(const QString& id) { m_id = id; }

    /**
     * @brief 生成新的唯一标识符
     * @return 新的UUID字符串
     */
    static QString generateId();

private:
    QString m_id;          // 节点唯一标识符
    QString m_text;        // 节点文本内容
    qreal m_x;             // 节点X坐标
    qreal m_y;             // 节点Y坐标
    qreal m_width;         // 节点宽度
    qreal m_height;        // 节点高度
    QColor m_color;        // 节点颜色
    QString m_style;       // 节点样式
    NodeShape m_shape;      // 节点形状
};

#endif // CONCEPTNODE_H
