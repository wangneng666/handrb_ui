#!/bin/bash
#去到Nite的库路径下
cd ~/catkin_ws

#启动行人检测节点
rosrun handrb_ui main 

wait
exit 0
