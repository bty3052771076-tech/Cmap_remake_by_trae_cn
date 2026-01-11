#ifndef EDGEMODEL_H
#define EDGEMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
#include <QVector>
#include "conceptedge.h"

/**
 * @brief 连接线模型类（继承 QAbstractTableModel）
 *
 * 该类用于管理连接线数据的表格模型，包括：
 * - 连接线数据的增删改查
 * - 数据持久化
 * - 数据验证
 */
class EdgeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个连接线模型
     * @param parent 父对象
     */
    explicit EdgeModel(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~EdgeModel();

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

    // 连接线操作
    /**
     * @brief 添加连接线
     * @param edge 要添加的连接线
     */
    void addEdge(const ConceptEdge& edge);

    /**
     * @brief 删除连接线
     * @param index 模型索引
     */
    void removeEdge(const QModelIndex& index);

    /**
     * @brief 更新连接线
     * @param index 模型索引
     * @param edge 新的连接线数据
     */
    void updateEdge(const QModelIndex& index, const ConceptEdge& edge);

    /**
     * @brief 根据索引获取连接线
     * @param index 模型索引
     * @return 连接线指针，如果不存在则返回 nullptr
     */
    ConceptEdge* edgeAt(const QModelIndex& index);

    /**
     * @brief 根据索引获取连接线（常量版本）
     * @param index 模型索引
     * @return 连接线指针，如果不存在则返回 nullptr
     */
    const ConceptEdge* edgeAt(const QModelIndex& index) const;

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
     * @brief 获取所有连接线
     * @return 连接线列表
     */
    QVector<ConceptEdge> edges() const { return m_edges; }

    /**
     * @brief 清空所有连接线
     */
    void clear();

    /**
     * @brief 设置连接线列表
     * @param edges 连接线列表
     */
    void setEdges(const QVector<ConceptEdge>& edges);

    /**
     * @brief 根据ID查找连接线索引
     * @param id 连接线ID
     * @return 模型索引
     */
    QModelIndex findIndexById(const QString& id) const;

signals:
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
    QVector<ConceptEdge> m_edges;         // 连接线列表
    QMap<QString, int> m_edgeIndexMap;    // 连接线ID到索引的映射
};

#endif // EDGEMODEL_H
