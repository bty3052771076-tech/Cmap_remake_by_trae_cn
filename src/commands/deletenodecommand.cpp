#include "deletenodecommand.h"
#include "graphicsscene.h"

/**
 * @brief 构造函数 - 创建一个删除节点命令
 * @param scene 图形场景
 * @param nodeId 要删除的节点ID
 * @param parent 父命令
 */
DeleteNodeCommand::DeleteNodeCommand(GraphicsScene* scene, const QString& nodeId, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_scene(scene)
    , m_nodeId(nodeId)
{
    // 保存节点数据
    if (m_scene) {
        const ConceptNode* node = m_scene->conceptMap().nodeById(nodeId);
        if (node) {
            m_node = *node;
        }

        // 保存相关的连接线数据
        m_relatedEdges = m_scene->conceptMap().edgesByNodeId(nodeId);
    }

    // 设置命令文本
    setText(QString("删除节点: %1").arg(m_node.text()));
}

/**
 * @brief 析构函数
 */
DeleteNodeCommand::~DeleteNodeCommand()
{
}

/**
 * @brief 撤销命令
 */
void DeleteNodeCommand::undo()
{
    if (m_scene) {
        // 重新添加节点
        m_scene->addNode(m_node);

        // 重新添加相关的连接线
        for (const ConceptEdge& edge : m_relatedEdges) {
            m_scene->addEdge(edge);
        }
    }
}

/**
 * @brief 重做命令
 */
void DeleteNodeCommand::redo()
{
    if (m_scene) {
        // 从场景中删除节点（会自动删除相关的连接线）
        m_scene->removeNode(m_nodeId);
    }
}
