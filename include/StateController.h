#ifndef HANDRB_UI_STATECONTROLLER_H
#define HANDRB_UI_STATECONTROLLER_H

#include <iostream>
#include "ros/ros.h"
#include "RbQthread.h"
#include "MainWindow.h"
#include <QMutex>

//消息类型头文件
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int16.h"
#include "rb_msgAndSrv/rb_DoubleBool.h"
#include "rb_msgAndSrv/rb_string.h"
#include "rb_msgAndSrv/rb_EmptyAndArray.h"
#include "sensor_msgs/Image.h"
#include <cv_bridge/cv_bridge.h>
#include "hsr_rosi_device/ClearFaultSrv.h"
#include "hsr_rosi_device/SetEnableSrv.h"
#include "industrial_msgs/RobotStatus.h"
#include "geometry_msgs/Wrench.h"
#include "hsr_rosi_device/setModeSrv.h"
#include "hirop_msgs/robotError.h"
using namespace std;


//业务逻辑控制
class StateController {
public:
    StateController();
    ~StateController();

public:
    int mode=0;//模式
    int voiceStep=0;//追踪步序
    controllerState* ctlState;//状态集合容器
    rosTopicHandle* rosTopicHd;//ros话题复位数据容器
    bool isStart= false;//开始运行按钮
    bool isStop= false;
    bool assist_funcRunOnce= false;//辅助_运行一次函数
    ros::NodeHandle* Node;//ros节点
    QMutex mutex_updateCtlState;
    //测试
    QLabel* lable_showinfo;
    //线程
    vector<rbQthread*> rbQthreadList;
    rbQthread* rbQthread_voiceCtl_AutoRun;
    rbQthread* rbQthread_updateState;
    rbQthread* rbQthread_voiceCtl_modeTask_N1;//声控模式-1任务
    rbQthread* rbQthread_voiceCtl_modeTask_1;//声控模式1任务
    rbQthread* rbQthread_voiceCtl_modeTask_3;//声控模式3任务
    rbQthread* rbQthread_voiceCtl_modeTask_4;//声控模式4任务
    rbQthread* rbQthread_voiceCtl_modeTask_5;//声控模式5任务

    rbQthread* rbQthread_rbGoShakeHandPose;//机器人去到等待点

    rbQthread* rbQthread_spin;
    //获取服务和话题
    void ShareTopicHandle(rosTopicHandle* _rosTopicHd);
    //使用线程一直更新数据
    void updateState(controllerState* _ctlState);
    //开始
    void start();
    //自动运行
    void voiceCtl_AutoRun();
    //模式为-1执行功能
    void ContrlMode_N1_task();

    //线程函数
    void thread_voiceCtl_modeTask_N1();
    void thread_voiceCtl_modeTask_1();
    void thread_voiceCtl_modeTask_3();
    void thread_voiceCtl_modeTask_4();
    void thread_voiceCtl_modeTask_5();

    void thread_rbGoShakeHandPose();

    void thread_rbQthread_spin();

    //行人检测开关
    void PersonDetect_Switch(bool flag);
    //语音检测开关
    void VoiceDetect_Switch(bool flag);





};


#endif //HANDRB_UI_STATECONTROLLER_H
