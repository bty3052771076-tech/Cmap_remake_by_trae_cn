#include "graphicsview.h"
#include <QPainter>
#include <QScrollBar>
#include <QApplication>
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个图形视图
 * @param scene 图形场景
 * @param parent 父窗口部件
 */
GraphicsView::GraphicsView(GraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
    , m_zoomScale(1.0)
    , m_minZoomScale(0.1)
    , m_maxZoomScale(10.0)
    , m_zoomStep(1.2)
    , m_panningEnabled(true)
    , m_zoomingEnabled(true)
    , m_isPanning(false)
    , m_panButton(Qt::MiddleButton)
    , m_drawGrid(true)
    , m_gridSize(50)
    , m_gridColor(QColor(220, 220, 220))
    , m_axisColor(QColor(180, 180, 180))
{
    // 设置渲染选项
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);

    // 设置拖拽模式
    setDragMode(QGraphicsView::RubberBandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // 设置滚动条策略
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 设置对齐方式
    setAlignment(Qt::AlignCenter);

    // 设置背景颜色
    setBackgroundBrush(QColor(250, 250, 250));

    // 启用鼠标追踪
    setMouseTracking(true);
}

/**
 * @brief 析构函数
 */
GraphicsView::~GraphicsView()
{
}

/**
 * @brief 设置缩放比例
 * @param scale 缩放比例
 */
void GraphicsView::setZoomScale(qreal scale)
{
    // 限制缩放范围
    scale = qBound(m_minZoomScale, scale, m_maxZoomScale);

    if (qFuzzyCompare(m_zoomScale, scale)) {
        return;
    }

    // 计算缩放因子
    qreal scaleFactor = scale / m_zoomScale;

    // 应用缩放
    QGraphicsView::scale(scaleFactor, scaleFactor);

    // 更新缩放比例
    m_zoomScale = scale;

    // 发送信号
    emit zoomChanged(m_zoomScale);
}

/**
 * @brief 放大视图
 */
void GraphicsView::zoomIn()
{
    setZoomScale(m_zoomScale * m_zoomStep);
}

/**
 * @brief 缩小视图
 */
void GraphicsView::zoomOut()
{
    setZoomScale(m_zoomScale / m_zoomStep);
}

/**
 * @brief 重置视图缩放
 */
void GraphicsView::resetZoom()
{
    setZoomScale(1.0);
    centerView();
}

/**
 * @brief 适应视图大小
 */
void GraphicsView::fitInView()
{
    if (!scene()) {
        return;
    }

    // 适应场景中的所有项
    QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);

    // 更新缩放比例
    m_zoomScale = transform().m11();

    // 发送信号
    emit zoomChanged(m_zoomScale);
}

/**
 * @brief 居中视图
 */
void GraphicsView::centerView()
{
    if (!scene()) {
        return;
    }

    centerOn(scene()->sceneRect().center());
}

/**
 * @brief 鼠标滚轮事件处理
 * @param event 滚轮事件
 */
void GraphicsView::wheelEvent(QWheelEvent* event)
{
    if (!m_zoomingEnabled) {
        QGraphicsView::wheelEvent(event);
        return;
    }

    // 检查是否按住 Ctrl 键
    if (event->modifiers() & Qt::ControlModifier) {
        // 计算缩放因子
        qreal scaleFactor = m_zoomStep;
        if (event->angleDelta().y() < 0) {
            scaleFactor = 1.0 / scaleFactor;
        }

        // 计算新的缩放比例
        qreal newScale = m_zoomScale * scaleFactor;

        // 应用缩放
        setZoomScale(newScale);
    } else {
        // 默认行为：滚动
        QGraphicsView::wheelEvent(event);
    }
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件
 */
void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (m_panningEnabled && event->button() == m_panButton) {
        // 开始平移
        m_isPanning = true;
        m_panStartPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }

    QGraphicsView::mousePressEvent(event);
}

/**
 * @brief 鼠标移动事件处理
 * @param event 鼠标事件
 */
void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isPanning) {
        // 计算平移增量
        QPointF delta = event->pos() - m_panStartPos;

        // 平移视图
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        // 更新起始位置
        m_panStartPos = event->pos();

        // 发送信号
        emit viewPanned(delta);

        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件
 */
void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_isPanning && event->button() == m_panButton) {
        // 结束平移
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

/**
 * @brief 键盘按下事件处理
 * @param event 键盘事件
 */
void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            if (event->modifiers() & Qt::ControlModifier) {
                zoomIn();
                event->accept();
                return;
            }
            break;

        case Qt::Key_Minus:
            if (event->modifiers() & Qt::ControlModifier) {
                zoomOut();
                event->accept();
                return;
            }
            break;

        case Qt::Key_0:
            if (event->modifiers() & Qt::ControlModifier) {
                resetZoom();
                event->accept();
                return;
            }
            break;

        case Qt::Key_F:
            if (event->modifiers() & Qt::ControlModifier) {
                fitInView();
                event->accept();
                return;
            }
            break;
    }

    QGraphicsView::keyPressEvent(event);
}

/**
 * @brief 绘制背景事件处理
 * @param painter 绘制器
 * @param rect 绘制矩形
 */
void GraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
    // 绘制背景
    QGraphicsView::drawBackground(painter, rect);

    if (m_drawGrid) {
        // 绘制网格
        drawGrid(painter, rect);

        // 绘制坐标轴
        drawAxes(painter, rect);
    }
}

/**
 * @brief 绘制网格背景
 * @param painter 绘制器
 * @param rect 绘制矩形
 */
void GraphicsView::drawGrid(QPainter* painter, const QRectF& rect)
{
    painter->save();

    // 设置画笔
    QPen pen(m_gridColor, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    // 计算网格起始点
    qreal left = qFloor(rect.left() / m_gridSize) * m_gridSize;
    qreal top = qFloor(rect.top() / m_gridSize) * m_gridSize;

    // 绘制垂直线
    for (qreal x = left; x < rect.right(); x += m_gridSize) {
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
    }

    // 绘制水平线
    for (qreal y = top; y < rect.bottom(); y += m_gridSize) {
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    }

    painter->restore();
}

/**
 * @brief 绘制坐标轴
 * @param painter 绘制器
 * @param rect 绘制矩形
 */
void GraphicsView::drawAxes(QPainter* painter, const QRectF& rect)
{
    painter->save();

    // 设置画笔
    QPen pen(m_axisColor, 2);
    pen.setCosmetic(true);
    painter->setPen(pen);

    // 绘制X轴
    if (rect.top() <= 0 && rect.bottom() >= 0) {
        painter->drawLine(QPointF(rect.left(), 0), QPointF(rect.right(), 0));
    }

    // 绘制Y轴
    if (rect.left() <= 0 && rect.right() >= 0) {
        painter->drawLine(QPointF(0, rect.top()), QPointF(0, rect.bottom()));
    }

    painter->restore();
}
