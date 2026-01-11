#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QGraphicsScene>
#include "conceptmap.h"

/**
 * @brief 文件管理器类
 *
 * 该类用于管理概念图文件的读写操作，包括：
 * - 文件加载
 * - 文件保存
 * - 文件格式转换
 * - 最近文件管理
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数 - 创建一个文件管理器
     * @param parent 父对象
     */
    explicit FileManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~FileManager();

    /**
     * @brief 加载概念图文件
     * @param filePath 文件路径
     * @param map 概念图数据（输出参数）
     * @return 如果成功加载返回 true，否则返回 false
     */
    bool loadMap(const QString& filePath, ConceptMap& map);

    /**
     * @brief 保存概念图文件
     * @param filePath 文件路径
     * @param map 概念图数据
     * @return 如果成功保存返回 true，否则返回 false
     */
    bool saveMap(const QString& filePath, const ConceptMap& map);

    /**
     * @brief 导出为图片
     * @param filePath 图片文件路径
     * @param scene 图形场景
     * @return 如果成功导出返回 true，否则返回 false
     */
    bool exportToImage(const QString& filePath, QGraphicsScene* scene);

    /**
     * @brief 导出为PDF
     * @param filePath PDF文件路径
     * @param scene 图形场景
     * @return 如果成功导出返回 true，否则返回 false
     */
    bool exportToPDF(const QString& filePath, QGraphicsScene* scene);

    /**
     * @brief 导出为SVG
     * @param filePath SVG文件路径
     * @param scene 图形场景
     * @return 如果成功导出返回 true，否则返回 false
     */
    bool exportToSVG(const QString& filePath, QGraphicsScene* scene);

    /**
     * @brief 导出为Cmap格式
     * @param filePath Cmap文件路径
     * @param map 概念图数据
     * @return 如果成功导出返回 true，否则返回 false
     */
    bool exportToCmap(const QString& filePath, const ConceptMap& map);

    /**
     * @brief 添加到最近文件列表
     * @param filePath 文件路径
     */
    void addToRecentFiles(const QString& filePath);

    /**
     * @brief 获取最近文件列表
     * @return 最近文件列表
     */
    QStringList recentFiles() const { return m_recentFiles; }

    /**
     * @brief 清空最近文件列表
     */
    void clearRecentFiles();

    /**
     * @brief 设置最近文件最大数量
     * @param maxCount 最大数量
     */
    void setMaxRecentFiles(int maxCount) { m_maxRecentFiles = maxCount; }

    /**
     * @brief 获取最近文件最大数量
     * @return 最大数量
     */
    int maxRecentFiles() const { return m_maxRecentFiles; }

    /**
     * @brief 获取支持的文件过滤器
     * @return 文件过滤器字符串
     */
    static QString fileFilter();

    /**
     * @brief 获取支持的图片格式过滤器
     * @return 图片格式过滤器字符串
     */
    static QString imageFilter();

    /**
     * @brief 检查文件是否为概念图文件
     * @param filePath 文件路径
     * @return 如果是概念图文件返回 true，否则返回 false
     */
    static bool isConceptMapFile(const QString& filePath);

    /**
     * @brief 获取文件扩展名
     * @param filePath 文件路径
     * @return 文件扩展名
     */
    static QString fileExtension(const QString& filePath);

signals:
    /**
     * @brief 最近文件列表变化信号
     */
    void recentFilesChanged();

private:
    /**
     * @brief 加载JSON格式文件
     * @param filePath 文件路径
     * @param map 概念图数据（输出参数）
     * @return 如果成功加载返回 true，否则返回 false
     */
    bool loadJson(const QString& filePath, ConceptMap& map);

    /**
     * @brief 保存为JSON格式文件
     * @param filePath 文件路径
     * @param map 概念图数据
     * @return 如果成功保存返回 true，否则返回 false
     */
    bool saveJson(const QString& filePath, const ConceptMap& map);

    /**
     * @brief 加载XML格式文件
     * @param filePath 文件路径
     * @param map 概念图数据（输出参数）
     * @return 如果成功加载返回 true，否则返回 false
     */
    bool loadXml(const QString& filePath, ConceptMap& map);

    /**
     * @brief 保存为XML格式文件
     * @param filePath 文件路径
     * @param map 概念图数据
     * @return 如果成功保存返回 true，否则返回 false
     */
    bool saveXml(const QString& filePath, const ConceptMap& map);

    QStringList m_recentFiles;     // 最近文件列表
    int m_maxRecentFiles;          // 最近文件最大数量
};

#endif // FILEMANAGER_H
