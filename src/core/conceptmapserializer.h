#ifndef CONCEPTMAPSERIALIZER_H
#define CONCEPTMAPSERIALIZER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QIODevice>

#include "conceptmap.h"

/**
 * @brief 概念图序列化类
 * 
 * 该类负责概念图的序列化和反序列化，支持 JSON 和 XML 格式。
 * 消除了 MapModel 和 FileManager 中的代码重复。
 * 
 * @author AI Assistant
 * @version 1.0.0
 */
class ConceptMapSerializer
{
public:
    /**
     * @brief 从 JSON 文件加载概念图
     * @param filePath 文件路径
     * @param map 概念图对象
     * @return 是否成功加载
     */
    static bool loadFromJson(const QString& filePath, ConceptMap& map);
    
    /**
     * @brief 保存概念图到 JSON 文件
     * @param filePath 文件路径
     * @param map 概念图对象
     * @return 是否成功保存
     */
    static bool saveToJson(const QString& filePath, const ConceptMap& map);
    
    /**
     * @brief 从 XML 文件加载概念图
     * @param filePath 文件路径
     * @param map 概念图对象
     * @return 是否成功加载
     */
    static bool loadFromXml(const QString& filePath, ConceptMap& map);
    
    /**
     * @brief 保存概念图到 XML 文件
     * @param filePath 文件路径
     * @param map 概念图对象
     * @return 是否成功保存
     */
    static bool saveToXml(const QString& filePath, const ConceptMap& map);
};

#endif // CONCEPTMAPSERIALIZER_H
