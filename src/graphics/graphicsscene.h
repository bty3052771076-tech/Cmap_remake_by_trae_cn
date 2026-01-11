#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>
#include "conceptmap.h"
#include "graphicsnode.h"
#include "graphicsedge.h"

/**
 * @brief 图形场景类（继承 QGraphicsScene）
 *
 * 该类用于管理概念图的图形场景，包括：
 * - 场景管理
 * - 节点和连接线的添加、删除
 * - 事件处理
 * - 场景缩放和平移
 */
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个图形场景
     * @param parent 父对象
     */
    explicit GraphicsScene(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~GraphicsScene();

    /**
     * @brief 设置概念图数据
     * @param map 概念图数据
     */
    void setConceptMap(const ConceptMap& map);

    /**
     * @brief 获取概念图数据
     * @return 概念图数据
     */
    ConceptMap conceptMap() const { return m_conceptMap; }

    /**
     * @brief 添加节点
     * @param node 概念节点数据
     * @return 图形节点指针
     */
    GraphicsNode* addNode(const ConceptNode& node);

    /**
     * @brief 删除节点
     * @param nodeId 节点ID
     * @return 如果成功删除返回 true，否则返回 false
     */
    bool removeNode(const QString& nodeId);

    /**
     * @brief 添加连接线
     * @param edge 概念连接线数据
     * @return 图形连接线指针
     */
    GraphicsEdge* addEdge(const ConceptEdge& edge);

    /**
     * @brief 删除连接线
     * @param edgeId 连接线ID
     * @return 如果成功删除返回 true，否则返回 false
     */
    bool removeEdge(const QString& edgeId);

    /**
     * @brief 根据ID获取图形节点
     * @param nodeId 节点ID
     * @return 图形节点指针，如果不存在则返回 nullptr
     */
    GraphicsNode* graphicsNodeById(const QString& nodeId);

    /**
     * @brief 根据ID获取图形连接线
     * @param edgeId 连接线ID
     * @return 图形连接线指针，如果不存在则返回 nullptr
     */
    GraphicsEdge* graphicsEdgeById(const QString& edgeId);

    /**
     * @brief 获取所有选中的节点
     * @return 选中的节点列表
     */
    QList<GraphicsNode*> selectedNodes() const;

    /**
     * @brief 获取所有选中的连接线
     * @return 选中的连接线列表
     */
    QList<GraphicsEdge*> selectedEdges() const;

    /**
     * @brief 获取所有节点
     * @return 所有节点列表
     */
    QList<GraphicsNode*> allNodes() const;

    /**
     * @brief 获取所有连接线
     * @return 所有连接线列表
     */
    QList<GraphicsEdge*> allEdges() const;

    /**
     * @brief 清空场景
     */
    void clearScene();

    /**
     * @brief 刷新场景
     */
    void refreshScene();

signals:
    /**
     * @brief 节点添加信号
     * @param node 添加的节点
     */
    void nodeAdded(const ConceptNode& node);

    /**
     * @brief 节点删除信号
     * @param nodeId 删除的节点ID
     */
    void nodeRemoved(const QString& nodeId);

    /**
     * @brief 连接线添加信号
     * @param edge 添加的连接线
     */
    void edgeAdded(const ConceptEdge& edge);

    /**
     * @brief 连接线删除信号
     * @param edgeId 删除的连接线ID
     */
    void edgeRemoved(const QString& edgeId);

    /**
     * @brief 节点选中信号
     * @param nodeId 选中的节点ID
     */
    void nodeSelected(const QString& nodeId);

    /**
     * @brief 连接线选中信号
     * @param edgeId 选中的连接线ID
     */
    void edgeSelected(const QString& edgeId);

    /**
     * @brief 场景变化信号
     */
    void sceneChanged();

protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     */
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief 鼠标移动事件处理
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件
     */
    void keyPressEvent(QKeyEvent* event) override;

private:
    /**
     * @brief 更新连接线位置
     */
    void updateEdgePositions();

    ConceptMap m_conceptMap;                    // 概念图数据
    QMap<QString, GraphicsNode*> m_graphicsNodes; // 图形节点映射
    QMap<QString, GraphicsEdge*> m_graphicsEdges; // 图形连接线映射
    bool m_isDragging;                          // 是否正在拖拽
    QPointF m_dragStartPos;                     // 拖拽起始位置
    bool m_isCreatingEdge;                      // 是否正在创建连接线
    GraphicsNode* m_edgeSourceNode;              // 连接线源节点
    QGraphicsLineItem* m_tempEdgeLine;          // 临时连接线（用于拖拽预览）
};

#endif // GRAPHICSSCENE_H
