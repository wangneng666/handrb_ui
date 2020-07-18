#!/bin/bash
  roslaunch hsr_bringup co605_finger_go.launch &
  rosrun gripper_bridge opensensor.sh &
  roslaunch gripper_bridge gripper_bridge_finger.launch &
  rosrun openni2_tracker voice.sh &
  rosrun handrb_ui launch_vision.sh &
  rosrun hsc3api_bridge hsc3api_bridge &
  sleep 3
  rosrun gripper_bridge opensensor.sh &
  rosrun gripper_bridge five_finger_gripper.sh
wait
exit 0
