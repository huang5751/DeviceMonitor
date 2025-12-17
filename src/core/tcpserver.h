// src/core/tcpserver.h
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QStringList>

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    // 服务器控制
    bool startServer(quint16 port = 12345);
    void stopServer();
    bool isRunning() const { return m_tcpServer && m_tcpServer->isListening(); }
    quint16 getPort() const { return m_port; }
    int getClientCount() const { return m_clients.size(); }

    // 设备模拟
    void startSimulation(int interval = 1000);
    void stopSimulation();
    bool isSimulating() const { return m_simulationRunning; }

    // 配置
    void setDeviceCount(int count);
    int getDeviceCount() const { return m_deviceCount; }
    void setDataRange(double min, double max);
    void getDataRange(double &min, double &max) const { min = m_minValue; max = m_maxValue; }

signals:
    void serverStarted(quint16 port);
    void serverStopped();
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);
    void dataSent(QString deviceId, double value, QString timestamp);
    void simulationStarted();
    void simulationStopped();
    void statusChanged(QString status);

    // 远程控制信号
    void remoteStartRequested();
    void remoteStopRequested();
    void remoteClearChart();
    void remoteSetUpdateInterval(int ms);

public slots:
    void sendTestData();

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();
    void sendDataToClients();

private:
    void generateDeviceData();
    void handleClientCommand(QTcpSocket *client, const QString &command);
    QString createDeviceInfo() const;
    double generateRandomValue() const;
    QString getCurrentTimestamp() const;

private:
    QTcpServer *m_tcpServer;
    QVector<QTcpSocket*> m_clients;
    QTimer *m_simulationTimer;

    // 模拟数据
    int m_deviceCount = 3;
    double m_minValue = 0.0;
    double m_maxValue = 100.0;
    quint16 m_port = 12345;

    // 当前设备数据
    QMap<QString, double> m_deviceData;
    QStringList m_deviceNames;

    bool m_simulationRunning = false;
    int m_simulationInterval = 1000;  // ms
};

#endif // TCPSERVER_H
