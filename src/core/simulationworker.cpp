#include "simulationworker.h"
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QDebug>

SimulationWorker::SimulationWorker(DeviceDataModel *dataModel, QObject *parent)
    : QThread(parent)
    , m_dataModel(dataModel)
    , m_stopRequested(0)
    , m_updateInterval(1000)
{
    qDebug() << "工作线程创建，更新间隔:" << m_updateInterval << "ms";
}

SimulationWorker::~SimulationWorker()
{
    requestStop();
    if (isRunning()) {
        wait(1000);  // 等待1秒
        if (isRunning()) {
            qWarning() << "线程未正常停止，强制终止";
            terminate();
            wait();
        }
    }
    qDebug() << "工作线程销毁";
}

void SimulationWorker::requestStop()
{
    m_stopRequested.storeRelaxed(1);
}

void SimulationWorker::setUpdateInterval(int milliseconds)
{
    m_updateInterval = qBound(200, milliseconds, 5000);  // 限制200ms-5秒
}

void SimulationWorker::run()
{
    emit statusMessage("工作线程启动");
    qDebug() << "工作线程进入运行循环";

    QElapsedTimer timer;
    int updateCount = 0;

    while (!m_stopRequested.loadRelaxed()) {
        timer.restart();

        if (!m_dataModel) {
            msleep(m_updateInterval);
            continue;
        }

        // 更新所有设备
        int deviceCount = m_dataModel->deviceCount();
        for (int i = 0; i < deviceCount; ++i) {
            DeviceData device = m_dataModel->getDevice(i);

            double newValue = generateNewValue(device.deviceId,
                                               device.currentValue,
                                               device.minValue,
                                               device.maxValue);

            // 发射信号到主线程
            emit dataGenerated(device.deviceId, newValue);
        }

        updateCount++;

        // 每10次更新发送一次状态
        if (updateCount % 10 == 0) {
            emit statusMessage(QString("已更新 %1 次数据").arg(updateCount));
        }

        // 精确控制间隔
        int elapsed = timer.elapsed();
        int sleepTime = m_updateInterval - elapsed;

        if (sleepTime > 0) {
            msleep(sleepTime);
        } else {
            // 如果处理时间超过间隔，下次减少等待
            qDebug() << "数据处理耗时" << elapsed << "ms，超过间隔" << m_updateInterval << "ms";
        }
    }

    emit statusMessage("工作线程停止");
    qDebug() << "工作线程退出，总更新次数:" << updateCount;
}

double SimulationWorker::generateNewValue(int deviceId, double current, double min, double max)
{
    Q_UNUSED(deviceId);

    if (max <= min) return current;

    double range = max - min;

    // 简单的随机波动 + 趋势
    static QMap<int, double> trends;
    if (!trends.contains(deviceId)) {
        trends[deviceId] = 0.0;
    }

    // 随机改变趋势
    if (QRandomGenerator::global()->bounded(100) < 20) {
        trends[deviceId] = (QRandomGenerator::global()->generateDouble() - 0.5) * 0.1;
    }

    // 趋势变化 + 随机噪声
    double change = trends[deviceId] * range;
    double noise = (QRandomGenerator::global()->generateDouble() - 0.5) * range * 0.05;

    double newValue = current + change + noise;

    // 限制在范围内
    if (newValue < min) {
        newValue = min;
        trends[deviceId] = 0.01;  // 碰到边界后反转趋势
    } else if (newValue > max) {
        newValue = max;
        trends[deviceId] = -0.01;
    }

    return newValue;
}
