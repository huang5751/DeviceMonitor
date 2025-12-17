#include "devicesimulator.h"
#include <QDebug>

DeviceSimulator::DeviceSimulator(QObject *parent)
    : QObject(parent)
    , m_dataModel(nullptr)
    , m_worker(nullptr)
    , m_updateInterval(1000)
{
}

DeviceSimulator::~DeviceSimulator()
{
    stop();
}

void DeviceSimulator::setDataModel(DeviceDataModel *model)
{
    m_dataModel = model;
}

void DeviceSimulator::start()
{
    if (m_worker && m_worker->isRunning()) {
        qWarning() << "模拟器已经在运行";
        return;
    }

    // 创建新的工作线程
    m_worker = new SimulationWorker(m_dataModel, this);
    m_worker->setUpdateInterval(m_updateInterval);

    // 连接信号
    connect(m_worker, &SimulationWorker::dataGenerated,
            this, &DeviceSimulator::onWorkerDataGenerated);
    connect(m_worker, &SimulationWorker::statusMessage,
            this, &DeviceSimulator::onWorkerStatusMessage);
    connect(m_worker, &SimulationWorker::finished,
            this, &DeviceSimulator::onWorkerFinished);

    // 启动线程
    m_worker->start();

    emit started();
    emit statusChanged("模拟器启动");
    qDebug() << "DeviceSimulator: 启动工作线程";
}

void DeviceSimulator::stop()
{
    if (!m_worker) return;

    if (m_worker->isRunning()) {
        m_worker->requestStop();
        m_worker->wait(1000);  // 等待1秒

        if (m_worker->isRunning()) {
            qWarning() << "工作线程未正常停止";
            m_worker->terminate();
            m_worker->wait();
        }
    }

    m_worker->deleteLater();
    m_worker = nullptr;

    emit stopped();
    emit statusChanged("模拟器停止");
    qDebug() << "DeviceSimulator: 停止工作线程";
}

bool DeviceSimulator::isRunning() const
{
    return m_worker && m_worker->isRunning();
}

void DeviceSimulator::setUpdateInterval(int milliseconds)
{
    m_updateInterval = qBound(200, milliseconds, 5000);
    if (m_worker) {
        m_worker->setUpdateInterval(m_updateInterval);
    }
}

void DeviceSimulator::onWorkerDataGenerated(int deviceId, double newValue)
{
    // 在主线程中更新数据模型
    if (m_dataModel) {
        m_dataModel->updateDeviceValue(deviceId, newValue);
    }

    // 转发信号
    emit dataUpdated(deviceId, newValue);
}

void DeviceSimulator::onWorkerStatusMessage(const QString &message)
{
    emit statusChanged(message);
}

void DeviceSimulator::onWorkerFinished()
{
    qDebug() << "工作线程正常结束";
    m_worker = nullptr;
}
