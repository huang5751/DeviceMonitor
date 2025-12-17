#include "chartwidget.h"
#include <QDebug>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxDataPoints(100)
    , m_currentDeviceId(-1)
{
    // 创建图表
    m_chart = new QChart();
    m_chart->setTheme(QChart::ChartThemeLight);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    // 创建图表视图
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setRubberBand(QChartView::RectangleRubberBand);

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_chartView);
    setLayout(layout);

    // 初始化坐标轴
    setupAxes();
    setupChart();

    // 设置默认标题
    setChartTitle("设备数据趋势图");
    setYAxisTitle("数值");
    setXAxisTitle("数据点索引");

    qDebug() << "图表控件创建完成";
}

ChartWidget::~ChartWidget()
{
    clearAllData();
}

void ChartWidget::setupChart()
{
    // 设置图表属性
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_chart->setDropShadowEnabled(false);
}

void ChartWidget::setupAxes()
{
    // X轴（索引轴）
    m_axisX = new QValueAxis();
    m_axisX->setTitleText("数据点");
    m_axisX->setLabelFormat("%d");
    m_axisX->setTickCount(10);
    m_axisX->setRange(0, m_maxDataPoints);

    // Y轴（数值轴）
    m_axisY = new QValueAxis();
    m_axisY->setTitleText("数值");
    m_axisY->setLabelFormat("%.2f");
    m_axisY->setTickCount(10);
    m_axisY->setRange(0, 100);

    // 时间轴（备用）
    m_timeAxis = new QDateTimeAxis();
    m_timeAxis->setTitleText("时间");
    m_timeAxis->setFormat("hh:mm:ss");

    // 添加到图表
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
}

void ChartWidget::setChartTitle(const QString &title)
{
    m_chart->setTitle(title);
    m_chart->setTitleFont(QFont("Microsoft YaHei", 12, QFont::Bold));
}

void ChartWidget::setYAxisTitle(const QString &title)
{
    m_axisY->setTitleText(title);
    m_axisY->setTitleFont(QFont("Microsoft YaHei", 10));
}

void ChartWidget::setXAxisTitle(const QString &title)
{
    m_axisX->setTitleText(title);
    m_axisX->setTitleFont(QFont("Microsoft YaHei", 10));
}

void ChartWidget::addDeviceSeries(int deviceId, const QString &deviceName, const QColor &color)
{
    if (m_deviceSeries.contains(deviceId)) {
        qWarning() << "设备" << deviceId << "的曲线已存在";
        return;
    }

    // 创建曲线
    QLineSeries *series = new QLineSeries();
    series->setName(deviceName);

    // QColor seriesColor = color.isValid() ? color : generateColor(m_deviceSeries.size());
    // series->setColor(seriesColor);
    QColor seriesColor;

    //温度传感器用红色，压力传感器用蓝色
    if (deviceId == 1) {  // 温度传感器
        seriesColor = QColor("#e74c3c");  // 红色
    } else if (deviceId == 2) {  // 压力传感器
        seriesColor = QColor("#3498db");  // 蓝色
    } else {
        seriesColor = color.isValid() ? color : generateColor(m_deviceSeries.size());
    }

    series->setColor(seriesColor);
    series->setPointsVisible(true);
    series->setPointLabelsVisible(false);

    // 设置线宽和样式
    QPen pen(seriesColor);
    pen.setWidth(2);
    series->setPen(pen);

    // 保存到映射
    DeviceSeries deviceSeries;
    deviceSeries.series = series;
    deviceSeries.name = deviceName;
    deviceSeries.color = seriesColor;

    m_deviceSeries[deviceId] = deviceSeries;

    // 添加到图表
    m_chart->addSeries(series);
    series->attachAxis(m_axisX);
    series->attachAxis(m_axisY);

    qDebug() << "添加设备曲线:" << deviceId << deviceName;
}

void ChartWidget::removeDeviceSeries(int deviceId)
{
    if (!m_deviceSeries.contains(deviceId)) return;

    DeviceSeries &series = m_deviceSeries[deviceId];
    m_chart->removeSeries(series.series);
    series.series->deleteLater();

    m_deviceSeries.remove(deviceId);

    if (m_currentDeviceId == deviceId) {
        m_currentDeviceId = -1;
    }
}

void ChartWidget::addDataPoint(int deviceId, double value, const QString &timestamp)
{
    if (!m_deviceSeries.contains(deviceId)) {
        // 自动创建曲线（如果没有）
        addDeviceSeries(deviceId, QString("设备 %1").arg(deviceId));
    }

    DeviceSeries &deviceSeries = m_deviceSeries[deviceId];
    QLineSeries *series = deviceSeries.series;

    // 添加数据点
    int pointIndex = deviceSeries.dataPoints.size();
    deviceSeries.dataPoints.append(QPointF(pointIndex, value));
    deviceSeries.timestamps.append(timestamp.isEmpty() ?
                                       QDateTime::currentDateTime().toString("hh:mm:ss") : timestamp);

    // 更新曲线数据
    series->append(pointIndex, value);

    // 限制数据点数量
    if (deviceSeries.dataPoints.size() > m_maxDataPoints) {
        // 移除最旧的数据点
        deviceSeries.dataPoints.removeFirst();
        deviceSeries.timestamps.removeFirst();

        // 重新构建整个曲线（性能考虑）
        series->clear();
        for (int i = 0; i < deviceSeries.dataPoints.size(); ++i) {
            series->append(i, deviceSeries.dataPoints[i].y());
        }
    }

    // 更新坐标轴范围
    updateAxesRange();

    // 发出信号
    emit dataPointAdded(deviceId, value);
}

void ChartWidget::clearDeviceData(int deviceId)
{
    if (!m_deviceSeries.contains(deviceId)) return;

    DeviceSeries &series = m_deviceSeries[deviceId];
    series.series->clear();
    series.dataPoints.clear();
    series.timestamps.clear();

    updateAxesRange();
}

void ChartWidget::clearAllData()
{
    for (auto it = m_deviceSeries.begin(); it != m_deviceSeries.end(); ++it) {
        it->series->clear();
        it->dataPoints.clear();
        it->timestamps.clear();
    }

    updateAxesRange();
    emit chartCleared();
}

void ChartWidget::setMaxDataPoints(int count)
{
    if (count < 10) count = 10;
    if (count > 1000) count = 1000;

    m_maxDataPoints = count;
    m_axisX->setRange(0, count);

    // 裁剪现有数据
    for (auto it = m_deviceSeries.begin(); it != m_deviceSeries.end(); ++it) {
        if (it->dataPoints.size() > count) {
            int removeCount = it->dataPoints.size() - count;
            it->dataPoints = it->dataPoints.mid(removeCount);
            it->timestamps = it->timestamps.mid(removeCount);

            // 重新构建曲线
            it->series->clear();
            for (int i = 0; i < it->dataPoints.size(); ++i) {
                it->series->append(i, it->dataPoints[i].y());
            }
        }
    }
}

void ChartWidget::setYRange(double min, double max)
{
    if (min >= max) return;

    m_axisY->setRange(min, max);
    m_axisY->setMin(min);
    m_axisY->setMax(max);
}

void ChartWidget::enableLegend(bool enabled)
{
    m_chart->legend()->setVisible(enabled);
}

void ChartWidget::onDeviceSelected(int deviceId)
{
    m_currentDeviceId = deviceId;

    // 可以在这里实现选中设备的高亮显示
    // 例如：加粗选中的曲线，淡化其他曲线
}

void ChartWidget::updateChart()
{
    // 强制图表重绘
    m_chartView->update();
}

void ChartWidget::updateAxesRange()
{
    if (m_deviceSeries.isEmpty()) {
        m_axisY->setRange(0, 100);
        return;
    }

    // 计算所有数据的Y轴范围
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto &deviceSeries : m_deviceSeries) {
        for (const QPointF &point : deviceSeries.dataPoints) {
            if (point.y() < minY) minY = point.y();
            if (point.y() > maxY) maxY = point.y();
        }
    }

    // 添加一些边距
    double range = maxY - minY;
    if (range < 0.1) range = 10;  // 防止范围太小

    minY -= range * 0.1;
    maxY += range * 0.1;

    // 更新Y轴范围
    m_axisY->setRange(minY, maxY);
}

QColor ChartWidget::generateColor(int index) const
{
    static const QList<QColor> defaultColors = {
        QColor(0x34, 0x98, 0xdb),  // 蓝色
        QColor(0xe7, 0x4c, 0x3c),  // 红色
        QColor(0x2e, 0xcc, 0x71),  // 绿色
        QColor(0xf3, 0x9c, 0x12),  // 橙色
        QColor(0x9b, 0x59, 0xb6),  // 紫色
        QColor(0x1a, 0xbc, 0x9c),  // 青色
        QColor(0x34, 0x49, 0x5e),  // 深蓝灰
        QColor(0xe6, 0x7e, 0x22),  // 胡萝卜色
    };

    return defaultColors.at(index % defaultColors.size());
}

void ChartWidget::saveChartAsImage(const QString &fileName)
{
    if (fileName.isEmpty()) return;

    QPixmap pixmap = m_chartView->grab();
    if (pixmap.save(fileName)) {
        qDebug() << "图表已保存到:" << fileName;
    } else {
        qWarning() << "保存图表失败:" << fileName;
    }
}

void ChartWidget::zoomReset()
{
    m_chart->zoomReset();
    updateAxesRange();
}

void ChartWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction("保存图表为图片", this, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        "保存图表", QString(), "PNG图片 (*.png);;JPEG图片 (*.jpg)");
        if (!fileName.isEmpty()) {
            saveChartAsImage(fileName);
        }
    });

    menu.addAction("重置缩放", this, &ChartWidget::zoomReset);
    menu.addSeparator();
    menu.addAction("清除所有数据", this, &ChartWidget::clearAllData);

    menu.exec(event->globalPos());
}

QString ChartWidget::formatValue(double value, const QString &unit) const
{
    return QString("%1 %2").arg(value, 0, 'f', 2).arg(unit);
}
