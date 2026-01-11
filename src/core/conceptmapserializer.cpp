#include "conceptmapserializer.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

/**
 * @brief 从 JSON 文件加载概念图
 * @param filePath 文件路径
 * @param map 概念图对象
 * @return 是否成功加载
 */
bool ConceptMapSerializer::loadFromJson(const QString& filePath, ConceptMap& map)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "无效的JSON格式";
        return false;
    }
    
    QJsonObject rootObj = doc.object();
    
    // 读取节点数据
    QJsonArray nodesArray = rootObj["nodes"].toArray();
    QVector<ConceptNode> nodes;
    for (const QJsonValue& value : nodesArray) {
        QJsonObject nodeObj = value.toObject();
        QString id = nodeObj["id"].toString();
        QString text = nodeObj["text"].toString();
        qreal x = nodeObj["x"].toDouble();
        qreal y = nodeObj["y"].toDouble();
        qreal width = nodeObj["width"].toDouble();
        qreal height = nodeObj["height"].toDouble();
        int red = nodeObj["color"].toObject()["red"].toInt();
        int green = nodeObj["color"].toObject()["green"].toInt();
        int blue = nodeObj["color"].toObject()["blue"].toInt();
        QString style = nodeObj["style"].toString();
        
        nodes.append(ConceptNode(text, x, y, width, height, QColor(red, green, blue)));
        nodes.last().setId(id);
        nodes.last().setStyle(style);
    }
    
    // 读取连接线数据
    QJsonArray edgesArray = rootObj["edges"].toArray();
    QVector<ConceptEdge> edges;
    for (const QJsonValue& value : edgesArray) {
        QJsonObject edgeObj = value.toObject();
        QString id = edgeObj["id"].toString();
        QString sourceId = edgeObj["sourceId"].toString();
        QString targetId = edgeObj["targetId"].toString();
        QString label = edgeObj["label"].toString();
        int red = edgeObj["color"].toObject()["red"].toInt();
        int green = edgeObj["color"].toObject()["green"].toInt();
        int blue = edgeObj["color"].toObject()["blue"].toInt();
        QString style = edgeObj["style"].toString();
        
        edges.append(ConceptEdge(sourceId, targetId, label, QColor(red, green, blue)));
        edges.last().setId(id);
        edges.last().setStyle(style);
    }
    
    // 设置概念图数据
    for (const ConceptNode& node : nodes) {
        map.addNode(node);
    }
    for (const ConceptEdge& edge : edges) {
        map.addEdge(edge);
    }
    
    return true;
}

/**
 * @brief 保存概念图到 JSON 文件
 * @param filePath 文件路径
 * @param map 概念图对象
 * @return 是否成功保存
 */
bool ConceptMapSerializer::saveToJson(const QString& filePath, const ConceptMap& map)
{
    QJsonObject rootObj;
    
    // 写入节点数据
    QJsonArray nodesArray;
    for (const ConceptNode& node : map.nodes()) {
        QJsonObject nodeObj;
        nodeObj["id"] = node.id();
        nodeObj["text"] = node.text();
        nodeObj["x"] = node.x();
        nodeObj["y"] = node.y();
        nodeObj["width"] = node.width();
        nodeObj["height"] = node.height();
        
        QJsonObject colorObj;
        colorObj["red"] = node.color().red();
        colorObj["green"] = node.color().green();
        colorObj["blue"] = node.color().blue();
        nodeObj["color"] = colorObj;
        
        nodeObj["style"] = node.style();
        nodesArray.append(nodeObj);
    }
    rootObj["nodes"] = nodesArray;
    
    // 写入连接线数据
    QJsonArray edgesArray;
    for (const ConceptEdge& edge : map.edges()) {
        QJsonObject edgeObj;
        edgeObj["id"] = edge.id();
        edgeObj["sourceId"] = edge.sourceNodeId();
        edgeObj["targetId"] = edge.targetNodeId();
        edgeObj["label"] = edge.label();
        
        QJsonObject colorObj;
        colorObj["red"] = edge.color().red();
        colorObj["green"] = edge.color().green();
        colorObj["blue"] = edge.color().blue();
        edgeObj["color"] = colorObj;
        
        edgeObj["style"] = edge.style();
        edgesArray.append(edgeObj);
    }
    rootObj["edges"] = edgesArray;
    
    // 写入文件
    QJsonDocument doc(rootObj);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法创建文件:" << filePath;
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    
    return true;
}

/**
 * @brief 从 XML 文件加载概念图
 * @param filePath 文件路径
 * @param map 概念图对象
 * @return 是否成功加载
 */
bool ConceptMapSerializer::loadFromXml(const QString& filePath, ConceptMap& map)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return false;
    }
    
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "XML解析错误:" << errorMsg << "行:" << errorLine << "列:" << errorColumn;
        return false;
    }
    
    QDomElement root = doc.documentElement();
    
    // 读取节点数据
    QVector<ConceptNode> nodes;
    QDomNodeList nodeElements = root.elementsByTagName("node");
    for (const QDomNode& nodeNode : nodeElements) {
        QDomElement nodeElement = nodeNode.toElement();
        QString id = nodeElement.attribute("id");
        QString text = nodeElement.attribute("text");
        qreal x = nodeElement.attribute("x").toDouble();
        qreal y = nodeElement.attribute("y").toDouble();
        qreal width = nodeElement.attribute("width").toDouble();
        qreal height = nodeElement.attribute("height").toDouble();
        int red = nodeElement.attribute("color").split(",")[0].toInt();
        int green = nodeElement.attribute("color").split(",")[1].toInt();
        int blue = nodeElement.attribute("color").split(",")[2].toInt();
        QString style = nodeElement.attribute("style");
        
        nodes.append(ConceptNode(text, x, y, width, height, QColor(red, green, blue)));
        nodes.last().setId(id);
        nodes.last().setStyle(style);
    }
    
    // 读取连接线数据
    QVector<ConceptEdge> edges;
    QDomNodeList edgeElements = root.elementsByTagName("edge");
    for (const QDomNode& edgeNode : edgeElements) {
        QDomElement edgeElement = edgeNode.toElement();
        QString id = edgeElement.attribute("id");
        QString sourceId = edgeElement.attribute("sourceId");
        QString targetId = edgeElement.attribute("targetId");
        QString label = edgeElement.attribute("label");
        int red = edgeElement.attribute("color").split(",")[0].toInt();
        int green = edgeElement.attribute("color").split(",")[1].toInt();
        int blue = edgeElement.attribute("color").split(",")[2].toInt();
        QString style = edgeElement.attribute("style");
        
        edges.append(ConceptEdge(sourceId, targetId, label, QColor(red, green, blue)));
        edges.last().setId(id);
        edges.last().setStyle(style);
    }
    
    // 设置概念图数据
    for (const ConceptNode& node : nodes) {
        map.addNode(node);
    }
    for (const ConceptEdge& edge : edges) {
        map.addEdge(edge);
    }
    
    return true;
}

/**
 * @brief 保存概念图到 XML 文件
 * @param filePath 文件路径
 * @param map 概念图对象
 * @return 是否成功保存
 */
bool ConceptMapSerializer::saveToXml(const QString& filePath, const ConceptMap& map)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("conceptmap");
    doc.appendChild(root);
    
    // 写入节点数据
    for (const ConceptNode& node : map.nodes()) {
        QDomElement nodeElement = doc.createElement("node");
        nodeElement.setAttribute("id", node.id());
        nodeElement.setAttribute("text", node.text());
        nodeElement.setAttribute("x", node.x());
        nodeElement.setAttribute("y", node.y());
        nodeElement.setAttribute("width", node.width());
        nodeElement.setAttribute("height", node.height());
        nodeElement.setAttribute("color", QString("%1,%2,%3").arg(node.color().red()).arg(node.color().green()).arg(node.color().blue()));
        nodeElement.setAttribute("style", node.style());
        root.appendChild(nodeElement);
    }
    
    // 写入连接线数据
    for (const ConceptEdge& edge : map.edges()) {
        QDomElement edgeElement = doc.createElement("edge");
        edgeElement.setAttribute("id", edge.id());
        edgeElement.setAttribute("sourceId", edge.sourceNodeId());
        edgeElement.setAttribute("targetId", edge.targetNodeId());
        edgeElement.setAttribute("label", edge.label());
        edgeElement.setAttribute("color", QString("%1,%2,%3").arg(edge.color().red()).arg(edge.color().green()).arg(edge.color().blue()));
        edgeElement.setAttribute("style", edge.style());
        root.appendChild(edgeElement);
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
