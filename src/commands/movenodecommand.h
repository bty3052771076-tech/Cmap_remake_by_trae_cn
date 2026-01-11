#ifndef MOVENODECOMMAND_H
#define MOVENODECOMMAND_H

#include <QUndoCommand>
#include <QPointF>
#include "conceptnode.h"
#include "graphicsscene.h"

/**
 * @brief 移动节点命令类（继承 QUndoCommand）
 *
 * 该类用于实现移动节点的撤销/重做功能，包括：
 * - 执行移动节点操作
 * - 撤销移动节点操作
 * - 重做移动节点操作
 */
class MoveNodeCommand : public QUndoCommand
{
public:
    /**
     * @brief 构造函数 - 创建一个移动节点命令
     * @param scene 图形场景
     * @param nodeId 要移动的节点ID
     * @param oldPos 旧位置
     * @param newPos 新位置
     * @param parent 父命令
     */
    explicit MoveNodeCommand(GraphicsScene* scene, const QString& nodeId,
                            const QPointF& oldPos, const QPointF& newPos,
                            QUndoCommand* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MoveNodeCommand();

    /**
     * @brief 撤销命令
     */
    void undo() override;

    /**
     * @brief 重做命令
     */
    void redo() override;

    /**
     * @brief 合并命令
     * @param command 要合并的命令
     * @return 如果成功合并返回 true，否则返回 false
     */
    bool mergeWith(const QUndoCommand* command) override;

    /**
     * @brief 获取命令ID
     * @return 命令ID
     */
    int id() const override { return 1001; }

private:
    GraphicsScene* m_scene;    // 图形场景
    QString m_nodeId;         // 要移动的节点ID
    QPointF m_oldPos;         // 旧位置
    QPointF m_newPos;         // 新位置
};

#endif // MOVENODECOMMAND_H
