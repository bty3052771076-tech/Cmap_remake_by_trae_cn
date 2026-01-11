#include "movenodecommand.h"
#include "graphicsscene.h"

/**
 * @brief 构造函数 - 创建一个移动节点命令
 * @param scene 图形场景
 * @param nodeId 要移动的节点ID
 * @param oldPos 旧位置
 * @param newPos 新位置
 * @param parent 父命令
 */
MoveNodeCommand::MoveNodeCommand(GraphicsScene* scene, const QString& nodeId,
                                 const QPointF& oldPos, const QPointF& newPos,
                                 QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_scene(scene)
    , m_nodeId(nodeId)
    , m_oldPos(oldPos)
    , m_newPos(newPos)
{
    // 设置命令文本
    setText(QString("移动节点: %1").arg(nodeId));
}

/**
 * @brief 析构函数
 */
MoveNodeCommand::~MoveNodeCommand()
{
}

/**
 * @brief 撤销命令
 */
void MoveNodeCommand::undo()
{
    if (m_scene) {
        GraphicsNode* graphicsNode = m_scene->graphicsNodeById(m_nodeId);
        if (graphicsNode) {
            // 恢复到旧位置
            graphicsNode->updatePosition(m_oldPos);
        }
    }
}

/**
 * @brief 重做命令
 */
void MoveNodeCommand::redo()
{
    if (m_scene) {
        GraphicsNode* graphicsNode = m_scene->graphicsNodeById(m_nodeId);
        if (graphicsNode) {
            // 移动到新位置
            graphicsNode->updatePosition(m_newPos);
        }
    }
}

/**
 * @brief 合并命令
 * @param command 要合并的命令
 * @return 如果成功合并返回 true，否则返回 false
 */
bool MoveNodeCommand::mergeWith(const QUndoCommand* command)
{
    const MoveNodeCommand* moveCommand = static_cast<const MoveNodeCommand*>(command);

    // 检查是否是同一个节点的移动命令
    if (m_nodeId != moveCommand->m_nodeId) {
        return false;
    }

    // 更新新位置
    m_newPos = moveCommand->m_newPos;

    return true;
}
