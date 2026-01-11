#include "addnodecommand.h"

/**
 * @brief 构造函数 - 创建一个添加节点命令
 * @param scene 图形场景
 * @param node 要添加的节点
 * @param parent 父命令
 */
AddNodeCommand::AddNodeCommand(GraphicsScene* scene, const ConceptNode& node, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_scene(scene)
    , m_node(node)
    , m_isFirstRedo(true)
{
    // 设置命令文本
    setText(QString("添加节点: %1").arg(node.text()));
}

/**
 * @brief 析构函数
 */
AddNodeCommand::~AddNodeCommand()
{
}

/**
 * @brief 撤销命令
 */
void AddNodeCommand::undo()
{
    if (m_scene) {
        // 从场景中删除节点
        m_scene->removeNode(m_node.id());
    }
}

/**
 * @brief 重做命令
 */
void AddNodeCommand::redo()
{
    if (m_scene) {
        // 如果是第一次重做，节点已经被添加了，不需要再次添加
        if (!m_isFirstRedo) {
            // 重新添加节点到场景
            m_scene->addNode(m_node);
        } else {
            m_isFirstRedo = false;
        }
    }
}
