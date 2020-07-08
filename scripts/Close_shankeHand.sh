#!/bin/bash
  rosrun openni2_tracker voice_shutdown.sh
  rosnode kill /hsr_impedance
wait
exit 0
