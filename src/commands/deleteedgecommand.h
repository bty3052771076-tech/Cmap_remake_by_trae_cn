#ifndef DELETEEDGECOMMAND_H
#define DELETEEDGECOMMAND_H

#include <QUndoCommand>
#include "conceptedge.h"
#include "graphicsscene.h"

/**
 * @brief 删除连接线命令类（继承 QUndoCommand）
 *
 * 该类用于实现删除连接线的撤销/重做功能，包括：
 * - 执行删除连接线操作
 * - 撤销删除连接线操作
 * - 重做删除连接线操作
 */
class DeleteEdgeCommand : public QUndoCommand
{
public:
    /**
     * @brief 构造函数 - 创建一个删除连接线命令
     * @param scene 图形场景
     * @param edgeId 要删除的连接线ID
     * @param parent 父命令
     */
    explicit DeleteEdgeCommand(GraphicsScene* scene, const QString& edgeId, QUndoCommand* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~DeleteEdgeCommand();

    /**
     * @brief 撤销命令
     */
    void undo() override;

    /**
     * @brief 重做命令
     */
    void redo() override;

private:
    GraphicsScene* m_scene;    // 图形场景
    QString m_edgeId;         // 要删除的连接线ID
    ConceptEdge m_edge;        // 连接线数据
};

#endif // DELETEEDGECOMMAND_H
