#include "nodemodel.h"

/**
 * @brief 构造函数 - 创建一个节点模型
 * @param parent 父对象
 */
NodeModel::NodeModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

/**
 * @brief 析构函数
 */
NodeModel::~NodeModel()
{
}

/**
 * @brief 返回行数
 * @param parent 父索引
 * @return 行数
 */
int NodeModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_nodes.size();
}

/**
 * @brief 返回列数
 * @param parent 父索引
 * @return 列数
 */
int NodeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 6; // ID, 文本, X, Y, 宽度, 高度
}

/**
 * @brief 返回数据
 * @param index 模型索引
 * @param role 角色
 * @return 数据
 */
QVariant NodeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_nodes.size()) {
        return QVariant();
    }

    const ConceptNode& node = m_nodes[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return node.id();
            case 1: return node.text();
            case 2: return node.x();
            case 3: return node.y();
            case 4: return node.width();
            case 5: return node.height();
            default: return QVariant();
        }
    }

    if (role == Qt::BackgroundRole) {
        return QBrush(node.color());
    }

    return QVariant();
}

/**
 * @brief 设置数据
 * @param index 模型索引
 * @param value 值
 * @param role 角色
 * @return 如果成功设置返回 true，否则返回 false
 */
bool NodeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= m_nodes.size() || role != Qt::EditRole) {
        return false;
    }

    ConceptNode& node = m_nodes[index.row()];

    switch (index.column()) {
        case 1:
            node.setText(value.toString());
            break;
        case 2:
            node.setX(value.toReal());
            break;
        case 3:
            node.setY(value.toReal());
            break;
        case 4:
            node.setWidth(value.toReal());
            break;
        case 5:
            node.setHeight(value.toReal());
            break;
        default:
            return false;
    }

    emit dataChanged(index, index, {role});
    emit nodeUpdated(node);

    return true;
}

/**
 * @brief 返回表头数据
 * @param section 节
 * @param orientation 方向
 * @param role 角色
 * @return 表头数据
 */
QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "ID";
            case 1: return "文本";
            case 2: return "X";
            case 3: return "Y";
            case 4: return "宽度";
            case 5: return "高度";
            default: return QVariant();
        }
    }

    return QVariant();
}

/**
 * @brief 返回项标志
 * @param index 模型索引
 * @return 项标志
 */
Qt::ItemFlags NodeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    // ID 列不可编辑
    if (index.column() == 0) {
        return QAbstractTableModel::flags(index);
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

/**
 * @brief 添加节点
 * @param node 要添加的节点
 */
void NodeModel::addNode(const ConceptNode& node)
{
    if (m_nodeIndexMap.contains(node.id())) {
        return;
    }

    int row = m_nodes.size();
    beginInsertRows(QModelIndex(), row, row);

    m_nodes.append(node);
    m_nodeIndexMap[node.id()] = row;

    endInsertRows();

    emit nodeAdded(node);
}

/**
 * @brief 删除节点
 * @param index 模型索引
 */
void NodeModel::removeNode(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= m_nodes.size()) {
        return;
    }

    QString nodeId = m_nodes[index.row()].id();

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_nodes.removeAt(index.row());

    // 更新索引映射
    m_nodeIndexMap.clear();
    for (int i = 0; i < m_nodes.size(); ++i) {
        m_nodeIndexMap[m_nodes[i].id()] = i;
    }

    endRemoveRows();

    emit nodeRemoved(nodeId);
}

/**
 * @brief 更新节点
 * @param index 模型索引
 * @param node 新的节点数据
 */
void NodeModel::updateNode(const QModelIndex& index, const ConceptNode& node)
{
    if (!index.isValid() || index.row() >= m_nodes.size()) {
        return;
    }

    m_nodes[index.row()] = node;

    emit dataChanged(index, index);
    emit nodeUpdated(node);
}

/**
 * @brief 根据索引获取节点
 * @param index 模型索引
 * @return 节点指针，如果不存在则返回 nullptr
 */
ConceptNode* NodeModel::nodeAt(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= m_nodes.size()) {
        return nullptr;
    }

    return &m_nodes[index.row()];
}

/**
 * @brief 根据索引获取节点（常量版本）
 * @param index 模型索引
 * @return 节点指针，如果不存在则返回 nullptr
 */
const ConceptNode* NodeModel::nodeAt(const QModelIndex& index) const
{
    if (!index.isValid() || index.row() >= m_nodes.size()) {
        return nullptr;
    }

    return &m_nodes[index.row()];
}

/**
 * @brief 根据ID获取节点
 * @param id 节点ID
 * @return 节点指针，如果不存在则返回 nullptr
 */
ConceptNode* NodeModel::nodeById(const QString& id)
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
const ConceptNode* NodeModel::nodeById(const QString& id) const
{
    if (!m_nodeIndexMap.contains(id)) {
        return nullptr;
    }

    return &m_nodes[m_nodeIndexMap[id]];
}

/**
 * @brief 清空所有节点
 */
void NodeModel::clear()
{
    beginResetModel();

    m_nodes.clear();
    m_nodeIndexMap.clear();

    endResetModel();
}

/**
 * @brief 设置节点列表
 * @param nodes 节点列表
 */
void NodeModel::setNodes(const QVector<ConceptNode>& nodes)
{
    beginResetModel();

    m_nodes = nodes;

    // 更新索引映射
    m_nodeIndexMap.clear();
    for (int i = 0; i < m_nodes.size(); ++i) {
        m_nodeIndexMap[m_nodes[i].id()] = i;
    }

    endResetModel();
}

/**
 * @brief 根据ID查找节点索引
 * @param id 节点ID
 * @return 模型索引
 */
QModelIndex NodeModel::findIndexById(const QString& id) const
{
    if (!m_nodeIndexMap.contains(id)) {
        return QModelIndex();
    }

    int row = m_nodeIndexMap[id];
    return index(row, 0);
}
