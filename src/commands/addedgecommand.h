#ifndef ADDEDGECOMMAND_H
#define ADDEDGECOMMAND_H

#include <QUndoCommand>
#include "conceptedge.h"
#include "graphicsscene.h"

/**
 * @brief 添加连接线命令类（继承 QUndoCommand）
 *
 * 该类用于实现添加连接线的撤销/重做功能，包括：
 * - 执行添加连接线操作
 * - 撤销添加连接线操作
 * - 重做添加连接线操作
 */
class AddEdgeCommand : public QUndoCommand
{
public:
    /**
     * @brief 构造函数 - 创建一个添加连接线命令
     * @param scene 图形场景
     * @param edge 要添加的连接线
     * @param parent 父命令
     */
    explicit AddEdgeCommand(GraphicsScene* scene, const ConceptEdge& edge, QUndoCommand* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~AddEdgeCommand();

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
    ConceptEdge m_edge;         // 要添加的连接线
    bool m_isFirstRedo;         // 是否是第一次重做
};

#endif // ADDEDGECOMMAND_H
