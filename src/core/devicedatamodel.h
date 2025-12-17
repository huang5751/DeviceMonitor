#ifndef DEVICEDATAMODEL_H
#define DEVICEDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDateTime>
#include <QColor>

// 设备数据结构
struct DeviceData {
    int deviceId;
    QString deviceName;
    QString deviceType;      // 设备类型：温度、压力等
    double currentValue;     // 当前值
    double minValue;         // 最小值
    double maxValue;         // 最大值
    QString unit;            // 单位：°C、MPa等
    QDateTime timestamp;     // 时间戳
    bool isOnline;           // 在线状态
    int warningLevel;        // 报警级别：0正常，1警告，2严重

    DeviceData()
        : deviceId(0), currentValue(0.0), minValue(0.0), maxValue(100.0),
        isOnline(true), warningLevel(0) {}
};

class DeviceDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        ColId = 0,
        ColName,
        ColType,
        ColValue,
        ColMin,
        ColMax,
        ColUnit,
        ColTime,
        ColStatus,
        ColWarning,
        ColCount  // 列数
    };

    explicit DeviceDataModel(QObject *parent = nullptr);
    ~DeviceDataModel();

    // 必需重写的模型接口
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // 自定义方法
    void addDevice(const DeviceData &device);
    void updateDeviceValue(int deviceId, double newValue);
    void removeDevice(int deviceId);
    DeviceData getDevice(int row) const;
    void clearAll();

    // 数据获取
    int deviceCount() const { return m_devices.size(); }
    QVector<DeviceData> allDevices() const { return m_devices; }
    DeviceData getDeviceById(int deviceId) const;

signals:
    void deviceDataChanged(int deviceId);
    void warningTriggered(int deviceId, int level);

private:
    QVector<DeviceData> m_devices;

    // 检查数值是否触发报警
    int checkWarningLevel(const DeviceData &device, double newValue) const;
};

#endif // DEVICEDATAMODEL_H
