// src/qml/DeviceDashboard.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: dashboard
    width: 300
    height: 300

    property string deviceName: "设备"
    property real currentValue: 0.0
    property real minValue: 0.0
    property real maxValue: 100.0
    property string unit: "单位"
    property int warningLevel: 0

    // 圆弧仪表盘
    Canvas {
        id: gaugeCanvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            var centerX = width / 2;
            var centerY = height / 2;
            var radius = Math.min(centerX, centerY) - 20;

            // 绘制背景圆弧
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, Math.PI * 0.8, Math.PI * 2.2);
            ctx.lineWidth = 15;
            ctx.strokeStyle = "#34495e";
            ctx.stroke();

            // 绘制进度圆弧
            var progress = (currentValue - minValue) / (maxValue - minValue);
            var endAngle = Math.PI * 0.8 + progress * Math.PI * 1.4;

            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, Math.PI * 0.8, endAngle);
            ctx.lineWidth = 15;

            // 根据数值设置颜色
            if (warningLevel === 2) {
                ctx.strokeStyle = "#e74c3c";
            } else if (warningLevel === 1) {
                ctx.strokeStyle = "#f39c12";
            } else if (progress < 0.3) {
                ctx.strokeStyle = "#2ecc71";
            } else if (progress < 0.7) {
                ctx.strokeStyle = "#f1c40f";
            } else {
                ctx.strokeStyle = "#e74c3c";
            }

            ctx.stroke();

            // 绘制刻度
            for (var i = 0; i <= 10; i++) {
                var angle = Math.PI * 0.8 + (i / 10) * Math.PI * 1.4;
                var x1 = centerX + (radius + 5) * Math.cos(angle);
                var y1 = centerY + (radius + 5) * Math.sin(angle);
                var x2 = centerX + (radius + 15) * Math.cos(angle);
                var y2 = centerY + (radius + 15) * Math.sin(angle);

                ctx.beginPath();
                ctx.moveTo(x1, y1);
                ctx.lineTo(x2, y2);
                ctx.lineWidth = 2;
                ctx.strokeStyle = "white";
                ctx.stroke();
            }
        }

        // 数值变化时重绘
        onCurrentValueChanged: requestPaint()
        onWarningLevelChanged: requestPaint()
    }

    // 中心显示数值
    Text {
        anchors.centerIn: parent
        text: currentValue.toFixed(1) + "\n" + unit
        color: "white"
        font.pixelSize: 24
        font.bold: true
        horizontalAlignment: Text.AlignHCenter

        // 数值变化动画
        Behavior on text {
            SequentialAnimation {
                NumberAnimation {
                    target: parent
                    property: "scale"
                    from: 1.0
                    to: 1.2
                    duration: 150
                }
                NumberAnimation {
                    target: parent
                    property: "scale"
                    from: 1.2
                    to: 1.0
                    duration: 150
                }
            }
        }
    }

    // 设备名称
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        text: deviceName
        color: "white"
        font.pixelSize: 16
    }

    // 旋转指针
    Rectangle {
        id: needle
        width: 4
        height: gaugeCanvas.height / 2 - 40
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        transformOrigin: Item.Bottom

        // 指针旋转角度
        rotation: {
            var progress = (currentValue - minValue) / (maxValue - minValue);
            return 80 + progress * 140;  // 从-80度到60度
        }

        // 指针旋转动画
        Behavior on rotation {
            NumberAnimation {
                duration: 800
                easing.type: Easing.OutElastic
                easing.amplitude: 1.0
                easing.period: 0.5
            }
        }

        // 指针头部
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 12
            height: 12
            radius: 6
            color: "#e74c3c"
        }
    }
}
