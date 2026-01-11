#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QObject>
#include "conceptmap.h"
#include "nodemodel.h"
#include "edgemodel.h"

/**
 * @brief 概念图模型类
 *
 * 该类用于管理整个概念图的数据模型，包括：
 * - 节点和连接线的统一管理
 * - 数据同步
 * - 文件操作
 */
class MapModel : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个概念图模型
     * @param parent 父对象
     */
    explicit MapModel(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MapModel();

    // Getter 方法
    /**
     * @brief 获取概念图数据
     * @return 概念图数据
     */
    ConceptMap conceptMap() const { return m_conceptMap; }

    /**
     * @brief 获取节点模型
     * @return 节点模型
     */
    NodeModel* nodeModel() { return &m_nodeModel; }

    /**
     * @brief 获取连接线模型
     * @return 连接线模型
     */
    EdgeModel* edgeModel() { return &m_edgeModel; }

    /**
     * @brief 获取概念图名称
     * @return 概念图名称
     */
    QString name() const { return m_conceptMap.name(); }

    /**
     * @brief 获取概念图版本
     * @return 概念图版本
     */
    QString version() const { return m_conceptMap.version(); }

    /**
     * @brief 获取节点数量
     * @return 节点数量
     */
    int nodeCount() const { return m_conceptMap.nodeCount(); }

    /**
     * @brief 获取连接线数量
     * @return 连接线数量
     */
    int edgeCount() const { return m_conceptMap.edgeCount(); }

    /**
     * @brief 检查概念图是否为空
     * @return 如果为空返回 true，否则返回 false
     */
    bool isEmpty() const { return m_conceptMap.isEmpty(); }

    // Setter 方法
    /**
     * @brief 设置概念图数据
     * @param map 概念图数据
     */
    void setConceptMap(const ConceptMap& map);

    /**
     * @brief 设置概念图名称
     * @param name 新的名称
     */
    void setName(const QString& name);

    /**
     * @brief 设置概念图版本
     * @param version 新的版本
     */
    void setVersion(const QString& version);

    // 节点操作
    /**
     * @brief 添加节点
     * @param node 要添加的节点
     * @return 如果成功添加返回 true，否则返回 false
     */
    bool addNode(const ConceptNode& node);

    /**
     * @brief 删除节点
     * @param nodeId 要删除的节点ID
     * @return 如果成功删除返回 true，否则返回 false
     */
    bool removeNode(const QString& nodeId);

    /**
     * @brief 更新节点
     * @param node 要更新的节点
     * @return 如果成功更新返回 true，否则返回 false
     */
    bool updateNode(const ConceptNode& node);

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

    // 连接线操作
    /**
     * @brief 添加连接线
     * @param edge 要添加的连接线
     * @return 如果成功添加返回 true，否则返回 false
     */
    bool addEdge(const ConceptEdge& edge);

    /**
     * @brief 删除连接线
     * @param edgeId 要删除的连接线ID
     * @return 如果成功删除返回 true，否则返回 false
     */
    bool removeEdge(const QString& edgeId);

    /**
     * @brief 更新连接线
     * @param edge 要更新的连接线
     * @return 如果成功更新返回 true，否则返回 false
     */
    bool updateEdge(const ConceptEdge& edge);

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
     * @brief 清空概念图
     */
    void clear();

    /**
     * @brief 创建新概念图
     * @param name 概念图名称
     */
    void newMap(const QString& name = "未命名概念图");

    /**
     * @brief 从文件加载概念图
     * @param filePath 文件路径
     * @return 如果成功加载返回 true，否则返回 false
     */
    bool loadFromFile(const QString& filePath);

    /**
     * @brief 保存概念图到文件
     * @param filePath 文件路径
     * @return 如果成功保存返回 true，否则返回 false
     */
    bool saveToFile(const QString& filePath);

signals:
    /**
     * @brief 概念图变化信号
     */
    void mapChanged();

    /**
     * @brief 节点添加信号
     * @param node 添加的节点
     */
    void nodeAdded(const ConceptNode& node);

    /**
     * @brief 节点删除信号
     * @param nodeId 删除的节点ID
     */
    void nodeRemoved(const QString& nodeId);

    /**
     * @brief 节点更新信号
     * @param node 更新的节点
     */
    void nodeUpdated(const ConceptNode& node);

    /**
     * @brief 连接线添加信号
     * @param edge 添加的连接线
     */
    void edgeAdded(const ConceptEdge& edge);

    /**
     * @brief 连接线删除信号
     * @param edgeId 删除的连接线ID
     */
    void edgeRemoved(const QString& edgeId);

    /**
     * @brief 连接线更新信号
     * @param edge 更新的连接线
     */
    void edgeUpdated(const ConceptEdge& edge);

private:
    ConceptMap m_conceptMap;    // 概念图数据
    NodeModel m_nodeModel;      // 节点模型
    EdgeModel m_edgeModel;      // 连接线模型
};

#endif // MAPMODEL_H
