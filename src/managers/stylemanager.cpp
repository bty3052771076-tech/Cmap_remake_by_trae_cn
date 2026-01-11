#include "stylemanager.h"
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个样式管理器
 * @param parent 父对象
 */
StyleManager::StyleManager(QObject* parent)
    : QObject(parent)
    , m_currentStyleName("默认")
    , m_nodeBackgroundColor(200, 220, 240)
    , m_nodeBorderColor(80, 80, 80)
    , m_nodeTextColor(0, 0, 0)
    , m_edgeColor(100, 100, 100)
    , m_selectionColor(0, 120, 215)
    , m_hoverColor(0, 120, 215, 100)
    , m_backgroundColor(250, 250, 250)
    , m_gridColor(220, 220, 220)
{
    // 初始化预设样式
    m_presetStyles["学术蓝"] = ":/styles/academic_blue.json";
    m_presetStyles["自然绿"] = ":/styles/nature_green.json";
    m_presetStyles["活力橙"] = ":/styles/vibrant_orange.json";
}

/**
 * @brief 析构函数
 */
StyleManager::~StyleManager()
{
}

/**
 * @brief 加载样式文件
 * @param filePath 样式文件路径
 * @return 如果成功加载返回 true，否则返回 false
 */
bool StyleManager::loadStyle(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开样式文件:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "无效的JSON格式";
        return false;
    }

    loadFromJson(doc.object());
    m_currentStyleName = QFileInfo(filePath).baseName();

    emit styleChanged();

    return true;
}

/**
 * @brief 保存样式文件
 * @param filePath 样式文件路径
 * @return 如果成功保存返回 true，否则返回 false
 */
bool StyleManager::saveStyle(const QString& filePath)
{
    QJsonObject json = saveToJson();

    QJsonDocument doc(json);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法打开样式文件:" << filePath;
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

/**
 * @brief 加载预设样式
 * @param styleName 样式名称
 * @return 如果成功加载返回 true，否则返回 false
 */
bool StyleManager::loadPresetStyle(const QString& styleName)
{
    if (!m_presetStyles.contains(styleName)) {
        qWarning() << "未找到预设样式:" << styleName;
        return false;
    }

    return loadStyle(m_presetStyles[styleName]);
}

/**
 * @brief 获取所有预设样式名称
 * @return 样式名称列表
 */
QStringList StyleManager::presetStyleNames() const
{
    return m_presetStyles.keys();
}

/**
 * @brief 重置为默认样式
 */
void StyleManager::resetToDefault()
{
    m_currentStyleName = "默认";
    m_nodeBackgroundColor = QColor(200, 220, 240);
    m_nodeBorderColor = QColor(80, 80, 80);
    m_nodeTextColor = QColor(0, 0, 0);
    m_edgeColor = QColor(100, 100, 100);
    m_selectionColor = QColor(0, 120, 215);
    m_hoverColor = QColor(0, 120, 215, 100);
    m_backgroundColor = QColor(250, 250, 250);
    m_gridColor = QColor(220, 220, 220);

    emit styleChanged();
}

/**
 * @brief 从JSON对象加载样式
 * @param json JSON对象
 */
void StyleManager::loadFromJson(const QJsonObject& json)
{
    m_nodeBackgroundColor = QColor(json["nodeBackgroundColor"].toString("#C8DCF0"));
    m_nodeBorderColor = QColor(json["nodeBorderColor"].toString("#505050"));
    m_nodeTextColor = QColor(json["nodeTextColor"].toString("#000000"));
    m_edgeColor = QColor(json["edgeColor"].toString("#646464"));
    m_selectionColor = QColor(json["selectionColor"].toString("#0078D7"));
    m_hoverColor = QColor(json["hoverColor"].toString("#0078D766"));
    m_backgroundColor = QColor(json["backgroundColor"].toString("#FAFAFA"));
    m_gridColor = QColor(json["gridColor"].toString("#DCDCDC"));
}

/**
 * @brief 保存到JSON对象
 * @return JSON对象
 */
QJsonObject StyleManager::saveToJson() const
{
    QJsonObject json;
    json["nodeBackgroundColor"] = m_nodeBackgroundColor.name();
    json["nodeBorderColor"] = m_nodeBorderColor.name();
    json["nodeTextColor"] = m_nodeTextColor.name();
    json["edgeColor"] = m_edgeColor.name();
    json["selectionColor"] = m_selectionColor.name();
    json["hoverColor"] = m_hoverColor.name();
    json["backgroundColor"] = m_backgroundColor.name();
    json["gridColor"] = m_gridColor.name();

    return json;
}
