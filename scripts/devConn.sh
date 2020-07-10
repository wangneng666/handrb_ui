#!/bin/bash
  roslaunch hsr_bringup co605_fight_go.launch &
  rosrun gripper_bridge opensensor.sh &
  roslaunch gripper_bridge gripper_bridge_finger.launch &
  sleep 5
  rosrun gripper_bridge five_finger_gripper.sh
wait
exit 0
