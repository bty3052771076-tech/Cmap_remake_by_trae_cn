#include "mapmodel.h"
#include "conceptmapserializer.h"
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个概念图模型
 * @param parent 父对象
 */
MapModel::MapModel(QObject* parent)
    : QObject(parent)
{
    // 连接节点模型的信号
    connect(&m_nodeModel, &NodeModel::nodeAdded, this, &MapModel::nodeAdded);
    connect(&m_nodeModel, &NodeModel::nodeRemoved, this, &MapModel::nodeRemoved);
    connect(&m_nodeModel, &NodeModel::nodeUpdated, this, &MapModel::nodeUpdated);

    // 连接连接线模型的信号
    connect(&m_edgeModel, &EdgeModel::edgeAdded, this, &MapModel::edgeAdded);
    connect(&m_edgeModel, &EdgeModel::edgeRemoved, this, &MapModel::edgeRemoved);
    connect(&m_edgeModel, &EdgeModel::edgeUpdated, this, &MapModel::edgeUpdated);

    // 创建新概念图
    newMap();
}

/**
 * @brief 析构函数
 */
MapModel::~MapModel()
{
}

/**
 * @brief 设置概念图数据
 * @param map 概念图数据
 */
void MapModel::setConceptMap(const ConceptMap& map)
{
    m_conceptMap = map;

    // 更新节点模型
    m_nodeModel.setNodes(map.nodes());

    // 更新连接线模型
    m_edgeModel.setEdges(map.edges());

    emit mapChanged();
}

/**
 * @brief 设置概念图名称
 * @param name 新的名称
 */
void MapModel::setName(const QString& name)
{
    m_conceptMap.setName(name);
    emit mapChanged();
}

/**
 * @brief 设置概念图版本
 * @param version 新的版本
 */
void MapModel::setVersion(const QString& version)
{
    m_conceptMap.setVersion(version);
    emit mapChanged();
}

/**
 * @brief 添加节点
 * @param node 要添加的节点
 * @return 如果成功添加返回 true，否则返回 false
 */
bool MapModel::addNode(const ConceptNode& node)
{
    if (!m_conceptMap.addNode(node)) {
        return false;
    }

    m_nodeModel.addNode(node);
    emit mapChanged();

    return true;
}

/**
 * @brief 删除节点
 * @param nodeId 要删除的节点ID
 * @return 如果成功删除返回 true，否则返回 false
 */
bool MapModel::removeNode(const QString& nodeId)
{
    if (!m_conceptMap.removeNode(nodeId)) {
        return false;
    }

    // 从节点模型中删除
    QModelIndex index = m_nodeModel.findIndexById(nodeId);
    if (index.isValid()) {
        m_nodeModel.removeNode(index);
    }

    // 从连接线模型中删除相关的连接线
    QVector<ConceptEdge> relatedEdges = m_conceptMap.edgesByNodeId(nodeId);
    for (const ConceptEdge& edge : relatedEdges) {
        QModelIndex edgeIndex = m_edgeModel.findIndexById(edge.id());
        if (edgeIndex.isValid()) {
            m_edgeModel.removeEdge(edgeIndex);
        }
    }

    emit mapChanged();

    return true;
}

/**
 * @brief 更新节点
 * @param node 要更新的节点
 * @return 如果成功更新返回 true，否则返回 false
 */
bool MapModel::updateNode(const ConceptNode& node)
{
    if (!m_conceptMap.updateNode(node)) {
        return false;
    }

    QModelIndex index = m_nodeModel.findIndexById(node.id());
    if (index.isValid()) {
        m_nodeModel.updateNode(index, node);
    }

    emit mapChanged();

    return true;
}

/**
 * @brief 根据ID获取节点
 * @param id 节点ID
 * @return 节点指针，如果不存在则返回 nullptr
 */
ConceptNode* MapModel::nodeById(const QString& id)
{
    return m_conceptMap.nodeById(id);
}

/**
 * @brief 根据ID获取节点（常量版本）
 * @param id 节点ID
 * @return 节点指针，如果不存在则返回 nullptr
 */
const ConceptNode* MapModel::nodeById(const QString& id) const
{
    return m_conceptMap.nodeById(id);
}

/**
 * @brief 添加连接线
 * @param edge 要添加的连接线
 * @return 如果成功添加返回 true，否则返回 false
 */
bool MapModel::addEdge(const ConceptEdge& edge)
{
    if (!m_conceptMap.addEdge(edge)) {
        return false;
    }

    m_edgeModel.addEdge(edge);
    emit mapChanged();

    return true;
}

/**
 * @brief 删除连接线
 * @param edgeId 要删除的连接线ID
 * @return 如果成功删除返回 true，否则返回 false
 */
bool MapModel::removeEdge(const QString& edgeId)
{
    if (!m_conceptMap.removeEdge(edgeId)) {
        return false;
    }

    QModelIndex index = m_edgeModel.findIndexById(edgeId);
    if (index.isValid()) {
        m_edgeModel.removeEdge(index);
    }

    emit mapChanged();

    return true;
}

/**
 * @brief 更新连接线
 * @param edge 要更新的连接线
 * @return 如果成功更新返回 true，否则返回 false
 */
bool MapModel::updateEdge(const ConceptEdge& edge)
{
    if (!m_conceptMap.updateEdge(edge)) {
        return false;
    }

    QModelIndex index = m_edgeModel.findIndexById(edge.id());
    if (index.isValid()) {
        m_edgeModel.updateEdge(index, edge);
    }

    emit mapChanged();

    return true;
}

/**
 * @brief 根据ID获取连接线
 * @param id 连接线ID
 * @return 连接线指针，如果不存在则返回 nullptr
 */
ConceptEdge* MapModel::edgeById(const QString& id)
{
    return m_conceptMap.edgeById(id);
}

/**
 * @brief 根据ID获取连接线（常量版本）
 * @param id 连接线ID
 * @return 连接线指针，如果不存在则返回 nullptr
 */
const ConceptEdge* MapModel::edgeById(const QString& id) const
{
    return m_conceptMap.edgeById(id);
}

/**
 * @brief 清空概念图
 */
void MapModel::clear()
{
    m_conceptMap.clear();
    m_nodeModel.clear();
    m_edgeModel.clear();

    emit mapChanged();
}

/**
 * @brief 创建新概念图
 * @param name 概念图名称
 */
void MapModel::newMap(const QString& name)
{
    clear();
    m_conceptMap.setName(name);
    m_conceptMap.setVersion("1.0");

    emit mapChanged();
}

/**
 * @brief 从文件加载概念图
 * @param filePath 文件路径
 * @return 如果成功加载返回 true，否则返回 false
 */
bool MapModel::loadFromFile(const QString& filePath)
{
    // 根据文件扩展名选择加载方式
    if (filePath.endsWith(".json", Qt::CaseInsensitive)) {
        if (!ConceptMapSerializer::loadFromJson(filePath, m_conceptMap)) {
            return false;
        }
    } else if (filePath.endsWith(".xml", Qt::CaseInsensitive)) {
        if (!ConceptMapSerializer::loadFromXml(filePath, m_conceptMap)) {
            return false;
        }
    } else {
        qWarning() << "不支持的文件格式:" << filePath;
        return false;
    }

    // 更新节点模型和连接线模型
    m_nodeModel.setNodes(m_conceptMap.nodes());
    m_edgeModel.setEdges(m_conceptMap.edges());

    emit mapChanged();
    return true;
}

/**
 * @brief 保存概念图到文件
 * @param filePath 文件路径
 * @return 如果成功保存返回 true，否则返回 false
 */
bool MapModel::saveToFile(const QString& filePath)
{
    // 根据文件扩展名选择保存方式
    if (filePath.endsWith(".json", Qt::CaseInsensitive)) {
        return ConceptMapSerializer::saveToJson(filePath, m_conceptMap);
    } else if (filePath.endsWith(".xml", Qt::CaseInsensitive)) {
        return ConceptMapSerializer::saveToXml(filePath, m_conceptMap);
    } else {
        qWarning() << "不支持的文件格式:" << filePath;
        return false;
    }
}
