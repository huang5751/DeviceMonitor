// src/widgets/qmlcontainer.cpp 修改后的完整代码
#include "qmlcontainer.h"
#include <QVBoxLayout>
#include <QDebug>

QmlContainer::QmlContainer(QWidget *parent)
    : QWidget(parent)
    , m_qmlWidget(new QQuickWidget(this))
{
    // 设置QML引擎
    m_qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_qmlWidget->setSource(QUrl("qrc:/qml/DeviceInfoCard.qml"));

    // 获取QML上下文
    m_context = m_qmlWidget->rootContext();

    // 设置初始属性
    m_context->setContextProperty("deviceName", "未选择设备");
    m_context->setContextProperty("currentValue", 0.0);
    m_context->setContextProperty("unit", "单位");
    m_context->setContextProperty("warningLevel", 0);
    m_context->setContextProperty("isOnline", false);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_qmlWidget);

    setLayout(layout);
}

void QmlContainer::updateDeviceInfo(const QString &name, double value,
                                    const QString &unit, int warningLevel, bool isOnline)
{
    if (!m_context) return;

    // 设置新的属性值
    m_context->setContextProperty("deviceName", name);
    m_context->setContextProperty("currentValue", value);
    m_context->setContextProperty("unit", unit);
    m_context->setContextProperty("warningLevel", warningLevel);
    m_context->setContextProperty("isOnline", isOnline);

    // 方法1：使用正确的invokeMethod调用（如果QML有update方法）
    // QMetaObject::invokeMethod(m_qmlWidget->rootObject(), "update", Qt::AutoConnection);

    // 方法2：更简单的方式，直接重新设置source（强制刷新）
    // m_qmlWidget->setSource(m_qmlWidget->source());

    // 方法3：最佳方案 - 在QML中使用属性绑定，不需要手动刷新
    // 什么都不做，QML会自动更新

    qDebug() << "QML更新设备信息:" << name << value << unit;
}

void QmlContainer::updateDashboard(const QString &name, double value,
                                   double min, double max, const QString &unit, int warningLevel)
{
    // 切换QML源到仪表盘
    if (m_qmlWidget->source() != QUrl("qrc:/qml/DeviceDashboard.qml")) {
        m_qmlWidget->setSource(QUrl("qrc:/qml/DeviceDashboard.qml"));
        m_context = m_qmlWidget->rootContext();
    }

    if (!m_context) return;

    m_context->setContextProperty("deviceName", name);
    m_context->setContextProperty("currentValue", value);
    m_context->setContextProperty("minValue", min);
    m_context->setContextProperty("maxValue", max);
    m_context->setContextProperty("unit", unit);
    m_context->setContextProperty("warningLevel", warningLevel);

    qDebug() << "QML更新仪表盘:" << name << value << unit;
}
