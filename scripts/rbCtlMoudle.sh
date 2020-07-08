#!/bin/bash
  roslaunch pick_place_bridge bringup.launch &
  roslaunch reception_robot fake_bringup.launch &
wait
exit 0
