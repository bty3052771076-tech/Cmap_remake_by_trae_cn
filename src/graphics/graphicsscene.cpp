#include "graphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsLineItem>
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个图形场景
 * @param parent 父对象
 */
GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_isDragging(false)
    , m_isCreatingEdge(false)
    , m_edgeSourceNode(nullptr)
    , m_tempEdgeLine(nullptr)
{
    // 设置场景大小
    setSceneRect(-2000, -2000, 4000, 4000);

    // 设置背景网格
    QBrush gridBrush(QColor(240, 240, 240));
    setBackgroundBrush(gridBrush);
}

/**
 * @brief 析构函数
 */
GraphicsScene::~GraphicsScene()
{
    clearScene();
}

/**
 * @brief 设置概念图数据
 * @param map 概念图数据
 */
void GraphicsScene::setConceptMap(const ConceptMap& map)
{
    m_conceptMap = map;
    refreshScene();
}

/**
 * @brief 添加节点
 * @param node 概念节点数据
 * @return 图形节点指针
 */
GraphicsNode* GraphicsScene::addNode(const ConceptNode& node)
{
    // 创建图形节点
    GraphicsNode* graphicsNode = new GraphicsNode(node);
    addItem(graphicsNode);

    // 添加到映射
    m_graphicsNodes[node.id()] = graphicsNode;

    // 添加到概念图
    m_conceptMap.addNode(node);

    // 发送信号
    emit nodeAdded(node);

    return graphicsNode;
}

/**
 * @brief 删除节点
 * @param nodeId 节点ID
 * @return 如果成功删除返回 true，否则返回 false
 */
bool GraphicsScene::removeNode(const QString& nodeId)
{
    GraphicsNode* graphicsNode = graphicsNodeById(nodeId);
    if (!graphicsNode) {
        return false;
    }

    // 删除与该节点相关的所有连接线
    QVector<ConceptEdge> relatedEdges = m_conceptMap.edgesByNodeId(nodeId);
    for (const ConceptEdge& edge : relatedEdges) {
        removeEdge(edge.id());
    }

    // 从场景中移除
    removeItem(graphicsNode);
    delete graphicsNode;

    // 从映射中移除
    m_graphicsNodes.remove(nodeId);

    // 从概念图中移除
    m_conceptMap.removeNode(nodeId);

    // 发送信号
    emit nodeRemoved(nodeId);
    emit sceneChanged();

    return true;
}

/**
 * @brief 添加连接线
 * @param edge 概念连接线数据
 * @return 图形连接线指针
 */
GraphicsEdge* GraphicsScene::addEdge(const ConceptEdge& edge)
{
    // 获取源节点和目标节点
    GraphicsNode* sourceNode = graphicsNodeById(edge.sourceNodeId());
    GraphicsNode* targetNode = graphicsNodeById(edge.targetNodeId());

    if (!sourceNode || !targetNode) {
        return nullptr;
    }

    // 创建图形连接线
    GraphicsEdge* graphicsEdge = new GraphicsEdge(edge, sourceNode, targetNode);
    addItem(graphicsEdge);

    // 添加到映射
    m_graphicsEdges[edge.id()] = graphicsEdge;

    // 添加到概念图
    m_conceptMap.addEdge(edge);

    // 发送信号
    emit edgeAdded(edge);

    return graphicsEdge;
}

/**
 * @brief 删除连接线
 * @param edgeId 连接线ID
 * @return 如果成功删除返回 true，否则返回 false
 */
bool GraphicsScene::removeEdge(const QString& edgeId)
{
    GraphicsEdge* graphicsEdge = graphicsEdgeById(edgeId);
    if (!graphicsEdge) {
        return false;
    }

    // 从场景中移除
    removeItem(graphicsEdge);
    delete graphicsEdge;

    // 从映射中移除
    m_graphicsEdges.remove(edgeId);

    // 从概念图中移除
    m_conceptMap.removeEdge(edgeId);

    // 发送信号
    emit edgeRemoved(edgeId);
    emit sceneChanged();

    return true;
}

/**
 * @brief 根据ID获取图形节点
 * @param nodeId 节点ID
 * @return 图形节点指针，如果不存在则返回 nullptr
 */
GraphicsNode* GraphicsScene::graphicsNodeById(const QString& nodeId)
{
    return m_graphicsNodes.value(nodeId, nullptr);
}

/**
 * @brief 根据ID获取图形连接线
 * @param edgeId 连接线ID
 * @return 图形连接线指针，如果不存在则返回 nullptr
 */
GraphicsEdge* GraphicsScene::graphicsEdgeById(const QString& edgeId)
{
    return m_graphicsEdges.value(edgeId, nullptr);
}

/**
 * @brief 获取所有选中的节点
 * @return 选中的节点列表
 */
QList<GraphicsNode*> GraphicsScene::selectedNodes() const
{
    QList<GraphicsNode*> result;
    for (GraphicsNode* node : m_graphicsNodes) {
        if (node->isSelectedNode()) {
            result.append(node);
        }
    }
    return result;
}

/**
 * @brief 获取所有选中的连接线
 * @return 选中的连接线列表
 */
QList<GraphicsEdge*> GraphicsScene::selectedEdges() const
{
    QList<GraphicsEdge*> result;
    for (GraphicsEdge* edge : m_graphicsEdges) {
        if (edge->isSelectedEdge()) {
            result.append(edge);
        }
    }
    return result;
}

/**
 * @brief 获取所有节点
 * @return 所有节点列表
 */
QList<GraphicsNode*> GraphicsScene::allNodes() const
{
    return m_graphicsNodes.values();
}

/**
 * @brief 获取所有连接线
 * @return 所有连接线列表
 */
QList<GraphicsEdge*> GraphicsScene::allEdges() const
{
    return m_graphicsEdges.values();
}

/**
 * @brief 清空场景
 */
void GraphicsScene::clearScene()
{
    // 清空图形连接线
    for (GraphicsEdge* edge : m_graphicsEdges) {
        removeItem(edge);
        delete edge;
    }
    m_graphicsEdges.clear();

    // 清空图形节点
    for (GraphicsNode* node : m_graphicsNodes) {
        removeItem(node);
        delete node;
    }
    m_graphicsNodes.clear();

    // 不清空概念图数据，因为clearScene只负责清空图形元素
    // m_conceptMap.clear();
}

/**
 * @brief 刷新场景
 */
void GraphicsScene::refreshScene()
{
    // 清空场景
    clearScene();

    // 重新添加节点
    for (const ConceptNode& node : m_conceptMap.nodes()) {
        GraphicsNode* graphicsNode = new GraphicsNode(node);
        addItem(graphicsNode);
        m_graphicsNodes[node.id()] = graphicsNode;
    }

    // 重新添加连接线
    for (const ConceptEdge& edge : m_conceptMap.edges()) {
        GraphicsNode* sourceNode = graphicsNodeById(edge.sourceNodeId());
        GraphicsNode* targetNode = graphicsNodeById(edge.targetNodeId());

        if (sourceNode && targetNode) {
            GraphicsEdge* graphicsEdge = new GraphicsEdge(edge, sourceNode, targetNode);
            addItem(graphicsEdge);
            m_graphicsEdges[edge.id()] = graphicsEdge;
        }
    }

    update();
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件
 */
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->scenePos();
        m_isDragging = true;

        // 检查是否按住 Shift 键且点击了节点
        if (event->modifiers() & Qt::ShiftModifier) {
            QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
            if (item && item->type() == GraphicsNode::Type) {
                GraphicsNode* node = qgraphicsitem_cast<GraphicsNode*>(item);
                if (node) {
                    // 开始创建连接线
                    m_isCreatingEdge = true;
                    m_edgeSourceNode = node;

                    // 创建临时连接线用于预览
                    m_tempEdgeLine = new QGraphicsLineItem();
                    m_tempEdgeLine->setPen(QPen(QColor(100, 100, 100), 2, Qt::DashLine));
                    m_tempEdgeLine->setZValue(1000); // 确保在最上层
                    addItem(m_tempEdgeLine);

                    QPointF nodeCenter = node->pos() + node->boundingRect().center();
                    QLineF line(nodeCenter, event->scenePos());
                    m_tempEdgeLine->setLine(line);

                    return; // 不调用父类方法，避免触发节点拖拽
                }
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);

    // 检查选中的项
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    if (item) {
        if (item->type() == GraphicsNode::Type) {
            GraphicsNode* node = qgraphicsitem_cast<GraphicsNode*>(item);
            if (node) {
                // 检查是否按住了 Ctrl 或 Ctrl+Shift 键
                if (event->modifiers() & Qt::ControlModifier) {
                    // 多选模式：切换选中状态
                    node->setSelectedNode(!node->isSelectedNode());
                } else {
                    // 单选模式：只选中当前节点
                    // 先取消其他节点的选中状态
                    for (GraphicsNode* n : m_graphicsNodes) {
                        if (n != node) {
                            n->setSelectedNode(false);
                        }
                    }
                    node->setSelectedNode(true);
                }
                emit nodeSelected(node->id());
            }
        } else if (item->type() == GraphicsEdge::Type) {
            GraphicsEdge* edge = qgraphicsitem_cast<GraphicsEdge*>(item);
            if (edge) {
                // 检查是否按住了 Ctrl 或 Ctrl+Shift 键
                if (event->modifiers() & Qt::ControlModifier) {
                    // 多选模式：切换选中状态
                    edge->setSelectedEdge(!edge->isSelectedEdge());
                } else {
                    // 单选模式：只选中当前连接线
                    // 先取消其他连接线的选中状态
                    for (GraphicsEdge* e : m_graphicsEdges) {
                        if (e != edge) {
                            e->setSelectedEdge(false);
                        }
                    }
                    edge->setSelectedEdge(true);
                }
                emit edgeSelected(edge->id());
            }
        }
    }
}

/**
 * @brief 鼠标移动事件处理
 * @param event 鼠标事件
 */
void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // 如果正在创建连接线，更新临时连接线
    if (m_isCreatingEdge && m_tempEdgeLine && m_edgeSourceNode) {
        QPointF nodeCenter = m_edgeSourceNode->pos() + m_edgeSourceNode->boundingRect().center();
        QLineF line(nodeCenter, event->scenePos());
        m_tempEdgeLine->setLine(line);
        return;
    }

    QGraphicsScene::mouseMoveEvent(event);

    if (m_isDragging) {
        emit sceneChanged();
    }
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件
 */
void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // 如果正在创建连接线
    if (m_isCreatingEdge) {
        // 删除临时连接线
        if (m_tempEdgeLine) {
            removeItem(m_tempEdgeLine);
            delete m_tempEdgeLine;
            m_tempEdgeLine = nullptr;
        }

        // 检查是否释放到了另一个节点上
        QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
        if (item && item->type() == GraphicsNode::Type) {
            GraphicsNode* targetNode = qgraphicsitem_cast<GraphicsNode*>(item);
            if (targetNode && targetNode != m_edgeSourceNode) {
                // 创建连接线
                ConceptEdge edge(m_edgeSourceNode->id(), targetNode->id(), "连接", QColor(100, 100, 100));
                addEdge(edge);
            }
        }

        // 重置状态
        m_isCreatingEdge = false;
        m_edgeSourceNode = nullptr;
        return;
    }

    QGraphicsScene::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}

/**
 * @brief 键盘按下事件处理
 * @param event 键盘事件
 */
void GraphicsScene::keyPressEvent(QKeyEvent* event)
{
    // 删除选中的项
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        // 删除选中的连接线
        QList<GraphicsEdge*> edges = selectedEdges();
        for (GraphicsEdge* edge : edges) {
            removeEdge(edge->id());
        }

        // 删除选中的节点
        QList<GraphicsNode*> nodes = selectedNodes();
        for (GraphicsNode* node : nodes) {
            removeNode(node->id());
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

/**
 * @brief 更新连接线位置
 */
void GraphicsScene::updateEdgePositions()
{
    for (GraphicsEdge* edge : m_graphicsEdges) {
        edge->updatePosition();
    }
}
