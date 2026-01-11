#include "edgemodel.h"

/**
 * @brief 构造函数 - 创建一个连接线模型
 * @param parent 父对象
 */
EdgeModel::EdgeModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

/**
 * @brief 析构函数
 */
EdgeModel::~EdgeModel()
{
}

/**
 * @brief 返回行数
 * @param parent 父索引
 * @return 行数
 */
int EdgeModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_edges.size();
}

/**
 * @brief 返回列数
 * @param parent 父索引
 * @return 列数
 */
int EdgeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 5; // ID, 源节点ID, 目标节点ID, 标签, 颜色
}

/**
 * @brief 返回数据
 * @param index 模型索引
 * @param role 角色
 * @return 数据
 */
QVariant EdgeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_edges.size()) {
        return QVariant();
    }

    const ConceptEdge& edge = m_edges[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return edge.id();
            case 1: return edge.sourceNodeId();
            case 2: return edge.targetNodeId();
            case 3: return edge.label();
            case 4: return edge.color().name();
            default: return QVariant();
        }
    }

    if (role == Qt::BackgroundRole) {
        return QBrush(edge.color());
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
bool EdgeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= m_edges.size() || role != Qt::EditRole) {
        return false;
    }

    ConceptEdge& edge = m_edges[index.row()];

    switch (index.column()) {
        case 1:
            edge.setSourceNodeId(value.toString());
            break;
        case 2:
            edge.setTargetNodeId(value.toString());
            break;
        case 3:
            edge.setLabel(value.toString());
            break;
        case 4:
            edge.setColor(QColor(value.toString()));
            break;
        default:
            return false;
    }

    emit dataChanged(index, index, {role});
    emit edgeUpdated(edge);

    return true;
}

/**
 * @brief 返回表头数据
 * @param section 节
 * @param orientation 方向
 * @param role 角色
 * @return 表头数据
 */
QVariant EdgeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "ID";
            case 1: return "源节点ID";
            case 2: return "目标节点ID";
            case 3: return "标签";
            case 4: return "颜色";
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
Qt::ItemFlags EdgeModel::flags(const QModelIndex& index) const
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
 * @brief 添加连接线
 * @param edge 要添加的连接线
 */
void EdgeModel::addEdge(const ConceptEdge& edge)
{
    if (m_edgeIndexMap.contains(edge.id())) {
        return;
    }

    int row = m_edges.size();
    beginInsertRows(QModelIndex(), row, row);

    m_edges.append(edge);
    m_edgeIndexMap[edge.id()] = row;

    endInsertRows();

    emit edgeAdded(edge);
}

/**
 * @brief 删除连接线
 * @param index 模型索引
 */
void EdgeModel::removeEdge(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= m_edges.size()) {
        return;
    }

    QString edgeId = m_edges[index.row()].id();

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_edges.removeAt(index.row());

    // 更新索引映射
    m_edgeIndexMap.clear();
    for (int i = 0; i < m_edges.size(); ++i) {
        m_edgeIndexMap[m_edges[i].id()] = i;
    }

    endRemoveRows();

    emit edgeRemoved(edgeId);
}

/**
 * @brief 更新连接线
 * @param index 模型索引
 * @param edge 新的连接线数据
 */
void EdgeModel::updateEdge(const QModelIndex& index, const ConceptEdge& edge)
{
    if (!index.isValid() || index.row() >= m_edges.size()) {
        return;
    }

    m_edges[index.row()] = edge;

    emit dataChanged(index, index);
    emit edgeUpdated(edge);
}

/**
 * @brief 根据索引获取连接线
 * @param index 模型索引
 * @return 连接线指针，如果不存在则返回 nullptr
 */
ConceptEdge* EdgeModel::edgeAt(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= m_edges.size()) {
        return nullptr;
    }

    return &m_edges[index.row()];
}

/**
 * @brief 根据索引获取连接线（常量版本）
 * @param index 模型索引
 * @return 连接线指针，如果不存在则返回 nullptr
 */
const ConceptEdge* EdgeModel::edgeAt(const QModelIndex& index) const
{
    if (!index.isValid() || index.row() >= m_edges.size()) {
        return nullptr;
    }

    return &m_edges[index.row()];
}

/**
 * @brief 根据ID获取连接线
 * @param id 连接线ID
 * @return 连接线指针，如果不存在则返回 nullptr
 */
ConceptEdge* EdgeModel::edgeById(const QString& id)
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
const ConceptEdge* EdgeModel::edgeById(const QString& id) const
{
    if (!m_edgeIndexMap.contains(id)) {
        return nullptr;
    }

    return &m_edges[m_edgeIndexMap[id]];
}

/**
 * @brief 清空所有连接线
 */
void EdgeModel::clear()
{
    beginResetModel();

    m_edges.clear();
    m_edgeIndexMap.clear();

    endResetModel();
}

/**
 * @brief 设置连接线列表
 * @param edges 连接线列表
 */
void EdgeModel::setEdges(const QVector<ConceptEdge>& edges)
{
    beginResetModel();

    m_edges = edges;

    // 更新索引映射
    m_edgeIndexMap.clear();
    for (int i = 0; i < m_edges.size(); ++i) {
        m_edgeIndexMap[m_edges[i].id()] = i;
    }

    endResetModel();
}

/**
 * @brief 根据ID查找连接线索引
 * @param id 连接线ID
 * @return 模型索引
 */
QModelIndex EdgeModel::findIndexById(const QString& id) const
{
    if (!m_edgeIndexMap.contains(id)) {
        return QModelIndex();
    }

    int row = m_edgeIndexMap[id];
    return index(row, 0);
}
