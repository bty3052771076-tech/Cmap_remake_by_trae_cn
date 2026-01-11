#ifndef CONCEPTEDGE_H
#define CONCEPTEDGE_H

#include <QString>
#include <QColor>
#include <QUuid>

/**
 * @brief 连接线数据类
 *
 * 该类用于存储概念图中连接线的数据信息，包括：
 * - 连接线唯一标识符
 * - 源节点ID
 * - 目标节点ID
 * - 连接线标签
 * - 连接线颜色
 * - 连接线样式
 */
class ConceptEdge
{
public:
    /**
     * @brief 构造函数 - 创建一个空的连接线
     */
    ConceptEdge();

    /**
     * @brief 构造函数 - 创建一个带有指定参数的连接线
     * @param sourceNodeId 源节点ID
     * @param targetNodeId 目标节点ID
     * @param label 连接线标签
     * @param color 连接线颜色
     */
    ConceptEdge(const QString& sourceNodeId, const QString& targetNodeId,
                const QString& label = "", const QColor& color = QColor(100, 100, 100));

    /**
     * @brief 析构函数
     */
    ~ConceptEdge();

    // Getter 方法
    /**
     * @brief 获取连接线唯一标识符
     * @return 连接线ID
     */
    QString id() const { return m_id; }

    /**
     * @brief 获取源节点ID
     * @return 源节点ID
     */
    QString sourceNodeId() const { return m_sourceNodeId; }

    /**
     * @brief 获取目标节点ID
     * @return 目标节点ID
     */
    QString targetNodeId() const { return m_targetNodeId; }

    /**
     * @brief 获取连接线标签
     * @return 连接线标签
     */
    QString label() const { return m_label; }

    /**
     * @brief 获取连接线颜色
     * @return 连接线颜色
     */
    QColor color() const { return m_color; }

    /**
     * @brief 获取连接线样式
     * @return 连接线样式
     */
    QString style() const { return m_style; }

    // Setter 方法
    /**
     * @brief 设置源节点ID
     * @param sourceNodeId 新的源节点ID
     */
    void setSourceNodeId(const QString& sourceNodeId) { m_sourceNodeId = sourceNodeId; }

    /**
     * @brief 设置目标节点ID
     * @param targetNodeId 新的目标节点ID
     */
    void setTargetNodeId(const QString& targetNodeId) { m_targetNodeId = targetNodeId; }

    /**
     * @brief 设置连接线标签
     * @param label 新的标签
     */
    void setLabel(const QString& label) { m_label = label; }

    /**
     * @brief 设置连接线颜色
     * @param color 新的颜色
     */
    void setColor(const QColor& color) { m_color = color; }

    /**
     * @brief 设置连接线样式
     * @param style 新的样式
     */
    void setStyle(const QString& style) { m_style = style; }

    /**
     * @brief 设置连接线ID
     * @param id 新的连接线ID
     */
    void setId(const QString& id) { m_id = id; }

    /**
     * @brief 生成新的唯一标识符
     * @return 新的UUID字符串
     */
    static QString generateId();

private:
    QString m_id;              // 连接线唯一标识符
    QString m_sourceNodeId;    // 源节点ID
    QString m_targetNodeId;    // 目标节点ID
    QString m_label;           // 连接线标签
    QColor m_color;            // 连接线颜色
    QString m_style;           // 连接线样式
};

#endif // CONCEPTEDGE_H
