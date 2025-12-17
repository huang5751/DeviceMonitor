// src/core/tcpserver.cpp
#include "tcpserver.h"
#include <QDateTime>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QDebug>
#include <QElapsedTimer>

TcpServer::TcpServer(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_simulationTimer(new QTimer(this))
{
    // 初始化设备名称（与你的项目对应）
    m_deviceNames << "温度传感器_01" << "压力传感器_01" << "流量计_01";

    // 连接新连接信号
    connect(m_tcpServer, &QTcpServer::newConnection,
            this, &TcpServer::onNewConnection);

    // 设置模拟定时器
    connect(m_simulationTimer, &QTimer::timeout,
            this, &TcpServer::sendDataToClients);

    qDebug() << "TCP服务器模块创建完成";
}

TcpServer::~TcpServer()
{
    stopServer();
    stopSimulation();
}

bool TcpServer::startServer(quint16 port)
{
    if (m_tcpServer->isListening()) {
        qWarning() << "服务器已在运行";
        return true;
    }

    m_port = port;

    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        qCritical() << "无法启动服务器:" << m_tcpServer->errorString();
        emit statusChanged(QString("启动失败: %1").arg(m_tcpServer->errorString()));
        return false;
    }

    // 初始化设备数据
    m_deviceData.clear();
    for (int i = 0; i < m_deviceCount && i < m_deviceNames.size(); ++i) {
        m_deviceData[m_deviceNames[i]] = generateRandomValue();
    }

    qDebug() << "TCP服务器已启动，端口:" << port;
    qDebug() << "模拟设备:" << m_deviceNames.mid(0, m_deviceCount);

    emit serverStarted(port);
    emit statusChanged(QString("服务器运行中 (端口:%1)").arg(port));

    return true;
}

void TcpServer::stopServer()
{
    if (m_tcpServer->isListening()) {
        m_tcpServer->close();

        // 断开所有客户端
        for (QTcpSocket *client : m_clients) {
            client->disconnectFromHost();
            if (client->state() != QAbstractSocket::UnconnectedState) {
                client->waitForDisconnected(1000);
            }
            client->deleteLater();
        }
        m_clients.clear();

        qDebug() << "TCP服务器已停止";
        emit serverStopped();
        emit statusChanged("服务器已停止");
    }
}

void TcpServer::setDeviceCount(int count)
{
    m_deviceCount = qMax(1, qMin(count, m_deviceNames.size()));
    m_deviceData.clear();

    for (int i = 0; i < m_deviceCount; ++i) {
        m_deviceData[m_deviceNames[i]] = generateRandomValue();
    }
}

void TcpServer::setDataRange(double min, double max)
{
    if (min < max) {
        m_minValue = min;
        m_maxValue = max;
    }
}

void TcpServer::startSimulation(int interval)
{
    if (interval > 0) {
        m_simulationInterval = interval;
        m_simulationTimer->setInterval(interval);
    }

    if (!m_simulationRunning) {
        m_simulationTimer->start(m_simulationInterval);
        m_simulationRunning = true;

        qDebug() << "TCP数据模拟开始，间隔:" << m_simulationInterval << "ms";
        emit simulationStarted();
        emit statusChanged("数据模拟已启动");
    }
}

void TcpServer::stopSimulation()
{
    if (m_simulationRunning) {
        m_simulationTimer->stop();
        m_simulationRunning = false;

        qDebug() << "TCP数据模拟停止";
        emit simulationStopped();
        emit statusChanged("数据模拟已停止");
    }
}

void TcpServer::onNewConnection()
{
    while (m_tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();

        // 连接信号
        connect(clientSocket, &QTcpSocket::disconnected,
                this, &TcpServer::onClientDisconnected);
        connect(clientSocket, &QTcpSocket::readyRead,
                this, &TcpServer::onReadyRead);

        QString clientInfo = QString("%1:%2")
                                 .arg(clientSocket->peerAddress().toString())
                                 .arg(clientSocket->peerPort());

        m_clients.append(clientSocket);

        qDebug() << "客户端连接:" << clientInfo;
        emit clientConnected(clientInfo);
        emit statusChanged(QString("客户端连接: %1").arg(clientInfo));

        // 发送欢迎消息
        QString welcomeMsg = QString("CONNECTED DeviceMonitor Server v1.0\r\n"
                                     "Port: %1 | Devices: %2 | Interval: %3ms\r\n"
                                     "Commands: STATUS, START, STOP, CLEAR, SET_INTERVAL <ms>\r\n"
                                     "OK\r\n")
                                 .arg(m_port)
                                 .arg(m_deviceCount)
                                 .arg(m_simulationInterval);
        clientSocket->write(welcomeMsg.toUtf8());
    }
}

void TcpServer::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QString clientInfo = QString("%1:%2")
                             .arg(client->peerAddress().toString())
                             .arg(client->peerPort());

    m_clients.removeAll(client);
    client->deleteLater();

    qDebug() << "客户端断开:" << clientInfo;
    emit clientDisconnected(clientInfo);
    emit statusChanged(QString("客户端断开: %1").arg(clientInfo));
}

void TcpServer::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    while (client->canReadLine()) {
        QByteArray data = client->readLine();
        QString command = QString::fromUtf8(data).trimmed();

        qDebug() << "收到客户端命令:" << command;
        handleClientCommand(client, command);
    }
}

void TcpServer::handleClientCommand(QTcpSocket *client, const QString &command)
{
    QString response;

    if (command.isEmpty()) {
        return;
    }

    QString cmdUpper = command.toUpper();

    if (cmdUpper == "STATUS") {
        response = QString("STATUS Port:%1 Devices:%2/%3 Running:%4 Clients:%5\r\n")
        .arg(m_port)
            .arg(m_deviceCount)
            .arg(m_deviceNames.size())
            .arg(m_simulationRunning ? "YES" : "NO")
            .arg(m_clients.size());
    }
    else if (cmdUpper == "START") {
        if (!m_simulationRunning) {
            startSimulation();
            response = "SIMULATION STARTED\r\n";
        } else {
            response = "SIMULATION ALREADY RUNNING\r\n";
        }
    }
    else if (cmdUpper == "STOP") {
        if (m_simulationRunning) {
            stopSimulation();
            response = "SIMULATION STOPPED\r\n";
        } else {
            response = "SIMULATION NOT RUNNING\r\n";
        }
    }
    else if (cmdUpper == "CLEAR") {
        emit remoteClearChart();
        response = "CHART CLEAR COMMAND SENT\r\n";
    }
    else if (cmdUpper == "START_UI") {
        emit remoteStartRequested();
        response = "UI START COMMAND SENT\r\n";
    }
    else if (cmdUpper == "STOP_UI") {
        emit remoteStopRequested();
        response = "UI STOP COMMAND SENT\r\n";
    }
    else if (cmdUpper.startsWith("SET_INTERVAL")) {
        QStringList parts = command.split(" ", Qt::SkipEmptyParts);
        if (parts.size() > 1) {
            bool ok;
            int interval = parts[1].toInt(&ok);
            if (ok && interval >= 100 && interval <= 10000) {
                m_simulationInterval = interval;
                if (m_simulationRunning) {
                    m_simulationTimer->setInterval(interval);
                }
                emit remoteSetUpdateInterval(interval);
                response = QString("INTERVAL SET TO %1ms\r\n").arg(interval);
            } else {
                response = "ERROR: Invalid interval (100-10000ms)\r\n";
            }
        } else {
            response = "ERROR: Missing interval value\r\n";
        }
    }
    else if (cmdUpper == "TEST_DATA") {
        sendTestData();
        response = "TEST DATA SENT\r\n";
    }
    else if (cmdUpper == "HELP") {
        response = "Available commands:\r\n"
                   "  STATUS          - Show server status\r\n"
                   "  START           - Start data simulation\r\n"
                   "  STOP            - Stop data simulation\r\n"
                   "  CLEAR           - Clear chart data\r\n"
                   "  START_UI        - Start UI monitoring\r\n"
                   "  STOP_UI         - Stop UI monitoring\r\n"
                   "  SET_INTERVAL ms - Set update interval\r\n"
                   "  TEST_DATA       - Send test data\r\n"
                   "  HELP            - Show this help\r\n";
    }
    else {
        response = "ERROR: Unknown command. Type HELP for commands list.\r\n";
    }

    if (client->state() == QAbstractSocket::ConnectedState) {
        client->write(response.toUtf8());
    }
}

void TcpServer::sendDataToClients()
{
    if (m_clients.isEmpty()) {
        return;  // 没有客户端连接
    }

    QString timestamp = getCurrentTimestamp();

    // 为每个设备生成新数据并发送
    for (auto it = m_deviceData.begin(); it != m_deviceData.end(); ++it) {
        QString deviceId = it.key();
        double newValue = generateRandomValue();
        m_deviceData[deviceId] = newValue;

        // 构建数据包格式：DEVICE|VALUE|TIMESTAMP|UNIT
        QString unit = deviceId.contains("温度") ? "°C" :
                           deviceId.contains("压力") ? "MPa" : "L/min";

        QString dataPacket = QString("DATA|%1|%2|%3|%4\r\n")
                                 .arg(deviceId)
                                 .arg(newValue, 0, 'f', 4)
                                 .arg(timestamp)
                                 .arg(unit);

        // 发送给所有连接的客户端
        for (QTcpSocket *client : m_clients) {
            if (client->state() == QAbstractSocket::ConnectedState) {
                client->write(dataPacket.toUtf8());
            }
        }

        // 发出信号（用于UI更新）
        emit dataSent(deviceId, newValue, timestamp);
    }
}

void TcpServer::sendTestData()
{
    QString timestamp = getCurrentTimestamp();

    // 发送测试数据
    QString testPacket = QString("DATA|测试设备|25.5000|%1|°C\r\n").arg(timestamp);

    for (QTcpSocket *client : m_clients) {
        if (client->state() == QAbstractSocket::ConnectedState) {
            client->write(testPacket.toUtf8());
        }
    }

    emit dataSent("测试设备", 25.5, timestamp);
}

double TcpServer::generateRandomValue() const
{
    return m_minValue + QRandomGenerator::global()->generateDouble() * (m_maxValue - m_minValue);
}

QString TcpServer::getCurrentTimestamp() const
{
    return QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
}
