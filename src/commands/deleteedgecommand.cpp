#include "deleteedgecommand.h"

/**
 * @brief 构造函数 - 创建一个删除连接线命令
 * @param scene 图形场景
 * @param edgeId 要删除的连接线ID
 * @param parent 父命令
 */
DeleteEdgeCommand::DeleteEdgeCommand(GraphicsScene* scene, const QString& edgeId, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_scene(scene)
    , m_edgeId(edgeId)
{
    // 保存连接线数据
    if (m_scene) {
        const ConceptEdge* edge = m_scene->conceptMap().edgeById(edgeId);
        if (edge) {
            m_edge = *edge;
        }
    }

    // 设置命令文本
    setText(QString("删除连接线: %1 -> %2").arg(m_edge.sourceNodeId()).arg(m_edge.targetNodeId()));
}

/**
 * @brief 析构函数
 */
DeleteEdgeCommand::~DeleteEdgeCommand()
{
}

/**
 * @brief 撤销命令
 */
void DeleteEdgeCommand::undo()
{
    if (m_scene) {
        // 重新添加连接线
        m_scene->addEdge(m_edge);
    }
}

/**
 * @brief 重做命令
 */
void DeleteEdgeCommand::redo()
{
    if (m_scene) {
        // 从场景中删除连接线
        m_scene->removeEdge(m_edgeId);
    }
}
