#ifndef CONCEPTMAP_H
#define CONCEPTMAP_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QUuid>
#include "conceptnode.h"
#include "conceptedge.h"

/**
 * @brief 概念图数据类
 *
 * 该类用于管理整个概念图的数据，包括：
 * - 所有节点集合
 * - 所有连接线集合
 * - 概念图名称
 * - 概念图版本
 * - 节点和连接线的增删改查操作
 */
class ConceptMap
{
public:
    /**
     * @brief 构造函数 - 创建一个空的概念图
     */
    ConceptMap();

    /**
     * @brief 构造函数 - 创建一个带有指定名称的概念图
     * @param name 概念图名称
     */
    explicit ConceptMap(const QString& name);

    /**
     * @brief 析构函数
     */
    ~ConceptMap();

    // Getter 方法
    /**
     * @brief 获取概念图名称
     * @return 概念图名称
     */
    QString name() const { return m_name; }

    /**
     * @brief 获取概念图版本
     * @return 概念图版本
     */
    QString version() const { return m_version; }

    /**
     * @brief 获取所有节点
     * @return 节点列表
     */
    QVector<ConceptNode> nodes() const { return m_nodes; }

    /**
     * @brief 获取所有连接线
     * @return 连接线列表
     */
    QVector<ConceptEdge> edges() const { return m_edges; }

    /**
     * @brief 根据ID获取节点
     * @param id 节点ID
     * @return 节点指针，如果不存在则返回 nullptr
     */
    ConceptNode* nodeById(const QString& id);

    /**
     * @brief 根据ID获取节点（常量版本）
     * @param id 节点ID
     * @return 节点指针，如果不存在则返回 nullptr
     */
    const ConceptNode* nodeById(const QString& id) const;

    /**
     * @brief 根据ID获取连接线
     * @param id 连接线ID
     * @return 连接线指针，如果不存在则返回 nullptr
     */
    ConceptEdge* edgeById(const QString& id);

    /**
     * @brief 根据ID获取连接线（常量版本）
     * @param id 连接线ID
     * @return 连接线指针，如果不存在则返回 nullptr
     */
    const ConceptEdge* edgeById(const QString& id) const;

    /**
     * @brief 获取与指定节点相关的所有连接线
     * @param nodeId 节点ID
     * @return 连接线列表
     */
    QVector<ConceptEdge> edgesByNodeId(const QString& nodeId) const;

    /**
     * @brief 检查节点是否存在
     * @param id 节点ID
     * @return 如果存在返回 true，否则返回 false
     */
    bool hasNode(const QString& id) const;

    /**
     * @brief 检查连接线是否存在
     * @param id 连接线ID
     * @return 如果存在返回 true，否则返回 false
     */
    bool hasEdge(const QString& id) const;

    // Setter 方法
    /**
     * @brief 设置概念图名称
     * @param name 新的名称
     */
    void setName(const QString& name) { m_name = name; }

    /**
     * @brief 设置概念图版本
     * @param version 新的版本
     */
    void setVersion(const QString& version) { m_version = version; }

    // 节点操作
    /**
     * @brief 添加节点
     * @param node 要添加的节点
     * @return 如果成功添加返回 true，如果已存在则返回 false
     */
    bool addNode(const ConceptNode& node);

    /**
     * @brief 删除节点
     * @param id 要删除的节点ID
     * @return 如果成功删除返回 true，如果不存在则返回 false
     */
    bool removeNode(const QString& id);

    /**
     * @brief 更新节点
     * @param node 要更新的节点
     * @return 如果成功更新返回 true，如果不存在则返回 false
     */
    bool updateNode(const ConceptNode& node);

    /**
     * @brief 清空所有节点
     */
    void clearNodes();

    // 连接线操作
    /**
     * @brief 添加连接线
     * @param edge 要添加的连接线
     * @return 如果成功添加返回 true，如果已存在则返回 false
     */
    bool addEdge(const ConceptEdge& edge);

    /**
     * @brief 删除连接线
     * @param id 要删除的连接线ID
     * @return 如果成功删除返回 true，如果不存在则返回 false
     */
    bool removeEdge(const QString& id);

    /**
     * @brief 更新连接线
     * @param edge 要更新的连接线
     * @return 如果成功更新返回 true，如果不存在则返回 false
     */
    bool updateEdge(const ConceptEdge& edge);

    /**
     * @brief 清空所有连接线
     */
    void clearEdges();

    /**
     * @brief 清空所有节点和连接线
     */
    void clear();

    /**
     * @brief 获取节点数量
     * @return 节点数量
     */
    int nodeCount() const { return m_nodes.size(); }

    /**
     * @brief 获取连接线数量
     * @return 连接线数量
     */
    int edgeCount() const { return m_edges.size(); }

    /**
     * @brief 检查概念图是否为空
     * @return 如果为空返回 true，否则返回 false
     */
    bool isEmpty() const { return m_nodes.isEmpty() && m_edges.isEmpty(); }

private:
    QString m_name;                       // 概念图名称
    QString m_version;                    // 概念图版本
    QVector<ConceptNode> m_nodes;         // 节点列表
    QVector<ConceptEdge> m_edges;         // 连接线列表
    QMap<QString, int> m_nodeIndexMap;    // 节点ID到索引的映射
    QMap<QString, int> m_edgeIndexMap;    // 连接线ID到索引的映射
};

#endif // CONCEPTMAP_H
