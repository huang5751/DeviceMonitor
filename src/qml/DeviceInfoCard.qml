// src/qml/DeviceInfoCard.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: deviceCard
    width: 300
    height: 200
    radius: 12

    // === 绑定到C++的属性 ===
    property string deviceName: "未选择设备"
    property real currentValue: 0.0
    property string unit: "单位"
    property int warningLevel: 0
    property bool isOnline: false

    // === 计算属性（根据绑定属性自动计算）===
    property color cardColor: {
        if (!isOnline) return "#7f8c8d"
        if (warningLevel === 2) return "#e74c3c"
        if (warningLevel === 1) return "#f39c12"
        return "#3498db"
    }

    // 组件加载完成时播放进入动画
    Component.onCompleted: {
        entranceAnimation.start();
    }

    // 数值更新时播放动画
    onCurrentValueChanged: {
        if (currentValue !== 0) {
            valueUpdateAnimation.start();
        }
    }

    // 警告级别变化时
    onWarningLevelChanged: {
        if (warningLevel > 0) {
            warningAnimation.start();
        } else {
            warningAnimation.stop();
        }
    }

    // 在线状态变化时
    onIsOnlineChanged: {
        // 可以添加在线状态变化的动画
    }
}
