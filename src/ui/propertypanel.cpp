#include "propertypanel.h"

/**
 * @brief 构造函数 - 创建属性面板
 * @param parent 父窗口
 */
PropertyPanel::PropertyPanel(QWidget* parent)
    : QWidget(parent)
    , m_isNodeSelected(false)
{
    // 初始化UI
    initUI();

    // 连接信号和槽
    connectSignals();
}

/**
 * @brief 析构函数
 */
PropertyPanel::~PropertyPanel()
{
}

/**
 * @brief 初始化UI
 */
void PropertyPanel::initUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(10);

    // 创建节点属性组
    createNodeGroup();
    mainLayout->addWidget(m_nodeGroup);

    // 创建连接线属性组
    createEdgeGroup();
    mainLayout->addWidget(m_edgeGroup);

    // 添加弹性空间
    mainLayout->addStretch();

    // 初始状态：隐藏所有组
    m_nodeGroup->setVisible(false);
    m_edgeGroup->setVisible(false);
}

/**
 * @brief 创建节点属性组
 */
void PropertyPanel::createNodeGroup()
{
    m_nodeGroup = new QGroupBox("节点属性", this);
    QFormLayout* layout = new QFormLayout(m_nodeGroup);

    // 节点ID
    m_nodeIdEdit = new QLineEdit(m_nodeGroup);
    m_nodeIdEdit->setReadOnly(true);
    layout->addRow("ID:", m_nodeIdEdit);

    // 节点文本
    m_nodeTextEdit = new QLineEdit(m_nodeGroup);
    layout->addRow("文本:", m_nodeTextEdit);

    // 节点位置
    m_nodeXSpinBox = new QDoubleSpinBox(m_nodeGroup);
    m_nodeXSpinBox->setRange(-10000, 10000);
    m_nodeXSpinBox->setDecimals(2);
    layout->addRow("X:", m_nodeXSpinBox);

    m_nodeYSpinBox = new QDoubleSpinBox(m_nodeGroup);
    m_nodeYSpinBox->setRange(-10000, 10000);
    m_nodeYSpinBox->setDecimals(2);
    layout->addRow("Y:", m_nodeYSpinBox);

    // 节点尺寸
    m_nodeWidthSpinBox = new QDoubleSpinBox(m_nodeGroup);
    m_nodeWidthSpinBox->setRange(10, 1000);
    m_nodeWidthSpinBox->setDecimals(2);
    layout->addRow("宽度:", m_nodeWidthSpinBox);

    m_nodeHeightSpinBox = new QDoubleSpinBox(m_nodeGroup);
    m_nodeHeightSpinBox->setRange(10, 1000);
    m_nodeHeightSpinBox->setDecimals(2);
    layout->addRow("高度:", m_nodeHeightSpinBox);

    // 节点颜色
    QHBoxLayout* colorLayout = new QHBoxLayout();
    m_nodeColorButton = new QPushButton("选择颜色", m_nodeGroup);
    m_nodeColorLabel = new QLabel(m_nodeGroup);
    m_nodeColorLabel->setMinimumSize(50, 20);
    m_nodeColorLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
    colorLayout->addWidget(m_nodeColorButton);
    colorLayout->addWidget(m_nodeColorLabel);
    layout->addRow("颜色:", colorLayout);
}

/**
 * @brief 创建连接线属性组
 */
void PropertyPanel::createEdgeGroup()
{
    m_edgeGroup = new QGroupBox("连接线属性", this);
    QFormLayout* layout = new QFormLayout(m_edgeGroup);

    // 连接线ID
    m_edgeIdEdit = new QLineEdit(m_edgeGroup);
    m_edgeIdEdit->setReadOnly(true);
    layout->addRow("ID:", m_edgeIdEdit);

    // 源节点ID
    m_edgeSourceNodeIdEdit = new QLineEdit(m_edgeGroup);
    m_edgeSourceNodeIdEdit->setReadOnly(true);
    layout->addRow("源节点ID:", m_edgeSourceNodeIdEdit);

    // 目标节点ID
    m_edgeTargetNodeIdEdit = new QLineEdit(m_edgeGroup);
    m_edgeTargetNodeIdEdit->setReadOnly(true);
    layout->addRow("目标节点ID:", m_edgeTargetNodeIdEdit);

    // 连接线标签
    m_edgeLabelEdit = new QLineEdit(m_edgeGroup);
    layout->addRow("标签:", m_edgeLabelEdit);

    // 连接线颜色
    QHBoxLayout* colorLayout = new QHBoxLayout();
    m_edgeColorButton = new QPushButton("选择颜色", m_edgeGroup);
    m_edgeColorLabel = new QLabel(m_edgeGroup);
    m_edgeColorLabel->setMinimumSize(50, 20);
    m_edgeColorLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
    colorLayout->addWidget(m_edgeColorButton);
    colorLayout->addWidget(m_edgeColorLabel);
    layout->addRow("颜色:", colorLayout);
}

/**
 * @brief 连接信号和槽
 */
void PropertyPanel::connectSignals()
{
    // 节点文本变化
    connect(m_nodeTextEdit, &QLineEdit::textChanged, this, &PropertyPanel::nodeTextChanged);

    // 节点位置变化
    connect(m_nodeXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        if (m_isNodeSelected) {
            m_currentNode.setX(value);
            emit nodeTextChanged(m_currentNode.text());
        }
    });
    
    connect(m_nodeYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        if (m_isNodeSelected) {
            m_currentNode.setY(value);
            emit nodeTextChanged(m_currentNode.text());
        }
    });

    // 节点宽度变化
    connect(m_nodeWidthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        if (m_isNodeSelected) {
            m_currentNode.setWidth(value);
            emit nodeTextChanged(m_currentNode.text());
        }
    });
    
    connect(m_nodeHeightSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        if (m_isNodeSelected) {
            m_currentNode.setHeight(value);
            emit nodeTextChanged(m_currentNode.text());
        }
    });

    // 节点颜色按钮点击
    connect(m_nodeColorButton, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(m_currentNode.color(), this, "选择节点颜色");
        if (color.isValid()) {
            m_nodeColorLabel->setStyleSheet(QString("background-color: %1").arg(color.name()));
            emit nodeColorChanged(color);
        }
    });

    // 连接线标签变化
    connect(m_edgeLabelEdit, &QLineEdit::textChanged, this, &PropertyPanel::edgeLabelChanged);

    // 连接线颜色按钮点击
    connect(m_edgeColorButton, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(m_currentEdge.color(), this, "选择连接线颜色");
        if (color.isValid()) {
            m_edgeColorLabel->setStyleSheet(QString("background-color: %1").arg(color.name()));
            emit edgeColorChanged(color);
        }
    });
}

/**
 * @brief 设置节点
 * @param node 节点数据
 */
void PropertyPanel::setNode(const ConceptNode& node)
{
    m_currentNode = node;
    m_isNodeSelected = true;

    // 显示节点属性组，隐藏连接线属性组
    m_nodeGroup->setVisible(true);
    m_edgeGroup->setVisible(false);

    // 更新节点属性
    updateNodeProperties();
}

/**
 * @brief 设置连接线
 * @param edge 连接线数据
 */
void PropertyPanel::setEdge(const ConceptEdge& edge)
{
    m_currentEdge = edge;
    m_isNodeSelected = false;

    // 显示连接线属性组，隐藏节点属性组
    m_edgeGroup->setVisible(true);
    m_nodeGroup->setVisible(false);

    // 更新连接线属性
    updateEdgeProperties();
}

/**
 * @brief 清空属性面板
 */
void PropertyPanel::clear()
{
    m_nodeGroup->setVisible(false);
    m_edgeGroup->setVisible(false);
    m_isNodeSelected = false;
}

/**
 * @brief 更新节点属性
 */
void PropertyPanel::updateNodeProperties()
{
    // 停止信号发送
    m_nodeTextEdit->blockSignals(true);
    m_nodeXSpinBox->blockSignals(true);
    m_nodeYSpinBox->blockSignals(true);
    m_nodeWidthSpinBox->blockSignals(true);
    m_nodeHeightSpinBox->blockSignals(true);

    // 更新节点属性
    m_nodeIdEdit->setText(m_currentNode.id());
    m_nodeTextEdit->setText(m_currentNode.text());
    m_nodeXSpinBox->setValue(m_currentNode.x());
    m_nodeYSpinBox->setValue(m_currentNode.y());
    m_nodeWidthSpinBox->setValue(m_currentNode.width());
    m_nodeHeightSpinBox->setValue(m_currentNode.height());
    m_nodeColorLabel->setStyleSheet(QString("background-color: %1").arg(m_currentNode.color().name()));

    // 恢复信号发送
    m_nodeTextEdit->blockSignals(false);
    m_nodeXSpinBox->blockSignals(false);
    m_nodeYSpinBox->blockSignals(false);
    m_nodeWidthSpinBox->blockSignals(false);
    m_nodeHeightSpinBox->blockSignals(false);
}

/**
 * @brief 更新连接线属性
 */
void PropertyPanel::updateEdgeProperties()
{
    // 阻止信号发送
    m_edgeLabelEdit->blockSignals(true);

    // 更新连接线属性
    m_edgeIdEdit->setText(m_currentEdge.id());
    m_edgeSourceNodeIdEdit->setText(m_currentEdge.sourceNodeId());
    m_edgeTargetNodeIdEdit->setText(m_currentEdge.targetNodeId());
    m_edgeLabelEdit->setText(m_currentEdge.label());
    m_edgeColorLabel->setStyleSheet(QString("background-color: %1").arg(m_currentEdge.color().name()));

    // 恢复信号发送
    m_edgeLabelEdit->blockSignals(false);
}
