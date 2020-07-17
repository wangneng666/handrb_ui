#ifndef HANDRB_UI_GLOALVAL_H
#define HANDRB_UI_GLOALVAL_H

#include "ros/ros.h"

#ifndef INFOLEVEL_H
#define INFOLEVEL_H
enum  infoLevel{information,warning};
#endif

#ifndef CONCTROLLERSTATE
#define CONCTROLLERSTATE

struct controllerState{
    bool RobNormalState= false;//机器人正常状态
    bool RobEnableState= false;//机器人伺服状态
    bool flag_rbCtlBusy= false;//机器人空闲状态
    int voice_order= -1;//语音指令码
    bool isEnd_shakeHand= false;//握手结束
    bool isOk_robPreparePose= false;
    bool grab_ok=false;
    bool detect_object_ok=false;
    bool err_PlanPose=false;
};

struct rosTopicHandle{
    ros::ServiceClient* RobReset_client;
    ros::ServiceClient* RobEnable_client;
    ros::ServiceClient* handClaw_gesture_client;
    ros::ServiceClient* handClaw_shakeHand_client;
    ros::ServiceClient* handClaw_grabDoll_client;
    ros::ServiceClient* rob_goHome_client;
    ros::ServiceClient* RobSetMode_client;
    ros::ServiceClient* robGetStatus_client;
    ros::ServiceClient* personDetect_client;
    ros::ServiceClient* switch_personDetect_client;
    ros::ServiceClient* switch_voiceDetect_client;
    ros::ServiceClient* backHomeClient ;
    ros::ServiceClient* detectePointClient;
    ros::ServiceClient* stopMotionClient;
    ros::ServiceClient* detectionClient;
    ros::ServiceClient* sayGoodByeAction_client;


    ros::Publisher* voice_order_publisher;
    ros::Publisher* visionDetech_publisher;
    ros::Publisher* rbGoHome_publisher;
    ros::Publisher* flag_forceSensor_publisher;
    ros::Publisher* impedenceLive_publisher;
    ros::Publisher* shakehandOver_publisher;
    ros::Publisher* robStatusSend_publisher;
    ros::Publisher* robSpeedSet_publisher;
};
#endif


#endif //HANDRB_UI_GLOALVAL_H
