roslaunch hsr_bringup publish_d435i_calibration_five_finger.launch &
roslaunch realsense2_camera rs_camera.launch &
roslaunch vision_bridge vision_bridge_yolo6d_hand_realsense2.launch &

rosparam set /grasp_place/position_x_add 0.015
rosparam set /grasp_place/position_y_add -0.03
# rosparam set /grasp_place/position_z_add -0.07
