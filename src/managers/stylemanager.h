#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QMap>
#include <QJsonObject>

/**
 * @brief 样式管理器类
 *
 * 该类用于管理概念图的样式，包括：
 * - 样式加载和保存
 * - 样式切换
 * - 颜色主题管理
 */
class StyleManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个样式管理器
     * @param parent 父对象
     */
    explicit StyleManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~StyleManager();

    /**
     * @brief 加载样式文件
     * @param filePath 样式文件路径
     * @return 如果成功加载返回 true，否则返回 false
     */
    bool loadStyle(const QString& filePath);

    /**
     * @brief 保存样式文件
     * @param filePath 样式文件路径
     * @return 如果成功保存返回 true，否则返回 false
     */
    bool saveStyle(const QString& filePath);

    /**
     * @brief 加载预设样式
     * @param styleName 样式名称
     * @return 如果成功加载返回 true，否则返回 false
     */
    bool loadPresetStyle(const QString& styleName);

    /**
     * @brief 获取所有预设样式名称
     * @return 样式名称列表
     */
    QStringList presetStyleNames() const;

    // 颜色获取方法
    /**
     * @brief 获取节点背景颜色
     * @return 节点背景颜色
     */
    QColor nodeBackgroundColor() const { return m_nodeBackgroundColor; }

    /**
     * @brief 获取节点边框颜色
     * @return 节点边框颜色
     */
    QColor nodeBorderColor() const { return m_nodeBorderColor; }

    /**
     * @brief 获取节点文本颜色
     * @return 节点文本颜色
     */
    QColor nodeTextColor() const { return m_nodeTextColor; }

    /**
     * @brief 获取连接线颜色
     * @return 连接线颜色
     */
    QColor edgeColor() const { return m_edgeColor; }

    /**
     * @brief 获取选中颜色
     * @return 选中颜色
     */
    QColor selectionColor() const { return m_selectionColor; }

    /**
     * @brief 获取悬停颜色
     * @return 悬停颜色
     */
    QColor hoverColor() const { return m_hoverColor; }

    /**
     * @brief 获取背景颜色
     * @return 背景颜色
     */
    QColor backgroundColor() const { return m_backgroundColor; }

    /**
     * @brief 获取网格颜色
     * @return 网格颜色
     */
    QColor gridColor() const { return m_gridColor; }

    // 颜色设置方法
    /**
     * @brief 设置节点背景颜色
     * @param color 新的颜色
     */
    void setNodeBackgroundColor(const QColor& color) { m_nodeBackgroundColor = color; }

    /**
     * @brief 设置节点边框颜色
     * @param color 新的颜色
     */
    void setNodeBorderColor(const QColor& color) { m_nodeBorderColor = color; }

    /**
     * @brief 设置节点文本颜色
     * @param color 新的颜色
     */
    void setNodeTextColor(const QColor& color) { m_nodeTextColor = color; }

    /**
     * @brief 设置连接线颜色
     * @param color 新的颜色
     */
    void setEdgeColor(const QColor& color) { m_edgeColor = color; }

    /**
     * @brief 设置选中颜色
     * @param color 新的颜色
     */
    void setSelectionColor(const QColor& color) { m_selectionColor = color; }

    /**
     * @brief 设置悬停颜色
     * @param color 新的颜色
     */
    void setHoverColor(const QColor& color) { m_hoverColor = color; }

    /**
     * @brief 设置背景颜色
     * @param color 新的颜色
     */
    void setBackgroundColor(const QColor& color) { m_backgroundColor = color; }

    /**
     * @brief 设置网格颜色
     * @param color 新的颜色
     */
    void setGridColor(const QColor& color) { m_gridColor = color; }

    /**
     * @brief 重置为默认样式
     */
    void resetToDefault();

    /**
     * @brief 获取当前样式名称
     * @return 样式名称
     */
    QString currentStyleName() const { return m_currentStyleName; }

signals:
    /**
     * @brief 样式变化信号
     */
    void styleChanged();

private:
    /**
     * @brief 从JSON对象加载样式
     * @param json JSON对象
     */
    void loadFromJson(const QJsonObject& json);

    /**
     * @brief 保存到JSON对象
     * @return JSON对象
     */
    QJsonObject saveToJson() const;

    QString m_currentStyleName;          // 当前样式名称
    QColor m_nodeBackgroundColor;        // 节点背景颜色
    QColor m_nodeBorderColor;             // 节点边框颜色
    QColor m_nodeTextColor;               // 节点文本颜色
    QColor m_edgeColor;                   // 连接线颜色
    QColor m_selectionColor;             // 选中颜色
    QColor m_hoverColor;                  // 悬停颜色
    QColor m_backgroundColor;             // 背景颜色
    QColor m_gridColor;                   // 网格颜色
    QMap<QString, QString> m_presetStyles; // 预设样式映射
};

#endif // STYLEMANAGER_H
