#!/bin/bash
  roslaunch hsr_bringup co605_fight_go.launch &
  sleep 6
  rosservice call /clear_robot_fault "{}" 
  sleep 1
  rosservice call /set_robot_enable "enable: true" 
exit 0
