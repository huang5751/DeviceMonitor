项目名称
设备监控系统
📍 核心文件
main.cpp - 程序入口
创建Qt应用程序
初始化主窗口
启动事件循环
mainwindow.cpp - 主窗口管理
构建用户界面框架
协调各子组件
处理全局用户交互
devicedatamodel.cpp - 数据模型
存储设备数据
管理数据状态
通知数据变化
🌐 网络通信模块
tcpserver.cpp - TCP服务器
监听网络端口
接收设备数据
数据解析转发
🔧 设备模拟模块
devicesimulator.cpp - 设备模拟器
生成虚拟设备数据
配置模拟参数
控制模拟过程
simulationworker.cpp - 模拟工作线程
后台生成数据
定时发送数据
线程安全通信
📊 可视化组件模块
chartwidget.cpp - 图表组件
绘制实时数据曲线
支持图表交互
动态更新显示
dashboardwidget.cpp - 仪表盘组件
展示设备关键指标
显示状态指示灯
提供报警提示
🚀 快速开始
编译与运行
bash
克隆项目
git clone https://jihulab.com/hyf0003/devicemonitor.git
进入项目目录
cd devicemonitor
编译项目
qmake
运行程序
./devicemonitor.exe
依赖环境：
Qt 5.15
C++11
qmake
📊 功能特性
✅ 实时数据监控：通过TCP接收并显示设备数据
✅ 设备模拟：支持虚拟设备数据生成
✅ 数据可视化：图表和仪表盘展示
✅ 多线程处理：后台数据生成不影响UI响应
✅ 模块化设计：各功能模块独立，易于维护
🛠️ 配置说明
网络配置
cpp
// TCP服务器配置
服务器端口：默认 8080
数据格式：自定义协议
模拟器配置
cpp
// 设备模拟器参数
数据生成频率：可配置
模拟设备数量：支持多设备
数据类型：支持多种传感器数据

最后更新时间：2025年12月18日