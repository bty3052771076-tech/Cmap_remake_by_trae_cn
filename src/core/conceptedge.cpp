#include "conceptedge.h"
#include <QUuid>

/**
 * @brief 构造函数 - 创建一个空的连接线
 */
ConceptEdge::ConceptEdge()
    : m_id(generateId())
    , m_sourceNodeId("")
    , m_targetNodeId("")
    , m_label("")
    , m_color(100, 100, 100)
    , m_style("default")
{
}

/**
 * @brief 构造函数 - 创建一个带有指定参数的连接线
 * @param sourceNodeId 源节点ID
 * @param targetNodeId 目标节点ID
 * @param label 连接线标签
 * @param color 连接线颜色
 */
ConceptEdge::ConceptEdge(const QString& sourceNodeId, const QString& targetNodeId,
                         const QString& label, const QColor& color)
    : m_id(generateId())
    , m_sourceNodeId(sourceNodeId)
    , m_targetNodeId(targetNodeId)
    , m_label(label)
    , m_color(color)
    , m_style("default")
{
}

/**
 * @brief 析构函数
 */
ConceptEdge::~ConceptEdge()
{
}

/**
 * @brief 生成新的唯一标识符
 * @return 新的UUID字符串
 */
QString ConceptEdge::generateId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
