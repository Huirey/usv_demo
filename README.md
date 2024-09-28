# usv_demo
基于VRX平台的水面无人艇(USV)仿真环境，实现无人艇的导航功能

VRX平台的下载与与安装可以参考https://github.com/osrf/vrx/tree/gazebo_classic

## 简介
仿真环境基于VRX

使用VRX中的CORA模型

版本:ubuntu20.04 ros-noetic


使用robotlocalization融合imu和gps定位

使用movebase导航包进行导航

运动控制目前尚未完成，仅仅做了简单的pid控制前进后退，固定推力转向

注意替换usv_nav/map/nav.yaml中的路径

## 使用方法

下载robot_localization用于融合定位(以noetic版本为例)
```
sudo apt install ros-noetic-robot-localization
```

编译
```
cd usv_demo
catkin_make
```
打开仿真环境

！！一定要先安装vrx_classic环境！！
```
roslaunch usv_world_model usv_env.launch
```
打开导航
```
roslaunch usv_nav usv_test.launch
```
