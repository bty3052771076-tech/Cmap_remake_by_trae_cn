#ifndef DELETENODECOMMAND_H
#define DELETENODECOMMAND_H

#include <QUndoCommand>
#include "conceptnode.h"
#include "conceptedge.h"
#include "graphicsscene.h"

/**
 * @brief 删除节点命令类（继承 QUndoCommand）
 *
 * 该类用于实现删除节点的撤销/重做功能，包括：
 * - 执行删除节点操作
 * - 撤销删除节点操作
 * - 重做删除节点操作
 */
class DeleteNodeCommand : public QUndoCommand
{
public:
    /**
     * @brief 构造函数 - 创建一个删除节点命令
     * @param scene 图形场景
     * @param nodeId 要删除的节点ID
     * @param parent 父命令
     */
    explicit DeleteNodeCommand(GraphicsScene* scene, const QString& nodeId, QUndoCommand* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~DeleteNodeCommand();

    /**
     * @brief 撤销命令
     */
    void undo() override;

    /**
     * @brief 重做命令
     */
    void redo() override;

private:
    GraphicsScene* m_scene;              // 图形场景
    QString m_nodeId;                    // 要删除的节点ID
    ConceptNode m_node;                   // 节点数据
    QVector<ConceptEdge> m_relatedEdges;  // 相关的连接线数据
};

#endif // DELETENODECOMMAND_H
