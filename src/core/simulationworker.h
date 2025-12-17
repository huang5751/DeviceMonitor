#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>
#include <QThread>
#include <QAtomicInt>
#include "devicedatamodel.h"

class SimulationWorker : public QThread
{
    Q_OBJECT

public:
    explicit SimulationWorker(DeviceDataModel *dataModel, QObject *parent = nullptr);
    ~SimulationWorker();

    // 简单控制接口
    void requestStop();
    void setUpdateInterval(int milliseconds);

signals:
    // 数据更新信号（会自动跨线程）
    void dataGenerated(int deviceId, double newValue);
    void statusMessage(const QString &message);

protected:
    void run() override;

private:
    DeviceDataModel *m_dataModel;
    QAtomicInt m_stopRequested;  // 原子操作，线程安全
    int m_updateInterval;

    double generateNewValue(int deviceId, double current, double min, double max);
};

#endif // SIMULATIONWORKER_H
