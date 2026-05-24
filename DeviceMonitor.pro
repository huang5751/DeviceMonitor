#QT       += core gui widgets

QT       += core gui widgets charts
CONFIG   += c++11
TARGET = DeviceMonitor
TEMPLATE = app

SOURCES += \
    main.cpp \
    src/core/devicesimulator.cpp \
    src/core/simulationworker.cpp \
    src/core/tcpserver.cpp \
    src/mainwindow.cpp \
    src/core/devicedatamodel.cpp \
    src/widgets/chartwidget.cpp \
    src/widgets/dashboardwidget.cpp \
    src/widgets/qmlcontainer.cpp

HEADERS += \
    src/core/devicesimulator.h \
    src/core/simulationworker.h \
    src/core/tcpserver.h \
    src/mainwindow.h \
    src/core/devicedatamodel.h \
    src/widgets/chartwidget.h \
    src/widgets/dashboardwidget.h \
    src/widgets/qmlcontainer.h

INCLUDEPATH += $$PWD/src

RESOURCES += resources.qrc
QT += core gui widgets charts  # 加入 Qt charts
QT += core gui network  # 加入 network
QT += quick quickwidgets# 加入 qml 模块
#QT += core gui sql# 加入 sql 模块

# QML文件
QML_FILES += \
    src/qml/DeviceInfoCard.qml \
    src/qml/DeviceDashboard.qml

# 启用调试输出
CONFIG += console


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# 链接MySQL驱动
#LIBS += -lmysql
