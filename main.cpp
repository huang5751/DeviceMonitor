#include "src/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("Device Monitor");
    app.setOrganizationName("MyCompany");

    // 使用Fusion样式以获得更好的外观
    app.setStyle(QStyleFactory::create("Fusion"));

    // 从资源文件加载样式表
    QFile styleFile(":/resources/styles/default.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        app.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    MainWindow window;
    window.show();

    return app.exec();
}
