#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

/**
 * @brief 主函数
 * @param argc 参数计数
 * @param argv 参数向量
 * @return 应用程序退出码
 */
int main(int argc, char* argv[])
{
    qDebug() << "程序启动...";

    // 创建应用程序
    QApplication app(argc, argv);

    qDebug() << "QApplication 创建成功";

    // 设置应用程序信息
    app.setApplicationName("ConceptMap");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ConceptMap");

    qDebug() << "应用程序信息设置完成";

    // 创建主窗口
    qDebug() << "创建主窗口...";
    MainWindow window;
    qDebug() << "主窗口创建成功";

    // 显示主窗口
    qDebug() << "显示主窗口...";
    window.show();
    qDebug() << "主窗口显示成功";

    // 运行应用程序
    qDebug() << "运行应用程序...";
    int result = app.exec();
    qDebug() << "应用程序退出，退出码:" << result;

    return result;
}
