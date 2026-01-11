#include "addedgecommand.h"

/**
 * @brief 构造函数 - 创建一个添加连接线命令
 * @param scene 图形场景
 * @param edge 要添加的连接线
 * @param parent 父命令
 */
AddEdgeCommand::AddEdgeCommand(GraphicsScene* scene, const ConceptEdge& edge, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_scene(scene)
    , m_edge(edge)
    , m_isFirstRedo(true)
{
    // 设置命令文本
    setText(QString("添加连接线: %1 -> %2").arg(edge.sourceNodeId()).arg(edge.targetNodeId()));
}

/**
 * @brief 析构函数
 */
AddEdgeCommand::~AddEdgeCommand()
{
}

/**
 * @brief 撤销命令
 */
void AddEdgeCommand::undo()
{
    if (m_scene) {
        // 从场景中删除连接线
        m_scene->removeEdge(m_edge.id());
    }
}

/**
 * @brief 重做命令
 */
void AddEdgeCommand::redo()
{
    if (m_scene) {
        // 如果是第一次重做，连接线已经被添加了，不需要再次添加
        if (!m_isFirstRedo) {
            // 重新添加连接线到场景
            m_scene->addEdge(m_edge);
        } else {
            m_isFirstRedo = false;
        }
    }
}
