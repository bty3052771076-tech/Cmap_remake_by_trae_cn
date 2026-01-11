#include "filemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QPainter>
#include <QSvgGenerator>
#include <QPrinter>
#include <QFileInfo>
#include <QDebug>

/**
 * @brief 构造函数 - 创建一个文件管理器
 * @param parent 父对象
 */
FileManager::FileManager(QObject* parent)
    : QObject(parent)
    , m_maxRecentFiles(10)
{
}

/**
 * @brief 析构函数
 */
FileManager::~FileManager()
{
}

/**
 * @brief 加载概念图文件
 * @param filePath 文件路径
 * @param map 概念图数据（输出参数）
 * @return 如果成功加载返回 true，否则返回 false
 */
bool FileManager::loadMap(const QString& filePath, ConceptMap& map)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "json") {
        return loadJson(filePath, map);
    } else if (extension == "xml") {
        return loadXml(filePath, map);
    } else {
        qWarning() << "不支持的文件格式:" << extension;
        return false;
    }
}

/**
 * @brief 保存概念图文件
 * @param filePath 文件路径
 * @param map 概念图数据
 * @return 如果成功保存返回 true，否则返回 false
 */
bool FileManager::saveMap(const QString& filePath, const ConceptMap& map)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "json") {
        return saveJson(filePath, map);
    } else if (extension == "xml") {
        return saveXml(filePath, map);
    } else {
        qWarning() << "不支持的文件格式:" << extension;
        return false;
    }
}

/**
 * @brief 导出为图片
 * @param filePath 图片文件路径
 * @param scene 图形场景
 * @return 如果成功导出返回 true，否则返回 false
 */
bool FileManager::exportToImage(const QString& filePath, QGraphicsScene* scene)
{
    if (!scene) {
        qWarning() << "场景为空";
        return false;
    }

    // 创建图片
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // 绘制场景
    QPainter painter(&image);
    scene->render(&painter);
    painter.end();

    // 保存图片
    if (!image.save(filePath)) {
        qWarning() << "无法保存图片:" << filePath;
        return false;
    }

    return true;
}

/**
 * @brief 导出为Cmap格式
 * @param filePath Cmap文件路径
 * @param map 概念图数据
 * @return 如果成功导出返回 true，否则返回 false
 */
bool FileManager::exportToCmap(const QString& filePath, const ConceptMap& map)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("cmap");
    doc.appendChild(root);
    
    // 设置cmap基本属性
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xsi:noNamespaceSchemaLocation", "http://cmap.ihmc.us/xml/cmap/CMAP.xsd");
    root.setAttribute("version", "1.0");
    root.setAttribute("name", "Concept Map");
    
    // 创建概念图内容
    QDomElement conceptMap = doc.createElement("conceptmap");
    root.appendChild(conceptMap);
    
    // 创建概念节点
    QDomElement concepts = doc.createElement("concepts");
    conceptMap.appendChild(concepts);
    
    // 写入节点数据
    for (const ConceptNode& node : map.nodes()) {
        QDomElement concept = doc.createElement("concept");
        concept.setAttribute("id", node.id());
        concept.setAttribute("label", node.text());
        
        // 位置信息
        QDomElement location = doc.createElement("location");
        location.setAttribute("x", QString::number(node.x() + node.width() / 2.0));
        location.setAttribute("y", QString::number(node.y() + node.height() / 2.0));
        concept.appendChild(location);
        
        concepts.appendChild(concept);
    }
    
    // 创建连接线
    QDomElement connections = doc.createElement("connections");
    conceptMap.appendChild(connections);
    
    // 写入连接线数据
    for (const ConceptEdge& edge : map.edges()) {
        QDomElement connection = doc.createElement("connection");
        connection.setAttribute("id", edge.id());
        connection.setAttribute("sourceId", edge.sourceNodeId());
        connection.setAttribute("targetId", edge.targetNodeId());
        
        // 连接语
        QDomElement linkLabel = doc.createElement("linkLabel");
        linkLabel.setAttribute("text", edge.label());
        connection.appendChild(linkLabel);
        
        connections.appendChild(connection);
    }
    
    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法创建文件:" << filePath;
        return false;
    }
    
    file.write(doc.toString().toUtf8());
    file.close();
    
    return true;
}

/**
 * @brief 导出为PDF
 * @param filePath PDF文件路径
 * @param scene 图形场景
 * @return 如果成功导出返回 true，否则返回 false
 */
bool FileManager::exportToPDF(const QString& filePath, QGraphicsScene* scene)
{
    if (!scene) {
        qWarning() << "场景为空";
        return false;
    }

    // 创建打印机
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(scene->sceneRect().size(), QPageSize::Point));

    // 绘制场景
    QPainter painter(&printer);
    scene->render(&painter);
    painter.end();

    return true;
}

/**
 * @brief 导出为SVG
 * @param filePath SVG文件路径
 * @param scene 图形场景
 * @return 如果成功导出返回 true，否则返回 false
 */
bool FileManager::exportToSVG(const QString& filePath, QGraphicsScene* scene)
{
    if (!scene) {
        qWarning() << "场景为空";
        return false;
    }

    // 创建SVG生成器
    QSvgGenerator generator;
    generator.setFileName(filePath);
    generator.setSize(scene->sceneRect().size().toSize());
    generator.setViewBox(scene->sceneRect());

    // 绘制场景
    QPainter painter(&generator);
    scene->render(&painter);
    painter.end();

    return true;
}

/**
 * @brief 添加到最近文件列表
 * @param filePath 文件路径
 */
void FileManager::addToRecentFiles(const QString& filePath)
{
    // 移除已存在的文件
    m_recentFiles.removeAll(filePath);

    // 添加到列表开头
    m_recentFiles.prepend(filePath);

    // 限制列表大小
    while (m_recentFiles.size() > m_maxRecentFiles) {
        m_recentFiles.removeLast();
    }

    emit recentFilesChanged();
}

/**
 * @brief 清空最近文件列表
 */
void FileManager::clearRecentFiles()
{
    m_recentFiles.clear();
    emit recentFilesChanged();
}

/**
 * @brief 获取支持的文件过滤器
 * @return 文件过滤器字符串
 */
QString FileManager::fileFilter()
{
    return "概念图文件 (*.json *.xml);;JSON 文件 (*.json);;XML 文件 (*.xml)";
}

/**
 * @brief 获取支持的图片格式过滤器
 * @return 图片格式过滤器字符串
 */
QString FileManager::imageFilter()
{
    return "图片文件 (*.png *.jpg *.jpeg *.bmp *.svg);;PNG 文件 (*.png);;JPEG 文件 (*.jpg *.jpeg);;BMP 文件 (*.bmp);;SVG 文件 (*.svg)";
}

/**
 * @brief 检查文件是否为概念图文件
 * @param filePath 文件路径
 * @return 如果是概念图文件返回 true，否则返回 false
 */
bool FileManager::isConceptMapFile(const QString& filePath)
{
    QString extension = fileExtension(filePath).toLower();
    return extension == "json" || extension == "xml";
}

/**
 * @brief 获取文件扩展名
 * @param filePath 文件路径
 * @return 文件扩展名
 */
QString FileManager::fileExtension(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix();
}

/**
 * @brief 加载JSON格式文件
 * @param filePath 文件路径
 * @param map 概念图数据（输出参数）
 * @return 如果成功加载返回 true，否则返回 false
 */
bool FileManager::loadJson(const QString& filePath, ConceptMap& map)
{
    try {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("无法打开文件: " + filePath.toStdString());
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) {
            throw std::runtime_error("无效的JSON格式");
        }

        QJsonObject root = doc.object();

        // 读取概念图信息
        map.setName(root["name"].toString("未命名概念图"));
        map.setVersion(root["version"].toString("1.0"));

        // 清空现有数据
        map.clear();

        // 读取节点
        QJsonArray nodesArray = root["nodes"].toArray();
        for (const QJsonValue& value : nodesArray) {
            QJsonObject nodeObj = value.toObject();

            ConceptNode node(
                nodeObj["text"].toString(),
                nodeObj["x"].toDouble(),
                nodeObj["y"].toDouble(),
                nodeObj["width"].toDouble(),
                nodeObj["height"].toDouble(),
                QColor(nodeObj["color"].toString())
            );

            node.setId(nodeObj["id"].toString());
            node.setStyle(nodeObj["style"].toString());

            map.addNode(node);
        }

        // 读取连接线
        QJsonArray edgesArray = root["edges"].toArray();
        for (const QJsonValue& value : edgesArray) {
            QJsonObject edgeObj = value.toObject();

            ConceptEdge edge(
                edgeObj["sourceNodeId"].toString(),
                edgeObj["targetNodeId"].toString(),
                edgeObj["label"].toString(),
                QColor(edgeObj["color"].toString())
            );

            edge.setId(edgeObj["id"].toString());
            edge.setStyle(edgeObj["style"].toString());

            map.addEdge(edge);
        }

        return true;
    } catch (const std::exception& e) {
        qWarning() << "加载文件失败:" << e.what();
        return false;
    } catch (...) {
        qWarning() << "未知错误: 加载文件失败";
        return false;
    }
}

/**
 * @brief 保存为JSON格式文件
 * @param filePath 文件路径
 * @param map 概念图数据
 * @return 如果成功保存返回 true，否则返回 false
 */
bool FileManager::saveJson(const QString& filePath, const ConceptMap& map)
{
    QJsonObject root;

    // 保存概念图信息
    root["name"] = map.name();
    root["version"] = map.version();

    // 保存节点
    QJsonArray nodesArray;
    for (const ConceptNode& node : map.nodes()) {
        QJsonObject nodeObj;
        nodeObj["id"] = node.id();
        nodeObj["text"] = node.text();
        nodeObj["x"] = node.x();
        nodeObj["y"] = node.y();
        nodeObj["width"] = node.width();
        nodeObj["height"] = node.height();
        nodeObj["color"] = node.color().name();
        nodeObj["style"] = node.style();
        nodesArray.append(nodeObj);
    }
    root["nodes"] = nodesArray;

    // 保存连接线
    QJsonArray edgesArray;
    for (const ConceptEdge& edge : map.edges()) {
        QJsonObject edgeObj;
        edgeObj["id"] = edge.id();
        edgeObj["sourceNodeId"] = edge.sourceNodeId();
        edgeObj["targetNodeId"] = edge.targetNodeId();
        edgeObj["label"] = edge.label();
        edgeObj["color"] = edge.color().name();
        edgeObj["style"] = edge.style();
        edgesArray.append(edgeObj);
    }
    root["edges"] = edgesArray;

    // 保存到文件
    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

/**
 * @brief 加载XML格式文件
 * @param filePath 文件路径
 * @param map 概念图数据（输出参数）
 * @return 如果成功加载返回 true，否则返回 false
 */
bool FileManager::loadXml(const QString& filePath, ConceptMap& map)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qWarning() << "无效的XML格式";
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();

    // 读取概念图信息
    map.setName(root.attribute("name", "未命名概念图"));
    map.setVersion(root.attribute("version", "1.0"));

    // 清空现有数据
    map.clear();

    // 读取节点
    QDomNodeList nodesList = root.elementsByTagName("node");
    for (int i = 0; i < nodesList.size(); ++i) {
        QDomElement nodeElement = nodesList.at(i).toElement();

        ConceptNode node(
            nodeElement.attribute("text"),
            nodeElement.attribute("x").toDouble(),
            nodeElement.attribute("y").toDouble(),
            nodeElement.attribute("width").toDouble(),
            nodeElement.attribute("height").toDouble(),
            QColor(nodeElement.attribute("color"))
        );

        node.setId(nodeElement.attribute("id"));
        node.setStyle(nodeElement.attribute("style"));

        map.addNode(node);
    }

    // 读取连接线
    QDomNodeList edgesList = root.elementsByTagName("edge");
    for (int i = 0; i < edgesList.size(); ++i) {
        QDomElement edgeElement = edgesList.at(i).toElement();

        ConceptEdge edge(
            edgeElement.attribute("sourceNodeId"),
            edgeElement.attribute("targetNodeId"),
            edgeElement.attribute("label"),
            QColor(edgeElement.attribute("color"))
        );

        edge.setId(edgeElement.attribute("id"));
        edge.setStyle(edgeElement.attribute("style"));

        map.addEdge(edge);
    }

    return true;
}

/**
 * @brief 保存为XML格式文件
 * @param filePath 文件路径
 * @param map 概念图数据
 * @return 如果成功保存返回 true，否则返回 false
 */
bool FileManager::saveXml(const QString& filePath, const ConceptMap& map)
{
    QDomDocument doc;

    // 创建根元素
    QDomElement root = doc.createElement("conceptMap");
    root.setAttribute("name", map.name());
    root.setAttribute("version", map.version());
    doc.appendChild(root);

    // 保存节点
    for (const ConceptNode& node : map.nodes()) {
        QDomElement nodeElement = doc.createElement("node");
        nodeElement.setAttribute("id", node.id());
        nodeElement.setAttribute("text", node.text());
        nodeElement.setAttribute("x", node.x());
        nodeElement.setAttribute("y", node.y());
        nodeElement.setAttribute("width", node.width());
        nodeElement.setAttribute("height", node.height());
        nodeElement.setAttribute("color", node.color().name());
        nodeElement.setAttribute("style", node.style());
        root.appendChild(nodeElement);
    }

    // 保存连接线
    for (const ConceptEdge& edge : map.edges()) {
        QDomElement edgeElement = doc.createElement("edge");
        edgeElement.setAttribute("id", edge.id());
        edgeElement.setAttribute("sourceNodeId", edge.sourceNodeId());
        edgeElement.setAttribute("targetNodeId", edge.targetNodeId());
        edgeElement.setAttribute("label", edge.label());
        edgeElement.setAttribute("color", edge.color().name());
        edgeElement.setAttribute("style", edge.style());
        root.appendChild(edgeElement);
    }

    // 保存到文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    return true;
}
