#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPointF>
#include "graphicsscene.h"

/**
 * @brief 图形视图类（继承 QGraphicsView）
 *
 * 该类用于显示和操作图形场景，包括：
 * - 缩放功能
 * - 平移功能
 * - 视图控制
 * - 鼠标和键盘事件处理
 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个图形视图
     * @param scene 图形场景
     * @param parent 父窗口部件
     */
    explicit GraphicsView(GraphicsScene* scene, QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~GraphicsView();

    /**
     * @brief 设置缩放比例
     * @param scale 缩放比例
     */
    void setZoomScale(qreal scale);

    /**
     * @brief 获取当前缩放比例
     * @return 缩放比例
     */
    qreal zoomScale() const { return m_zoomScale; }

    /**
     * @brief 放大视图
     */
    void zoomIn();

    /**
     * @brief 缩小视图
     */
    void zoomOut();

    /**
     * @brief 重置视图缩放
     */
    void resetZoom();

    /**
     * @brief 适应视图大小
     */
    void fitInView();

    /**
     * @brief 居中视图
     */
    void centerView();

    /**
     * @brief 设置是否启用平移
     * @param enabled 是否启用
     */
    void setPanningEnabled(bool enabled) { m_panningEnabled = enabled; }

    /**
     * @brief 检查是否启用平移
     * @return 如果启用返回 true，否则返回 false
     */
    bool isPanningEnabled() const { return m_panningEnabled; }

    /**
     * @brief 设置是否启用缩放
     * @param enabled 是否启用
     */
    void setZoomingEnabled(bool enabled) { m_zoomingEnabled = enabled; }

    /**
     * @brief 检查是否启用缩放
     * @return 如果启用返回 true，否则返回 false
     */
    bool isZoomingEnabled() const { return m_zoomingEnabled; }

signals:
    /**
     * @brief 视图缩放信号
     * @param scale 缩放比例
     */
    void zoomChanged(qreal scale);

    /**
     * @brief 视图平移信号
     * @param delta 平移增量
     */
    void viewPanned(const QPointF& delta);

protected:
    /**
     * @brief 鼠标滚轮事件处理
     * @param event 滚轮事件
     */
    void wheelEvent(QWheelEvent* event) override;

    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标移动事件处理
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief 绘制背景事件处理
     * @param painter 绘制器
     * @param rect 绘制矩形
     */
    void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
    /**
     * @brief 绘制网格背景
     * @param painter 绘制器
     * @param rect 绘制矩形
     */
    void drawGrid(QPainter* painter, const QRectF& rect);

    /**
     * @brief 绘制坐标轴
     * @param painter 绘制器
     * @param rect 绘制矩形
     */
    void drawAxes(QPainter* painter, const QRectF& rect);

    qreal m_zoomScale;              // 当前缩放比例
    qreal m_minZoomScale;           // 最小缩放比例
    qreal m_maxZoomScale;           // 最大缩放比例
    qreal m_zoomStep;               // 缩放步长
    bool m_panningEnabled;          // 是否启用平移
    bool m_zoomingEnabled;          // 是否启用缩放
    bool m_isPanning;               // 是否正在平移
    QPointF m_panStartPos;          // 平移起始位置
    Qt::MouseButton m_panButton;    // 平移按钮
    bool m_drawGrid;                // 是否绘制网格
    int m_gridSize;                 // 网格大小
    QColor m_gridColor;             // 网格颜色
    QColor m_axisColor;             // 坐标轴颜色
};

#endif // GRAPHICSVIEW_H
