// src/qml/SimpleTest.qml
import QtQuick 2.15

Rectangle {
    width: 300
    height: 200
    color: "#3498db"
    border.color: "yellow"
    border.width: 3
    radius: 10

    Text {
        anchors.centerIn: parent
        text: "QML测试成功!"
        color: "white"
        font.bold: true
        font.pixelSize: 20
    }

    // 一个简单的动画
    Rectangle {
        x: 50
        y: 50
        width: 50
        height: 50
        color: "red"
        radius: 10

        RotationAnimation on rotation {
            from: 0
            to: 360
            duration: 2000
            loops: Animation.Infinite
        }
    }
}
