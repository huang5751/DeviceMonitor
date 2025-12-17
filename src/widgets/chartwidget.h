#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

    class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    // 图表控制
    void setChartTitle(const QString &title);
    void setYAxisTitle(const QString &title);
    void setXAxisTitle(const QString &title);

    // 数据操作
    void addDataPoint(int deviceId, double value, const QString &timestamp = QString());
    void clearDeviceData(int deviceId);
    void clearAllData();

    // 设备管理
    void addDeviceSeries(int deviceId, const QString &deviceName, const QColor &color = Qt::blue);
    void removeDeviceSeries(int deviceId);

    // 显示控制
    void setMaxDataPoints(int count);
    void setYRange(double min, double max);
    void enableLegend(bool enabled);

    // 工具
    void saveChartAsImage(const QString &fileName);
    void zoomReset();

public slots:
    void onDeviceSelected(int deviceId);
    void updateChart();

signals:
    void chartCleared();
    void dataPointAdded(int deviceId, double value);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    // 图表组件
    QChart *m_chart;
    QChartView *m_chartView;

    // 数据存储
    struct DeviceSeries {
        QLineSeries *series;
        QString name;
        QColor color;
        QVector<QPointF> dataPoints;
        QVector<QString> timestamps;
    };

    QMap<int, DeviceSeries> m_deviceSeries;
    int m_maxDataPoints;

    // 坐标轴
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QDateTimeAxis *m_timeAxis;

    // 当前显示的设备
    int m_currentDeviceId;

    // 私有方法
    void setupChart();
    void setupAxes();
    void setupLegend();
    void createContextMenu();
    void updateAxesRange();

    // 工具方法
    QString formatValue(double value, const QString &unit) const;
    QColor generateColor(int index) const;
};

#endif // CHARTWIDGET_H
