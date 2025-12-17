#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/devicesimulator.h"
#include "widgets/chartwidget.h"
#include "core/tcpserver.h"
#include <QMainWindow>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTreeWidget>
#include <QProgressBar>
#include <QFormLayout>
#include <QSettings>//数据持久化
#include "widgets/qmlcontainer.h"

// 前向声明（避免头文件相互包含）
class DeviceDataModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onStartClicked();     // 开始按钮点击
    void onStopClicked();      // 停止按钮点击
    void updateStatus();       // 更新状态栏

    void onTestQmlClicked();  // qlm测试槽函数

    // 模拟器相关槽函数
    void onSimulationStarted();
    void onSimulationStopped();
    void onDeviceDataUpdated(int deviceId, double newValue);
    void onStatisticsUpdated(const QString &status);

    //tcp相关函数
    void onTcpStartClicked();
    void onTcpStopClicked();
    void onTcpTestClicked();
    void onTcpDataReceived(QString deviceId, double value, QString timestamp);
    void onTcpStatusChanged(QString status);
    void onRemoteStartRequested();
    void onRemoteStopRequested();
    void onRemoteClearChart();
    void onRemoteSetInterval(int ms);

private:
    // 初始化方法
    void setupUI();            // 设置界面
    void setupDeviceTable();   // 初始化设备表格
    void setupToolBar();       // 初始化工具栏
    void setupConnections();   // 连接信号槽

    // 现有成员基础上添加：
    QTreeWidget *m_deviceTree;        // 设备树
    QWidget *m_deviceInfoWidget;      // 设备详细信息面板
    QmlContainer *m_qmlDeviceInfo;    // 设备信息部件qml
    QStackedWidget *m_deviceInfoStack; // 设备信息堆叠布局（用于切换）
    QLabel *m_deviceNameLabel;        // 设备名称标签
    QLabel *m_deviceValueLabel;       // 当前值标签
    QLabel *m_deviceStatusLabel;      // 状态标签
    QProgressBar *m_deviceProgress;   // 数值进度条
    DeviceSimulator *m_simulator;     // 数据模拟器
    ChartWidget *m_chartWidget;       // 图表控件

    QDialog *m_testDialog;             // QML测试对话框

    // 新增方法
    void setupDeviceTree();           // 初始化设备树
    void setupDeviceInfoPanel();      // 初始化设备信息面板
    void updateDeviceInfo(int deviceId); // 更新设备信息显示
    void setupChartArea();            //初始化图表区域
    //数据持久化
    void loadSettings();
    void saveSettings();

    // 成员变量
    QTableView *m_deviceTableView;  // 设备表格视图
    QLabel *m_statusLabel;          // 状态栏标签
    QLabel *m_statsLabel;           // 新增：统计信息标签
    QPushButton *m_startBtn;        // 开始按钮
    QPushButton *m_stopBtn;         // 停止按钮

    // 数据模型
    DeviceDataModel *m_dataModel;

    // 定时器
    QTimer *m_statusTimer;

    // 状态
    bool m_monitoring;

    // TCP服务器
    TcpServer *m_tcpServer;

    // TCP控制UI
    QPushButton *m_tcpStartBtn;
    QPushButton *m_tcpStopBtn;
    QPushButton *m_tcpTestBtn;
    QLabel *m_tcpStatusLabel;

    // Tcp
    void setupTcpUI();
    void setupTcpConnections();
};

#endif // MAINWINDOW_H
