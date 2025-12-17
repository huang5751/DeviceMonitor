// src/widgets/qmlcontainer.h
#ifndef QMLCONTAINER_H
#define QMLCONTAINER_H

#include <QWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWidget>

class QmlContainer : public QWidget
{
    Q_OBJECT

public:
    explicit QmlContainer(QWidget *parent = nullptr);

    // 更新设备信息
    void updateDeviceInfo(const QString &name, double value,
                          const QString &unit, int warningLevel, bool isOnline);

    // 更新仪表盘数据
    void updateDashboard(const QString &name, double value,
                         double min, double max, const QString &unit, int warningLevel);

private:
    QQuickWidget *m_qmlWidget;
    QQmlContext *m_context;
};

#endif // QMLCONTAINER_H
