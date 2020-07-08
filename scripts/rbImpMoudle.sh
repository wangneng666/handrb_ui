#!/bin/bash
  sleep 5
  rosservice call /clear_robot_fault "{}" 
  rosservice call /set_mode_srv "mode: 1" 
  rosservice call /set_robot_enable "enable: true"
  rosrun hsr_impedance hsr_impedance
wait
exit 0
