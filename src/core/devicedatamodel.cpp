#include "devicedatamodel.h"
#include <QDateTime>
#include <QBrush>

DeviceDataModel::DeviceDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // 添加一些测试设备
    DeviceData tempDevice;
    tempDevice.deviceId = 1;
    tempDevice.deviceName = "温度传感器_01";
    tempDevice.deviceType = "温度";
    tempDevice.currentValue = 25.5;
    tempDevice.minValue = -10.0;
    tempDevice.maxValue = 80.0;
    tempDevice.unit = "°C";
    tempDevice.timestamp = QDateTime::currentDateTime();
    tempDevice.isOnline = true;
    tempDevice.warningLevel = 0;
    m_devices.append(tempDevice);

    DeviceData pressureDevice;
    pressureDevice.deviceId = 2;
    pressureDevice.deviceName = "压力传感器_01";
    pressureDevice.deviceType = "压力";
    pressureDevice.currentValue = 1.2;
    pressureDevice.minValue = 0.5;
    pressureDevice.maxValue = 2.5;
    pressureDevice.unit = "MPa";
    pressureDevice.timestamp = QDateTime::currentDateTime();
    pressureDevice.isOnline = true;
    pressureDevice.warningLevel = 0;
    m_devices.append(pressureDevice);
}

DeviceDataModel::~DeviceDataModel()
{
    clearAll();
}

int DeviceDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_devices.size();
}

int DeviceDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColCount;
}

QVariant DeviceDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_devices.size())
        return QVariant();

    const DeviceData &device = m_devices.at(index.row());
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (col) {
        case ColId: return device.deviceId;
        case ColName: return device.deviceName;
        case ColType: return device.deviceType;
        case ColValue: return QString::number(device.currentValue, 'f', 2);
        case ColMin: return QString::number(device.minValue, 'f', 1);
        case ColMax: return QString::number(device.maxValue, 'f', 1);
        case ColUnit: return device.unit;
        case ColTime: return device.timestamp.toString("MM-dd hh:mm:ss");
        case ColStatus: return device.isOnline ? "在线" : "离线";
        case ColWarning:
            switch (device.warningLevel) {
            case 0: return "正常";
            case 1: return "警告";
            case 2: return "严重";
            default: return "未知";
            }
        }
        break;

    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;

    case Qt::BackgroundRole:
        if (col == ColWarning) {
            switch (device.warningLevel) {
            case 1: return QBrush(QColor(255, 255, 150));  // 黄色警告
            case 2: return QBrush(QColor(255, 150, 150));  // 红色严重
            }
        }
        break;

    case Qt::ForegroundRole:
        if (!device.isOnline) {
            return QBrush(Qt::gray);
        }
        break;
    }

    return QVariant();
}

QVariant DeviceDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColId: return "ID";
        case ColName: return "设备名称";
        case ColType: return "类型";
        case ColValue: return "当前值";
        case ColMin: return "最小值";
        case ColMax: return "最大值";
        case ColUnit: return "单位";
        case ColTime: return "更新时间";
        case ColStatus: return "状态";
        case ColWarning: return "报警";
        }
    }
    return QVariant();
}

bool DeviceDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_devices.size() || role != Qt::EditRole)
        return false;

    DeviceData &device = m_devices[index.row()];
    int col = index.column();

    switch (col) {
    case ColName: device.deviceName = value.toString(); break;
    case ColMin: device.minValue = value.toDouble(); break;
    case ColMax: device.maxValue = value.toDouble(); break;
    case ColUnit: device.unit = value.toString(); break;
    default: return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags DeviceDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    // 允许编辑的列
    int col = index.column();
    if (col == ColName || col == ColMin || col == ColMax || col == ColUnit) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

void DeviceDataModel::addDevice(const DeviceData &device)
{
    beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
    m_devices.append(device);
    endInsertRows();
}

void DeviceDataModel::updateDeviceValue(int deviceId, double newValue)
{
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i].deviceId == deviceId) {
            m_devices[i].currentValue = newValue;
            m_devices[i].timestamp = QDateTime::currentDateTime();

            // 检查报警
            int newLevel = checkWarningLevel(m_devices[i], newValue);
            if (newLevel != m_devices[i].warningLevel) {
                m_devices[i].warningLevel = newLevel;
                emit warningTriggered(deviceId, newLevel);
            }

            // 更新显示
            QModelIndex idx = createIndex(i, ColValue);
            emit dataChanged(idx, idx);

            idx = createIndex(i, ColTime);
            emit dataChanged(idx, idx);

            idx = createIndex(i, ColWarning);
            emit dataChanged(idx, idx);

            emit deviceDataChanged(deviceId);
            break;
        }
    }
}

void DeviceDataModel::removeDevice(int deviceId)
{
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i].deviceId == deviceId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_devices.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

DeviceData DeviceDataModel::getDevice(int row) const
{
    if (row >= 0 && row < m_devices.size())
        return m_devices[row];
    return DeviceData();
}

DeviceData DeviceDataModel::getDeviceById(int deviceId) const
{
    for (const auto &device : m_devices) {
        if (device.deviceId == deviceId)
            return device;
    }
    return DeviceData();
}

void DeviceDataModel::clearAll()
{
    beginResetModel();
    m_devices.clear();
    endResetModel();
}

int DeviceDataModel::checkWarningLevel(const DeviceData &device, double newValue) const
{
    double range = device.maxValue - device.minValue;
    if (range <= 0) return 0;

    double normalized = (newValue - device.minValue) / range;

    if (normalized < 0.1 || normalized > 0.9) {
        return 2;  // 严重：超出安全范围10%
    } else if (normalized < 0.2 || normalized > 0.8) {
        return 1;  // 警告：接近边界
    }
    return 0;      // 正常
}
