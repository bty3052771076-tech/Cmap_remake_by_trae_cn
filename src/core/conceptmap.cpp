#include "conceptmap.h"

/**
 * @brief 构造函数 - 创建一个空的概念图
 */
ConceptMap::ConceptMap()
    : m_name("未命名概念图")
    , m_version("1.0")
{
}

/**
 * @brief 构造函数 - 创建一个带有指定名称的概念图
 * @param name 概念图名称
 */
ConceptMap::ConceptMap(const QString& name)
    : m_name(name)
    , m_version("1.0")
{
}

/**
 * @brief 析构函数
 */
ConceptMap::~ConceptMap()
{
    clear();
}

/**
 * @brief 根据ID获取节点
 * @param id 节点ID
 * @return 节点指针，如果不存在则返回 nullptr
 */
ConceptNode* ConceptMap::nodeById(const QString& id)
{
    if (!m_nodeIndexMap.contains(id)) {
        return nullptr;
    }
    return &m_nodes[m_nodeIndexMap[id]];
}

/**
 * @brief 根据ID获取节点（常量版本）
 * @param id 节点ID
 * @return 节点指针，如果不存在则返回 nullptr
 */
const ConceptNode* ConceptMap::nodeById(const QString& id) const
{
    if (!m_nodeIndexMap.contains(id)) {
        return nullptr;
    }
    return &m_nodes[m_nodeIndexMap[id]];
}

/**
 * @brief 根据ID获取连接线
 * @param id 连接线ID
 * @return 连接线指针，如果不存在则返回 nullptr
 */
ConceptEdge* ConceptMap::edgeById(const QString& id)
{
    if (!m_edgeIndexMap.contains(id)) {
        return nullptr;
    }
    return &m_edges[m_edgeIndexMap[id]];
}

/**
 * @brief 根据ID获取连接线（常量版本）
 * @param id 连接线ID
 * @return 连接线指针，如果不存在则返回 nullptr
 */
const ConceptEdge* ConceptMap::edgeById(const QString& id) const
{
    if (!m_edgeIndexMap.contains(id)) {
        return nullptr;
    }
    return &m_edges[m_edgeIndexMap[id]];
}

/**
 * @brief 获取与指定节点相关的所有连接线
 * @param nodeId 节点ID
 * @return 连接线列表
 */
QVector<ConceptEdge> ConceptMap::edgesByNodeId(const QString& nodeId) const
{
    QVector<ConceptEdge> result;
    for (const ConceptEdge& edge : m_edges) {
        if (edge.sourceNodeId() == nodeId || edge.targetNodeId() == nodeId) {
            result.append(edge);
        }
    }
    return result;
}

/**
 * @brief 检查节点是否存在
 * @param id 节点ID
 * @return 如果存在返回 true，否则返回 false
 */
bool ConceptMap::hasNode(const QString& id) const
{
    return m_nodeIndexMap.contains(id);
}

/**
 * @brief 检查连接线是否存在
 * @param id 连接线ID
 * @return 如果存在返回 true，否则返回 false
 */
bool ConceptMap::hasEdge(const QString& id) const
{
    return m_edgeIndexMap.contains(id);
}

/**
 * @brief 添加节点
 * @param node 要添加的节点
 * @return 如果成功添加返回 true，如果已存在则返回 false
 */
bool ConceptMap::addNode(const ConceptNode& node)
{
    if (hasNode(node.id())) {
        return false;
    }

    m_nodes.append(node);
    m_nodeIndexMap[node.id()] = m_nodes.size() - 1;
    return true;
}

/**
 * @brief 删除节点
 * @param id 要删除的节点ID
 * @return 如果成功删除返回 true，如果不存在则返回 false
 */
bool ConceptMap::removeNode(const QString& id)
{
    if (!hasNode(id)) {
        return false;
    }

    // 删除与该节点相关的所有连接线
    QVector<ConceptEdge> relatedEdges = edgesByNodeId(id);
    for (const ConceptEdge& edge : relatedEdges) {
        removeEdge(edge.id());
    }

    // 删除节点
    int index = m_nodeIndexMap[id];
    m_nodes.removeAt(index);

    // 使用增量更新索引映射，只更新受影响的索引
    for (auto it = m_nodeIndexMap.begin(); it != m_nodeIndexMap.end(); ++it) {
        if (it.value() > index) {
            --it.value();
        }
    }
    m_nodeIndexMap.remove(id);

    return true;
}

/**
 * @brief 更新节点
 * @param node 要更新的节点
 * @return 如果成功更新返回 true，如果不存在则返回 false
 */
bool ConceptMap::updateNode(const ConceptNode& node)
{
    if (!hasNode(node.id())) {
        return false;
    }

    int index = m_nodeIndexMap[node.id()];
    m_nodes[index] = node;
    return true;
}

/**
 * @brief 清空所有节点
 */
void ConceptMap::clearNodes()
{
    m_nodes.clear();
    m_nodeIndexMap.clear();
}

/**
 * @brief 添加连接线
 * @param edge 要添加的连接线
 * @return 如果成功添加返回 true，如果已存在则返回 false
 */
bool ConceptMap::addEdge(const ConceptEdge& edge)
{
    if (hasEdge(edge.id())) {
        return false;
    }

    // 检查源节点和目标节点是否存在
    if (!hasNode(edge.sourceNodeId()) || !hasNode(edge.targetNodeId())) {
        return false;
    }

    m_edges.append(edge);
    m_edgeIndexMap[edge.id()] = m_edges.size() - 1;
    return true;
}

/**
 * @brief 删除连接线
 * @param id 要删除的连接线ID
 * @return 如果成功删除返回 true，如果不存在则返回 false
 */
bool ConceptMap::removeEdge(const QString& id)
{
    if (!hasEdge(id)) {
        return false;
    }

    int index = m_edgeIndexMap[id];
    m_edges.removeAt(index);

    // 更新索引映射
    m_edgeIndexMap.clear();
    for (int i = 0; i < m_edges.size(); ++i) {
        m_edgeIndexMap[m_edges[i].id()] = i;
    }

    return true;
}

/**
 * @brief 更新连接线
 * @param edge 要更新的连接线
 * @return 如果成功更新返回 true，如果不存在则返回 false
 */
bool ConceptMap::updateEdge(const ConceptEdge& edge)
{
    if (!hasEdge(edge.id())) {
        return false;
    }

    int index = m_edgeIndexMap[edge.id()];
    m_edges[index] = edge;
    return true;
}

/**
 * @brief 清空所有连接线
 */
void ConceptMap::clearEdges()
{
    m_edges.clear();
    m_edgeIndexMap.clear();
}

/**
 * @brief 清空所有节点和连接线
 */
void ConceptMap::clear()
{
    clearNodes();
    clearEdges();
}
