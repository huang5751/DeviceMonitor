#include "mainwindow.h"
#include "core/devicedatamodel.h"

#include "mainwindow.h"
#include "core/devicedatamodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QTreeWidget>
#include <QProgressBar>
#include <QFormLayout>
#include <QFrame>
#include <QDebug>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_deviceTableView(nullptr)
    , m_deviceTree(nullptr)
    , m_qmlDeviceInfo(nullptr)
    //, m_deviceInfoWidget(nullptr)
    , m_chartWidget(nullptr)
    , m_statusLabel(nullptr)
    , m_statsLabel(nullptr)
    , m_startBtn(nullptr)
    , m_stopBtn(nullptr)
    , m_dataModel(nullptr)
    , m_simulator(nullptr)
    , m_statusTimer(nullptr)
    , m_monitoring(false)
    , m_tcpServer(new TcpServer(this))
{
    // 创建数据模型
    m_dataModel = new DeviceDataModel(this);

    // 创建数据模拟器
    m_simulator = new DeviceSimulator(this);
    m_simulator->setDataModel(m_dataModel);
    m_simulator->setUpdateInterval(500);  // 每500ms更新一次

    // 连接模拟器信号
    connect(m_simulator, &DeviceSimulator::started,
            this, &MainWindow::onSimulationStarted);
    connect(m_simulator, &DeviceSimulator::stopped,
            this, &MainWindow::onSimulationStopped);
    connect(m_simulator, &DeviceSimulator::dataUpdated,
            this, &MainWindow::onDeviceDataUpdated);
    connect(m_simulator, &DeviceSimulator::statusChanged,
            this, &MainWindow::onStatisticsUpdated);


    // 设置TCP服务器
    m_tcpServer->setDeviceCount(2);  // 温度传感器和压力传感器
    m_tcpServer->setDataRange(20.0, 80.0);  // 温度范围
    // 设置界面
    setupUI();

    //TCP控制UI
    setupTcpUI();

    // 连接信号槽
    setupConnections();

    // 新增：TCP连接
    setupTcpConnections();

    // 初始状态
    updateStatus();

    //加载设置
    loadSettings();
}
MainWindow::~MainWindow()
{
    // Qt会自动删除有父对象的内存，这里不需要手动delete
}

void MainWindow::setupUI()
{
    // 设置窗口属性
    setWindowTitle("智能设备监控仪表盘");
    resize(1400, 800);  // 增大窗口尺寸

    // ===== 创建中心部件 =====
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局（水平分割：左-右）
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(10);

    // ===== 左侧：设备树 =====
    QWidget *leftPanel = new QWidget(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    m_deviceTree = new QTreeWidget(leftPanel);
    setupDeviceTree();

    QLabel *leftTitle = new QLabel("📱 设备列表", leftPanel);
    leftTitle->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");

    leftLayout->addWidget(leftTitle);
    leftLayout->addWidget(m_deviceTree);

    leftPanel->setLayout(leftLayout);
    leftPanel->setMinimumWidth(280);
    leftPanel->setMaximumWidth(350);

    // ===== 右侧：上下分割 =====
    QWidget *rightPanel = new QWidget(centralWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    // ===== 右上：设备表格 =====
    QWidget *topRightPanel = new QWidget(rightPanel);
    QVBoxLayout *topRightLayout = new QVBoxLayout(topRightPanel);
    topRightLayout->setContentsMargins(0, 0, 0, 0);

    m_deviceTableView = new QTableView(topRightPanel);
    setupDeviceTable();

    QLabel *tableTitle = new QLabel("📊 设备数据监控", topRightPanel);
    tableTitle->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");

    topRightLayout->addWidget(tableTitle);
    topRightLayout->addWidget(m_deviceTableView);
    topRightPanel->setLayout(topRightLayout);

    // ===== 右下：设备信息和图表 =====
    QWidget *bottomRightPanel = new QWidget(rightPanel);
    QHBoxLayout *bottomRightLayout = new QHBoxLayout(bottomRightPanel);
    bottomRightLayout->setContentsMargins(0, 0, 0, 0);
    bottomRightLayout->setSpacing(10);

    // 右下左：设备信息
    QWidget *infoPanel = new QWidget(bottomRightPanel);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoPanel);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    // === 创建设备信息堆叠布局（支持切换）===
    QLabel *infoTitle = new QLabel("ℹ️ 设备详细信息", infoPanel);
    infoTitle->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");

    // 创建堆叠部件，可以切换显示
    m_deviceInfoStack = new QStackedWidget(infoPanel);

    // 1. 创建原版设备信息面板
    setupDeviceInfoPanel();  // 初始化原版设备信息
    m_deviceInfoStack->addWidget(m_deviceInfoWidget);

    // 2. 创建QML设备信息面板
    m_qmlDeviceInfo = new QmlContainer(infoPanel);
    m_qmlDeviceInfo->setVisible(false);  // 初始隐藏
    m_deviceInfoStack->addWidget(m_qmlDeviceInfo);

    // 3. 初始显示原版
    m_deviceInfoStack->setCurrentIndex(0);

    infoLayout->addWidget(infoTitle);
    infoLayout->addWidget(m_deviceInfoStack);


    infoPanel->setLayout(infoLayout);
    infoPanel->setMinimumWidth(300);
    infoPanel->setMaximumWidth(350);

    // 右下右：图表
    QWidget *chartPanel = new QWidget(bottomRightPanel);
    QVBoxLayout *chartLayout = new QVBoxLayout(chartPanel);
    chartLayout->setContentsMargins(0, 0, 0, 0);

    m_chartWidget = new ChartWidget(chartPanel);
    m_chartWidget->setChartTitle("设备数据趋势图");
    m_chartWidget->setYAxisTitle("数值");
    m_chartWidget->setMaxDataPoints(100);
    m_chartWidget->setMinimumHeight(300);

    QLabel *chartTitle = new QLabel("📈 数据趋势图", chartPanel);
    chartTitle->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");

    chartLayout->addWidget(chartTitle);
    chartLayout->addWidget(m_chartWidget);
    chartPanel->setLayout(chartLayout);

    // 添加右下两个面板
    bottomRightLayout->addWidget(infoPanel, 1);  // 设备信息占1份
    bottomRightLayout->addWidget(chartPanel, 2); // 图表占2份（更宽）

    bottomRightPanel->setLayout(bottomRightLayout);

    // ===== 组装右侧面板 =====
    rightLayout->addWidget(topRightPanel, 2);     // 右上表格占2份高度
    rightLayout->addWidget(bottomRightPanel, 1);  // 右下信息+图表占1份高度

    rightPanel->setLayout(rightLayout);

    // ===== 添加到主布局 =====
    mainLayout->addWidget(leftPanel, 1);   // 左侧占1份宽度
    mainLayout->addWidget(rightPanel, 3);  // 右侧占3份宽度

    centralWidget->setLayout(mainLayout);

    // ===== 底部工具栏和状态栏 =====
    setupToolBar();

    // 状态栏
    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("就绪 | 设备数: 0");

    m_statsLabel = new QLabel(this);
    m_statsLabel->setText("更新: 0 | 运行: 0s");
    m_statsLabel->setObjectName("statsLabel");

    // 添加到状态栏
    statusBar()->addWidget(m_statusLabel);
    statusBar()->addPermanentWidget(m_statsLabel);
}

void MainWindow::setupTcpUI()
{
    // 在现有工具栏后面添加TCP控制
    QToolBar *tcpToolBar = new QToolBar("TCP通信", this);
    addToolBar(Qt::TopToolBarArea, tcpToolBar);
    tcpToolBar->setObjectName("tcpToolBar");
    tcpToolBar->setMovable(false);

    // TCP服务器控制
    m_tcpStartBtn = new QPushButton("🌐 启动TCP服务器", this);
    m_tcpStopBtn = new QPushButton("🛑 停止服务器", this);
    m_tcpStopBtn->setEnabled(false);

    // 测试按钮
    m_tcpTestBtn = new QPushButton("📡 发送测试数据", this);
    m_tcpTestBtn->setEnabled(false);

    // 状态标签
    m_tcpStatusLabel = new QLabel("服务器未运行", this);
    m_tcpStatusLabel->setStyleSheet("color: #7f8c8d; padding: 5px 10px;");
    m_tcpStatusLabel->setMinimumWidth(200);

    // 端口标签
    QLabel *portLabel = new QLabel("端口: 12345", this);
    portLabel->setStyleSheet("color: #3498db; padding: 5px 10px; font-weight: bold;");

    // 按钮样式
    QString tcpBtnStyle = "QPushButton { "
                          "padding: 8px 15px; "
                          "border-radius: 4px; "
                          "font-weight: bold; "
                          "margin: 2px; "
                          "}"
                          "QPushButton:enabled { "
                          "background-color: #3498db; "
                          "color: white; "
                          "border: 1px solid #2980b9; "
                          "}"
                          "QPushButton:disabled { "
                          "background-color: #bdc3c7; "
                          "color: #7f8c8d; "
                          "}"
                          "QPushButton:hover:enabled { "
                          "background-color: #2980b9; "
                          "}";

    m_tcpStartBtn->setStyleSheet(tcpBtnStyle);
    m_tcpStopBtn->setStyleSheet(tcpBtnStyle.replace("#3498db", "#e74c3c"));
    m_tcpTestBtn->setStyleSheet(tcpBtnStyle.replace("#3498db", "#2ecc71"));

    // 添加到工具栏
    tcpToolBar->addWidget(m_tcpStartBtn);
    tcpToolBar->addWidget(m_tcpStopBtn);
    tcpToolBar->addSeparator();
    tcpToolBar->addWidget(m_tcpTestBtn);
    tcpToolBar->addSeparator();
    tcpToolBar->addWidget(portLabel);
    tcpToolBar->addSeparator();
    tcpToolBar->addWidget(m_tcpStatusLabel);
    tcpToolBar->addWidget(new QLabel("  ", this));  // 占位

    // // 添加帮助提示
    // QLabel *helpLabel = new QLabel("💡 使用 telnet 127.0.0.1 12345 连接测试", this);
    // helpLabel->setStyleSheet("color: #95a5a6; font-size: 12px; padding: 5px 10px;");
    // tcpToolBar->addWidget(helpLabel);
}

void MainWindow::setupDeviceTable()
{
    if (!m_deviceTableView || !m_dataModel)
        return;

    // 设置数据模型
    m_deviceTableView->setModel(m_dataModel);

    // 表格属性
    m_deviceTableView->setAlternatingRowColors(true);
    m_deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_deviceTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_deviceTableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                       QAbstractItemView::SelectedClicked);

    // 列宽调整
    m_deviceTableView->horizontalHeader()->setStretchLastSection(true);
    m_deviceTableView->verticalHeader()->setDefaultSectionSize(25);

    // 设置列宽
    m_deviceTableView->setColumnWidth(0, 50);   // ID
    m_deviceTableView->setColumnWidth(1, 150);  // 名称
    m_deviceTableView->setColumnWidth(2, 80);   // 类型
    m_deviceTableView->setColumnWidth(3, 100);  // 当前值
    m_deviceTableView->setColumnWidth(4, 80);   // 最小值
    m_deviceTableView->setColumnWidth(5, 80);   // 最大值
    m_deviceTableView->setColumnWidth(6, 60);   // 单位
    m_deviceTableView->setColumnWidth(7, 140);  // 更新时间
    m_deviceTableView->setColumnWidth(8, 80);   // 状态
    // 报警列使用剩余空间
}

void MainWindow::setupDeviceTree()
{
    if (!m_deviceTree) return;

    // 设置树控件属性
    m_deviceTree->setHeaderLabel("设备");
    m_deviceTree->setColumnCount(1);
    m_deviceTree->setAlternatingRowColors(true);

    // 创建设备分类
    QTreeWidgetItem *tempGroup = new QTreeWidgetItem(m_deviceTree);
    tempGroup->setText(0, "温度传感器");
    tempGroup->setData(0, Qt::UserRole, -1);  // -1表示分组

    QTreeWidgetItem *pressureGroup = new QTreeWidgetItem(m_deviceTree);
    pressureGroup->setText(0, "压力传感器");
    pressureGroup->setData(0, Qt::UserRole, -1);

    QTreeWidgetItem *flowGroup = new QTreeWidgetItem(m_deviceTree);
    flowGroup->setText(0, "流量计");
    flowGroup->setData(0, Qt::UserRole, -1);

    // 添加测试设备（从数据模型中读取）
    if (m_dataModel) {
        for (int i = 0; i < m_dataModel->deviceCount(); ++i) {
            DeviceData device = m_dataModel->getDevice(i);
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, device.deviceName);
            item->setData(0, Qt::UserRole, device.deviceId);  // 存储设备ID

            // 根据设备类型添加到不同分组
            if (device.deviceType.contains("温度")) {
                tempGroup->addChild(item);
            } else if (device.deviceType.contains("压力")) {
                pressureGroup->addChild(item);
            } else {
                flowGroup->addChild(item);
            }

            // 设置图标（简单用文本颜色表示状态）
            if (!device.isOnline) {
                item->setForeground(0, QBrush(Qt::gray));
            } else if (device.warningLevel == 2) {
                item->setForeground(0, QBrush(Qt::red));
            } else if (device.warningLevel == 1) {
                item->setForeground(0, QBrush(QColor(255, 165, 0)));  // 橙色
            }
        }
    }

    // 展开所有分组
    tempGroup->setExpanded(true);
    pressureGroup->setExpanded(true);
    flowGroup->setExpanded(true);

    // 连接选择信号
    connect(m_deviceTree, &QTreeWidget::itemClicked,
            this, [this](QTreeWidgetItem *item, int column) {
                Q_UNUSED(column);
                int deviceId = item->data(0, Qt::UserRole).toInt();
                if (deviceId > 0) {  // 不是分组项（-1）
                    updateDeviceInfo(deviceId);

                    // 在表格中选中对应行
                    for (int i = 0; i < m_dataModel->deviceCount(); ++i) {
                        if (m_dataModel->getDevice(i).deviceId == deviceId) {
                            QModelIndex idx = m_dataModel->index(i, 0);
                            m_deviceTableView->selectRow(i);
                            break;
                        }
                    }
                }
            });
}


void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("控制工具栏");

    m_startBtn = new QPushButton("▶ 开始监控", this);
    m_stopBtn = new QPushButton("⏸ 停止监控", this);
    m_stopBtn->setEnabled(false);

    QPushButton *refreshBtn = new QPushButton("↻ 刷新设备", this);

    // 按钮样式
    QString buttonStyle =
        "QPushButton { padding: 8px 15px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:enabled { background-color: #3498db; color: white; }"
        "QPushButton:disabled { background-color: #bdc3c7; color: #7f8c8d; }"
        "QPushButton:hover:enabled { background-color: #2980b9; }";

    m_startBtn->setStyleSheet(buttonStyle);
    m_stopBtn->setStyleSheet(buttonStyle);
    m_startBtn->setObjectName("startButton");
    m_stopBtn->setObjectName("stopButton");
    refreshBtn->setStyleSheet(buttonStyle.replace("#3498db", "#2ecc71"));

    toolBar->addWidget(m_startBtn);
    toolBar->addWidget(m_stopBtn);
    toolBar->addSeparator();
    toolBar->addWidget(refreshBtn);

    // 连接刷新按钮
    connect(refreshBtn, &QPushButton::clicked, this, [this]() {
        // 简单重新加载设备树
        if (m_deviceTree) {
            m_deviceTree->clear();
            setupDeviceTree();
        }
        statusBar()->showMessage("设备列表已刷新", 1500);
    });

    // 添加QML视图切换按钮
    toolBar->addSeparator();
    QPushButton *qmlViewBtn = new QPushButton("🎨 QML视图", this);
    qmlViewBtn->setStyleSheet("QPushButton { padding: 8px 15px; border-radius: 4px; background-color: #9b59b6; color: white; }");
    toolBar->addWidget(qmlViewBtn);

    // 在lambda中捕获qmlViewBtn
    connect(qmlViewBtn, &QPushButton::clicked, this, [this, qmlViewBtn]() {
        static bool isDashboard = false;

        if (m_qmlDeviceInfo && m_dataModel) {
            // 获取当前选中设备
            QTreeWidgetItem *selected = m_deviceTree->currentItem();
            if (selected) {
                int deviceId = selected->data(0, Qt::UserRole).toInt();
                if (deviceId > 0) {
                    DeviceData device = m_dataModel->getDeviceById(deviceId);

                    if (isDashboard) {
                        // 切换到卡片视图
                        m_qmlDeviceInfo->updateDeviceInfo(
                            device.deviceName,
                            device.currentValue,
                            device.unit,
                            device.warningLevel,
                            device.isOnline
                            );
                        qmlViewBtn->setText("📊 仪表盘视图");
                        statusBar()->showMessage("切换到卡片视图", 1500);
                    } else {
                        // 切换到仪表盘视图
                        m_qmlDeviceInfo->updateDashboard(
                            device.deviceName,
                            device.currentValue,
                            device.minValue,
                            device.maxValue,
                            device.unit,
                            device.warningLevel
                            );
                        qmlViewBtn->setText("🃏 卡片视图");
                        statusBar()->showMessage("切换到仪表盘视图", 1500);
                    }

                    isDashboard = !isDashboard;
                } else {
                    statusBar()->showMessage("请先选择设备", 1500);
                }
            } else {
                statusBar()->showMessage("请先选择设备", 1500);
            }
        }
    });

    // 添加按钮
    toolBar->addSeparator();
    QPushButton *testQmlBtn = new QPushButton("🧪 测试QML", this);
    testQmlBtn->setStyleSheet(
        "QPushButton {"
        "  padding: 8px 15px;"
        "  border-radius: 4px;"
        "  font-weight: bold;"
        "  background-color: #9b59b6;"
        "  color: white;"
        "}"
        "QPushButton:hover {"
        "  background-color: #8e44ad;"
        "}"
        );
    toolBar->addWidget(testQmlBtn);

    // 连接测试按钮
    connect(testQmlBtn, &QPushButton::clicked, this, &MainWindow::onTestQmlClicked);
}

void MainWindow::setupDeviceInfoPanel()
{
    m_deviceInfoWidget = new QWidget();
    QVBoxLayout *infoLayout = new QVBoxLayout(m_deviceInfoWidget);
    infoLayout->setSpacing(10);
    infoLayout->setContentsMargins(10, 10, 10, 10);

    // 设备名称
    QWidget *nameWidget = new QWidget(m_deviceInfoWidget);
    QHBoxLayout *nameLayout = new QHBoxLayout(nameWidget);
    nameLayout->setContentsMargins(0, 0, 0, 0);
    nameLayout->addWidget(new QLabel("设备名称:"));
    m_deviceNameLabel = new QLabel("未选择设备");
    m_deviceNameLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
    nameLayout->addWidget(m_deviceNameLabel);
    nameLayout->addStretch();
    nameWidget->setLayout(nameLayout);

    // 当前值
    QWidget *valueWidget = new QWidget(m_deviceInfoWidget);
    QHBoxLayout *valueLayout = new QHBoxLayout(valueWidget);
    valueLayout->setContentsMargins(0, 0, 0, 0);
    valueLayout->addWidget(new QLabel("当前值:"));
    m_deviceValueLabel = new QLabel("--");
    m_deviceValueLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #27ae60;");
    valueLayout->addWidget(m_deviceValueLabel);
    valueLayout->addStretch();
    valueWidget->setLayout(valueLayout);

    // 进度条（显示数值在正常范围内的比例）
    m_deviceProgress = new QProgressBar(m_deviceInfoWidget);
    m_deviceProgress->setRange(0, 100);
    m_deviceProgress->setValue(50);
    m_deviceProgress->setTextVisible(true);
    m_deviceProgress->setFormat("%p%");
    m_deviceProgress->setStyleSheet(
        "QProgressBar { border: 1px solid #bdc3c7; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: #3498db; border-radius: 5px; }"
        );

    // 状态信息
    QWidget *statusWidget = new QWidget(m_deviceInfoWidget);
    QFormLayout *statusLayout = new QFormLayout(statusWidget);
    statusLayout->setContentsMargins(0, 0, 0, 0);
    statusLayout->setSpacing(5);

    m_deviceStatusLabel = new QLabel("--");
    m_deviceStatusLabel->setStyleSheet("color: #7f8c8d;");

    statusLayout->addRow("状态:", m_deviceStatusLabel);
    statusLayout->addRow("更新时间:", new QLabel("--"));
    statusLayout->addRow("报警级别:", new QLabel("正常"));

    // 添加到主布局
    infoLayout->addWidget(nameWidget);
    infoLayout->addWidget(valueWidget);
    infoLayout->addWidget(m_deviceProgress);
    infoLayout->addWidget(statusWidget);
    infoLayout->addStretch();

    // 分隔线
    QFrame *line = new QFrame(m_deviceInfoWidget);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #ecf0f1;");
    infoLayout->addWidget(line);

    // 添加一个提示标签
    QLabel *hintLabel = new QLabel("点击左侧设备列表中的设备查看详细信息");
    hintLabel->setWordWrap(true);
    hintLabel->setStyleSheet("color: #95a5a6; font-style: italic; font-size: 12px;");
    infoLayout->addWidget(hintLabel);

    m_deviceInfoWidget->setLayout(infoLayout);

    m_deviceInfoWidget->setObjectName("deviceInfoWidget");
    m_deviceNameLabel->setObjectName("deviceNameLabel");
    m_deviceValueLabel->setObjectName("deviceValueLabel");
}

void MainWindow::setupConnections()
{
    // 按钮连接
    connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    // 设备选择时更新图表
    connect(m_deviceTree, &QTreeWidget::itemClicked,
            this, [this](QTreeWidgetItem *item, int column) {
                Q_UNUSED(column);
                int deviceId = item->data(0, Qt::UserRole).toInt();
                if (deviceId > 0) {
                    // 更新设备信息
                    updateDeviceInfo(deviceId);

                    // 告诉图表选中了哪个设备
                    if (m_chartWidget) {
                        m_chartWidget->onDeviceSelected(deviceId);
                    }

                    // 在表格中选中对应行
                    for (int i = 0; i < m_dataModel->deviceCount(); ++i) {
                        if (m_dataModel->getDevice(i).deviceId == deviceId) {
                            m_deviceTableView->selectRow(i);
                            break;
                        }
                    }
                }
            });

    // 模拟器信号连接
    if (m_simulator) {
        connect(m_simulator, &DeviceSimulator::dataUpdated,  // 注意信号名
                this, &MainWindow::onDeviceDataUpdated);

        connect(m_simulator, &DeviceSimulator::statusChanged,
                this, [this](const QString &status) {
                    statusBar()->showMessage(status, 2000);
                });
    }

    // 定时更新状态
    m_statusTimer = new QTimer(this);
    connect(m_statusTimer, &QTimer::timeout, this, &MainWindow::updateStatus);
    m_statusTimer->start(1000);
}

void MainWindow::setupTcpConnections()
{
    // 按钮连接
    connect(m_tcpStartBtn, &QPushButton::clicked, this, &MainWindow::onTcpStartClicked);
    connect(m_tcpStopBtn, &QPushButton::clicked, this, &MainWindow::onTcpStopClicked);
    connect(m_tcpTestBtn, &QPushButton::clicked, this, &MainWindow::onTcpTestClicked);

    // TCP服务器信号连接
    connect(m_tcpServer, &TcpServer::serverStarted, this, [this](quint16 port) {
        m_tcpStatusLabel->setText(QString("运行中 (客户端:0)"));
        m_tcpStatusLabel->setStyleSheet("color: #27ae60; padding: 5px 10px; font-weight: bold;");
        m_tcpStartBtn->setEnabled(false);
        m_tcpStopBtn->setEnabled(true);
        m_tcpTestBtn->setEnabled(true);

        statusBar()->showMessage(QString("✅ TCP服务器已启动，端口: %1").arg(port), 3000);
        qDebug() << "✅ TCP服务器启动成功，端口:" << port;
    });

    connect(m_tcpServer, &TcpServer::serverStopped, this, [this]() {
        m_tcpStatusLabel->setText("服务器未运行");
        m_tcpStatusLabel->setStyleSheet("color: #7f8c8d; padding: 5px 10px;");
        m_tcpStartBtn->setEnabled(true);
        m_tcpStopBtn->setEnabled(false);
        m_tcpTestBtn->setEnabled(false);

        statusBar()->showMessage("⏹️ TCP服务器已停止", 3000);
        qDebug() << "⏹️ TCP服务器已停止";
    });

    connect(m_tcpServer, &TcpServer::clientConnected, this, [this](QString clientInfo) {
        m_tcpStatusLabel->setText(QString("运行中 (客户端:%1)").arg(m_tcpServer->getClientCount()));
        statusBar()->showMessage(QString("🔗 客户端连接: %1").arg(clientInfo), 2000);
        qDebug() << "🔗 客户端连接:" << clientInfo;
    });

    connect(m_tcpServer, &TcpServer::clientDisconnected, this, [this](QString clientInfo) {
        m_tcpStatusLabel->setText(QString("运行中 (客户端:%1)").arg(m_tcpServer->getClientCount()));
        statusBar()->showMessage(QString("🔌 客户端断开: %1").arg(clientInfo), 2000);
        qDebug() << "🔌 客户端断开:" << clientInfo;
    });

    // 数据接收连接 - 最重要的部分！
    connect(m_tcpServer, &TcpServer::dataSent, this, &MainWindow::onTcpDataReceived);

    connect(m_tcpServer, &TcpServer::statusChanged, this, &MainWindow::onTcpStatusChanged);

    // 远程控制信号连接
    connect(m_tcpServer, &TcpServer::remoteStartRequested, this, &MainWindow::onRemoteStartRequested);
    connect(m_tcpServer, &TcpServer::remoteStopRequested, this, &MainWindow::onRemoteStopRequested);
    connect(m_tcpServer, &TcpServer::remoteClearChart, this, &MainWindow::onRemoteClearChart);
    connect(m_tcpServer, &TcpServer::remoteSetUpdateInterval, this, &MainWindow::onRemoteSetInterval);

    // 模拟器状态
    connect(m_tcpServer, &TcpServer::simulationStarted, this, [this]() {
        statusBar()->showMessage("📡 TCP数据模拟已启动", 2000);
        qDebug() << "📡 TCP数据模拟已启动";
    });

    connect(m_tcpServer, &TcpServer::simulationStopped, this, [this]() {
        statusBar()->showMessage("⏸️ TCP数据模拟已停止", 2000);
        qDebug() << "⏸️ TCP数据模拟已停止";
    });
}

void MainWindow::onStartClicked()
{
    if (m_monitoring || !m_simulator)
        return;

    m_monitoring = true;
    m_startBtn->setEnabled(false);
    m_startBtn->setText("监控中...");
    m_stopBtn->setEnabled(true);

    // 启动模拟器
    m_simulator->start();

    // 更新设备树显示
    if (m_deviceTree) {
        for (int i = 0; i < m_deviceTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *group = m_deviceTree->topLevelItem(i);
            for (int j = 0; j < group->childCount(); ++j) {
                QTreeWidgetItem *deviceItem = group->child(j);
                deviceItem->setForeground(0, QBrush(QColor("#27ae60")));  // 绿色表示运行中
            }
        }
    }

    // 新增：初始化图表中的设备曲线
    if (m_chartWidget && m_dataModel) {
        // 为每个设备创建曲线
        for (int i = 0; i < m_dataModel->deviceCount(); ++i) {
            DeviceData device = m_dataModel->getDevice(i);
            QString displayName = QString("%1 (%2)")
                                      .arg(device.deviceName)
                                      .arg(device.unit);

            m_chartWidget->addDeviceSeries(device.deviceId, displayName);
        }
    }

    statusBar()->showMessage("监控已启动，数据开始更新", 2000);
}

void MainWindow::onStopClicked()
{
    if (!m_monitoring || !m_simulator)
        return;

    m_monitoring = false;
    m_startBtn->setEnabled(true);
    m_startBtn->setText("▶ 开始监控");
    m_stopBtn->setEnabled(false);

    // // 停止时清空图表数据（但不删除曲线）
    // if (m_chartWidget) {
    //     m_chartWidget->clearAllData();
    // }

    // 停止模拟器
    m_simulator->stop();

    // 恢复设备树颜色
    if (m_deviceTree) {
        // 这里可以根据设备实际状态设置颜色
        for (int i = 0; i < m_deviceTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *group = m_deviceTree->topLevelItem(i);
            for (int j = 0; j < group->childCount(); ++j) {
                QTreeWidgetItem *deviceItem = group->child(j);
                deviceItem->setForeground(0, QBrush(Qt::black));
            }
        }
    }

    statusBar()->showMessage("监控已停止", 2000);
}

void MainWindow::updateStatus()
{
    if (m_dataModel) {
        int deviceCount = m_dataModel->deviceCount();
        int onlineCount = 0;
        int warningCount = 0;

        // 统计在线和报警设备
        for (int i = 0; i < deviceCount; ++i) {
            DeviceData device = m_dataModel->getDevice(i);
            if (device.isOnline) onlineCount++;
            if (device.warningLevel > 0) warningCount++;
        }

        QString status = QString("设备: %1 | 在线: %2 | 报警: %3 | 状态: %4")
                             .arg(deviceCount)
                             .arg(onlineCount)
                             .arg(warningCount)
                             .arg(m_monitoring ? "运行中" : "已停止");
        m_statusLabel->setText(status);
    }
}

void MainWindow::updateDeviceInfo(int deviceId)
{
    if (!m_dataModel) return;

    DeviceData device = m_dataModel->getDeviceById(deviceId);
    if (device.deviceId == 0) return;

    // === 1. 更新原版设备信息面板 ===
    // 更新设备名称
    m_deviceNameLabel->setText(device.deviceName);

    // 更新当前值
    QString valueText = QString("%1 %2")
                            .arg(device.currentValue, 0, 'f', 2)
                            .arg(device.unit);
    m_deviceValueLabel->setText(valueText);

    // 更新进度条（显示数值在正常范围内的位置）
    double range = device.maxValue - device.minValue;
    if (range > 0) {
        int progress = int(((device.currentValue - device.minValue) / range) * 100);
        progress = qBound(0, progress, 100);  // 限制在0-100之间
        m_deviceProgress->setValue(progress);

        // 根据数值设置进度条颜色
        QString style;
        if (device.warningLevel == 2) {
            style = "QProgressBar { border: 1px solid #bdc3c7; border-radius: 5px; text-align: center; }"
                    "QProgressBar::chunk { background-color: #e74c3c; border-radius: 5px; }";  // 红色
        } else if (device.warningLevel == 1) {
            style = "QProgressBar { border: 1px solid #bdc3c7; border-radius: 5px; text-align: center; }"
                    "QProgressBar::chunk { background-color: #f39c12; border-radius: 5px; }";  // 橙色
        } else {
            style = "QProgressBar { border: 1px solid #bdc3c7; border-radius: 5px; text-align: center; }"
                    "QProgressBar::chunk { background-color: #3498db; border-radius: 5px; }";  // 蓝色
        }
        m_deviceProgress->setStyleSheet(style);
    }

    // 更新状态
    QString statusText = device.isOnline ?
                             QString("<span style='color: #27ae60;'>● 在线</span>") :
                             QString("<span style='color: #7f8c8d;'>● 离线</span>");

    if (device.warningLevel == 2) {
        statusText += " <span style='color: #e74c3c;'>(严重报警)</span>";
    } else if (device.warningLevel == 1) {
        statusText += " <span style='color: #f39c12;'>(警告)</span>";
    }

    m_deviceStatusLabel->setText(statusText);

    // === 2. 更新QML设备信息面板 ===
    if (m_qmlDeviceInfo) {
        m_qmlDeviceInfo->updateDeviceInfo(
            device.deviceName,
            device.currentValue,
            device.unit,
            device.warningLevel,
            device.isOnline
            );
    }

    // 在状态栏显示提示
    statusBar()->showMessage(QString("已选择设备: %1").arg(device.deviceName), 2000);
}

void MainWindow::onStatisticsUpdated(const QString &status)
{
    // 更新统计标签
    if (m_statsLabel) {
        m_statsLabel->setText(status);
    }

    // 状态包含重要信息，也在状态栏临时显示
    if (status.contains("错误") || status.contains("停止")) {
        statusBar()->showMessage(status, 3000);
    }
}

void MainWindow::onDeviceDataUpdated(int deviceId, double newValue)
{
    // 记录调试信息
    qDebug() << "收到设备数据更新 - 设备ID:" << deviceId << "新值:" << newValue;

    // 如果当前选中的设备更新了，刷新右侧信息面板
    QTreeWidgetItem *selectedItem = m_deviceTree->currentItem();
    if (selectedItem) {
        int selectedId = selectedItem->data(0, Qt::UserRole).toInt();
        if (selectedId == deviceId) {
            updateDeviceInfo(deviceId);

            // 更新进度条显示
            DeviceData device = m_dataModel->getDeviceById(deviceId);
            if (device.deviceId > 0) {
                double range = device.maxValue - device.minValue;
                if (range > 0) {
                    int progress = int(((newValue - device.minValue) / range) * 100);
                    progress = qBound(0, progress, 100);

                    // 根据报警级别设置颜色
                    QString style;
                    if (device.warningLevel == 2) {
                        style = "QProgressBar::chunk { background-color: #e74c3c; }";
                    } else if (device.warningLevel == 1) {
                        style = "QProgressBar::chunk { background-color: #f39c12; }";
                    } else {
                        style = "QProgressBar::chunk { background-color: #3498db; }";
                    }

                    if (m_deviceProgress) {
                        m_deviceProgress->setValue(progress);
                        m_deviceProgress->setStyleSheet(style);
                    }
                }
            }
        }
    }

    // 新增：更新图表
    if (m_chartWidget) {
        DeviceData device = m_dataModel->getDeviceById(deviceId);
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

        // 添加数据点到图表
        m_chartWidget->addDataPoint(deviceId, newValue, timestamp);

        // 如果这是当前选中的设备，可以特别处理
        QTreeWidgetItem *selectedItem = m_deviceTree->currentItem();
        if (selectedItem) {
            int selectedId = selectedItem->data(0, Qt::UserRole).toInt();
            if (selectedId == deviceId) {
                // 可以在这里调整图表显示，比如只显示选中设备的数据
            }
        }
    }
}

void MainWindow::onSimulationStarted()
{
    m_monitoring = true;

    // 更新按钮状态
    if (m_startBtn) {
        m_startBtn->setEnabled(false);
        m_startBtn->setText("监控中...");
    }

    if (m_stopBtn) {
        m_stopBtn->setEnabled(true);
    }

    // 简单更新状态栏
    statusBar()->showMessage("数据模拟已启动", 2000);

    // 更新状态显示
    updateStatus();

    qDebug() << "模拟器启动";
}
void MainWindow::onSimulationStopped()
{
    m_monitoring = false;

    // 更新按钮状态
    if (m_startBtn) {
        m_startBtn->setEnabled(true);
        m_startBtn->setText("▶ 开始监控");
    }

    if (m_stopBtn) {
        m_stopBtn->setEnabled(false);
    }

    // 简单更新状态栏
    statusBar()->showMessage("数据模拟已停止", 2000);

    // 更新状态显示
    updateStatus();

    qDebug() << "模拟器停止";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();  // 保存设置
    QMainWindow::closeEvent(event);
}

void MainWindow::loadSettings()
{
    QSettings settings("YourCompany", "DeviceMonitor");

    // 1. 恢复窗口几何状态
    QByteArray geometry = settings.value("window/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }

    // 2. 恢复窗口状态
    QByteArray state = settings.value("window/state").toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }

    // 3. 恢复窗口是否为最大化
    bool wasMaximized = settings.value("window/maximized", false).toBool();
    if (wasMaximized) {
        showMaximized();
    }

    // 4. 注释掉或移除网格设置的代码，暂时不实现
    // bool showGrid = settings.value("chart/showGrid", true).toBool();
    // if (m_chartWidget) {
    //     m_chartWidget->setGridVisible(showGrid);
    // }

    // 5. 恢复最后选择的设备
    // int lastDeviceId = settings.value("device/lastSelected", -1).toInt();
    // if (lastDeviceId != -1) {
    //     selectDevice(lastDeviceId);
    // }

    qDebug() << "设置加载完成";
}

void MainWindow::saveSettings()
{
    QSettings settings("YourCompany", "DeviceMonitor");

    // 1. 保存窗口几何状态
    settings.setValue("window/geometry", saveGeometry());

    // 2. 保存窗口状态
    settings.setValue("window/state", saveState());

    // 3. 保存窗口最大化状态
    settings.setValue("window/maximized", isMaximized());

    // 4. 注释掉或移除网格设置的代码
    // if (m_chartWidget) {
    //     settings.setValue("chart/showGrid", m_chartWidget->isGridVisible());
    //     settings.setValue("chart/maxPoints", m_chartWidget->maxDataPoints());
    // }

    // 5. 保存最后选择的设备
    // if (m_currentDeviceId != -1) {
    //     settings.setValue("device/lastSelected", m_currentDeviceId);
    // }

    // 立即写入磁盘
    settings.sync();

    qDebug() << "设置保存完成";
}

// 在mainwindow.cpp末尾添加这些方法
void MainWindow::onTcpStartClicked()
{
    if (m_tcpServer->isRunning()) {
        return;
    }

    // 启动TCP服务器（默认端口12345）
    if (m_tcpServer->startServer(12345)) {
        // 同时启动数据模拟
        m_tcpServer->startSimulation(1500);
    }
}

void MainWindow::onTcpStopClicked()
{
    if (m_tcpServer->isRunning()) {
        m_tcpServer->stopSimulation();
        m_tcpServer->stopServer();
    }
}

void MainWindow::onTcpTestClicked()
{
    if (!m_tcpServer->isRunning()) {
        QMessageBox::warning(this, "警告", "请先启动TCP服务器");
        return;
    }

    // 发送测试数据
    m_tcpServer->sendTestData();
    statusBar()->showMessage("📤 测试数据已发送", 1500);
}

void MainWindow::onTcpDataReceived(QString deviceId, double value, QString timestamp)
{
    // 这是最重要的方法！将TCP数据显示在界面上
    qDebug() << "📨 TCP数据 ->" << deviceId << "=" << value << "at" << timestamp;

    // 1. 在状态栏显示最新数据
    static QElapsedTimer displayTimer;
    if (!displayTimer.isValid() || displayTimer.hasExpired(500)) {
        statusBar()->showMessage(QString("📡 收到: %1 = %2").arg(deviceId).arg(value, 0, 'f', 2), 1000);
        displayTimer.start();
    }

    // 2. 添加到图表（让网络数据驱动图表！）
    if (m_chartWidget) {
        // 根据设备类型映射到不同的图表ID（使用100+的ID避免与本地设备冲突）
        int chartId = 100;

        if (deviceId.contains("温度")) {
            chartId = 101;
        } else if (deviceId.contains("压力")) {
            chartId = 102;
        } else if (deviceId.contains("流量")) {
            chartId = 103;
        } else if (deviceId.contains("测试")) {
            chartId = 104;
        }

        // 如果曲线不存在，先创建
        // 需要先在chartwidget.h中添加 hasSeries 方法，或者直接尝试添加
        QString displayName = QString("TCP-%1").arg(deviceId);
        QColor color;

        if (chartId == 101) color = QColor(255, 100, 100);    // 温度：亮红色
        else if (chartId == 102) color = QColor(100, 100, 255);  // 压力：蓝色
        else if (chartId == 103) color = QColor(100, 200, 100);  // 流量：绿色
        else color = QColor(200, 100, 200);  // 测试：紫色

        // 添加数据点到图表
        m_chartWidget->addDataPoint(chartId, value, timestamp);

        // 简单方法：每次都添加系列（如果已存在，addDeviceSeries会跳过）
        m_chartWidget->addDeviceSeries(chartId, displayName, color);
    }
}

void MainWindow::onTcpStatusChanged(QString status)
{
    // 更新TCP状态标签
    m_tcpStatusLabel->setText(status);
}

void MainWindow::onRemoteStartRequested()
{
    // 远程控制：开始监控
    if (!m_monitoring && m_startBtn && m_startBtn->isEnabled()) {
        m_startBtn->click();
        statusBar()->showMessage("🔄 远程控制：开始监控", 2000);
        qDebug() << "🔄 远程控制：开始监控";
    }
}

void MainWindow::onRemoteStopRequested()
{
    // 远程控制：停止监控
    if (m_monitoring && m_stopBtn && m_stopBtn->isEnabled()) {
        m_stopBtn->click();
        statusBar()->showMessage("⏸️ 远程控制：停止监控", 2000);
        qDebug() << "⏸️ 远程控制：停止监控";
    }
}

void MainWindow::onRemoteClearChart()
{
    // 远程控制：清空图表
    if (m_chartWidget) {
        m_chartWidget->clearAllData();
        statusBar()->showMessage("🗑️ 远程控制：图表已清空", 2000);
        qDebug() << "🗑️ 远程控制：图表已清空";
    }
}

void MainWindow::onRemoteSetInterval(int ms)
{
    // 远程控制：设置更新间隔
    if (m_simulator) {
        m_simulator->setUpdateInterval(ms);
        statusBar()->showMessage(QString("⏱️ 远程控制：更新间隔设为 %1ms").arg(ms), 2000);
        qDebug() << "⏱️ 远程控制：更新间隔设为" << ms << "ms";
    }
}

void MainWindow::onTestQmlClicked()
{
    qDebug() << "=== 开始 ===";

    // 创建测试对话框
    m_testDialog = new QDialog(this);
    m_testDialog->setWindowTitle("窗口");
    m_testDialog->resize(400, 300);

    QVBoxLayout *dialogLayout = new QVBoxLayout(m_testDialog);

    // 1. 创建QQuickWidget
    QQuickWidget *qmlWidget = new QQuickWidget(m_testDialog);
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // 2. 尝试加载QML
    // qDebug() << "尝试加载QML文件...";
    // QUrl qmlUrl("qrc:/qml/SimpleTest.qml");
    // qmlWidget->setSource(qmlUrl);

    //临时使用绝对路径
    QString absPath = "E:/QtProjects/DeviceMonitor/src/qml/SimpleTest.qml";
    QUrl qmlUrl = QUrl::fromLocalFile(absPath);
    qmlWidget->setSource(qmlUrl);

    // 3. 检查加载状态
    QQuickWidget::Status status = qmlWidget->status();
    qDebug() << "QML状态:" << status;

    if (status == QQuickWidget::Error) {
        qDebug() << "QML加载失败!";

        // 显示错误信息
        QLabel *errorLabel = new QLabel("QML加载失败", m_testDialog);
        errorLabel->setStyleSheet("color: red; font-weight: bold; padding: 10px;");
        dialogLayout->addWidget(errorLabel);

        // 显示具体错误
        QTextEdit *errorText = new QTextEdit(m_testDialog);
        errorText->setReadOnly(true);

        QString errorMsg = "错误详情:\n";
        QList<QQmlError> errors = qmlWidget->errors();
        for (const QQmlError &error : errors) {
            errorMsg += error.toString() + "\n";
            errorMsg += "位置: " + error.url().toString() + " 行:" + QString::number(error.line()) + "\n\n";
        }

        errorText->setText(errorMsg);
        dialogLayout->addWidget(errorText);

    } else if (status == QQuickWidget::Ready) {
        qDebug() << "QML加载成功!";

        // 显示成功信息
        QLabel *successLabel = new QLabel("✅ QML加载成功!", m_testDialog);
        successLabel->setStyleSheet("color: green; font-weight: bold; padding: 10px;");
        dialogLayout->addWidget(successLabel);

        // 添加QML部件
        dialogLayout->addWidget(qmlWidget);

        // 测试数据绑定
        QQmlContext *context = qmlWidget->rootContext();
        if (context) {
            context->setContextProperty("testName", "测试设备");
            context->setContextProperty("testValue", 42.5);
            qDebug() << "QML上下文设置完成";
        }
    } else {
        qDebug() << "QML状态未知:" << status;
    }

    // 4. 添加关闭按钮
    QPushButton *closeBtn = new QPushButton("关闭测试窗口", m_testDialog);
    connect(closeBtn, &QPushButton::clicked, m_testDialog, &QDialog::close);
    dialogLayout->addWidget(closeBtn);

    m_testDialog->setLayout(dialogLayout);
    m_testDialog->exec();  // 显示模态对话框

    qDebug() << "=== 结束 ===";
}
