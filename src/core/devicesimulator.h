#ifndef DEVICESIMULATOR_H
#define DEVICESIMULATOR_H

#include <QObject>
#include "simulationworker.h"

class DeviceSimulator : public QObject
{
    Q_OBJECT

public:
    explicit DeviceSimulator(QObject *parent = nullptr);
    ~DeviceSimulator();

    void setDataModel(DeviceDataModel *model);

    // 简单控制
    void start();
    void stop();

    bool isRunning() const;

    // 配置
    void setUpdateInterval(int milliseconds);
    int updateInterval() const { return m_updateInterval; }

signals:
    void started();
    void stopped();
    void dataUpdated(int deviceId, double newValue);
    void statusChanged(const QString &status);

private slots:
    void onWorkerDataGenerated(int deviceId, double newValue);
    void onWorkerStatusMessage(const QString &message);
    void onWorkerFinished();

private:
    DeviceDataModel *m_dataModel;
    SimulationWorker *m_worker;
    int m_updateInterval;
};

#endif // DEVICESIMULATOR_H
