#ifndef ADDNODECOMMAND_H
#define ADDNODECOMMAND_H

#include <QUndoCommand>
#include "conceptnode.h"
#include "graphicsscene.h"

/**
 * @brief 添加节点命令类（继承 QUndoCommand）
 *
 * 该类用于实现添加节点的撤销/重做功能，包括：
 * - 执行添加节点操作
 * - 撤销添加节点操作
 * - 重做添加节点操作
 */
class AddNodeCommand : public QUndoCommand
{
public:
    /**
     * @brief 构造函数 - 创建一个添加节点命令
     * @param scene 图形场景
     * @param node 要添加的节点
     * @param parent 父命令
     */
    explicit AddNodeCommand(GraphicsScene* scene, const ConceptNode& node, QUndoCommand* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~AddNodeCommand();

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
    ConceptNode m_node;         // 要添加的节点
    bool m_isFirstRedo;         // 是否是第一次重做
};

#endif // ADDNODECOMMAND_H
