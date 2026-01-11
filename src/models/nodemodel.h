#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
#include <QVector>
#include "conceptnode.h"

/**
 * @brief 节点模型类（继承 QAbstractTableModel）
 *
 * 该类用于管理节点数据的表格模型，包括：
 * - 节点数据的增删改查
 * - 数据持久化
 * - 数据验证
 */
class NodeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个节点模型
     * @param parent 父对象
     */
    explicit NodeModel(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~NodeModel();

    // QAbstractTableModel 接口实现
    /**
     * @brief 返回行数
     * @param parent 父索引
     * @return 行数
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @brief 返回列数
     * @param parent 父索引
     * @return 列数
     */
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @brief 返回数据
     * @param index 模型索引
     * @param role 角色
     * @return 数据
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    /**
     * @brief 设置数据
     * @param index 模型索引
     * @param value 值
     * @param role 角色
     * @return 如果成功设置返回 true，否则返回 false
     */
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    /**
     * @brief 返回表头数据
     * @param section 节
     * @param orientation 方向
     * @param role 角色
     * @return 表头数据
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief 返回项标志
     * @param index 模型索引
     * @return 项标志
     */
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // 节点操作
    /**
     * @brief 添加节点
     * @param node 要添加的节点
     */
    void addNode(const ConceptNode& node);

    /**
     * @brief 删除节点
     * @param index 模型索引
     */
    void removeNode(const QModelIndex& index);

    /**
     * @brief 更新节点
     * @param index 模型索引
     * @param node 新的节点数据
     */
    void updateNode(const QModelIndex& index, const ConceptNode& node);

    /**
     * @brief 根据索引获取节点
     * @param index 模型索引
     * @return 节点指针，如果不存在则返回 nullptr
     */
    ConceptNode* nodeAt(const QModelIndex& index);

    /**
     * @brief 根据索引获取节点（常量版本）
     * @param index 模型索引
     * @return 节点指针，如果不存在则返回 nullptr
     */
    const ConceptNode* nodeAt(const QModelIndex& index) const;

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
     * @brief 获取所有节点
     * @return 节点列表
     */
    QVector<ConceptNode> nodes() const { return m_nodes; }

    /**
     * @brief 清空所有节点
     */
    void clear();

    /**
     * @brief 设置节点列表
     * @param nodes 节点列表
     */
    void setNodes(const QVector<ConceptNode>& nodes);

    /**
     * @brief 根据ID查找节点索引
     * @param id 节点ID
     * @return 模型索引
     */
    QModelIndex findIndexById(const QString& id) const;

signals:
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

private:
    QVector<ConceptNode> m_nodes;         // 节点列表
    QMap<QString, int> m_nodeIndexMap;    // 节点ID到索引的映射
};

#endif // NODEMODEL_H
