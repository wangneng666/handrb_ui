
#ifndef RB_UI_MAINWINDOW_H
#define RB_UI_MAINWINDOW_H

#include "BaseWindow.h"
#include "RbQthread.h"
#include "gloalVal.h"
#include "logmanager.h"
//opencv库
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//消息类型头文件
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int8.h"
#include "rb_msgAndSrv/rb_DoubleBool.h"
#include "rb_msgAndSrv/rb_string.h"
#include "sensor_msgs/Image.h"
#include <cv_bridge/cv_bridge.h>
#include "hsr_rosi_device/ClearFaultSrv.h"
#include "hsr_rosi_device/SetEnableSrv.h"
#include "industrial_msgs/RobotStatus.h"
#include "geometry_msgs/Wrench.h"
//标准库
#include "ros/ros.h"
#include <iostream>
#include <fstream>
using namespace std;


struct  devDetector{
    string name; //设备名
    int lifeNum; //生命值
    bool status; //状态
    QLabel* lable_showStatus;//状态显示lable
};

//观察者模式(管理节点重启与关闭)
class observer_rebootUiNode{
public:
    MainWindow* mainwindow;
    ros::AsyncSpinner* sp= nullptr;
public:
    void setparm(MainWindow* mainwindow){
        this->mainwindow=mainwindow;
    }
    //释放内存
    void shutdownNode(){
        ros::shutdown();
        if(sp!= nullptr){
            delete sp;
            sp= nullptr;
        }
    }
    //重启ui节点
    void rebootUiNode();
};


class MainWindow: public BaseWindow {
Q_OBJECT
public:
    MainWindow(ros::NodeHandle* node,QWidget* parent = Q_NULLPTR);
    ~MainWindow();
private:
    //全局变量
    int index_sysMode=0;
    int index_SysRunStep=0;
    QMutex mutex_devDetector;
    bool flag_switchPersonDecBtnText= false;
    bool flag_havedReset= false;
    bool flag_rbEnable= false;
    //加入节点观察者
    observer_rebootUiNode ob_node;
    //设备监控
    vector<devDetector*> devDetectorList;//设备监控器列表
    devDetector RobConn_Detector{"RobConn_Detector",0, false,label_tabmain_rbConnStatusValue}; //机器人连接状态
    devDetector RobErr_Detector{"RobErr_Detector",0, false,label_tabmain_rbErrStatusValue}; //机器人故障状态 fasle有故障,true没故障
    devDetector RobEnable_Detector{"RobEnable_Detector",0, false,label_tabmain_rbservoStatusValue}; //机器人伺服状态
    devDetector cameraConn_Detector{"cameraConn_Detector",0, false,label_tabmain_caConnStatusValue}; //相机连接状态
    devDetector handClawConn_Detector{"handClawConn_Detector",0, false,label_tabmain_5fConnStatusValue}; //五指夹爪连接状态
    devDetector forceSensorConn_Detector{"forceSensorConn_Detector",0, false,label_tabmain_TsensorStatusValue}; //六轴力传感器连接状态
    //定时器
    QTimer* Timer_listen_status;
    QTimer* Timer_listen_SysResetThread;
    QTimer* Timer_listen_SysErr;
    //ros消息对象
    ros::ServiceClient RobReset_client;
    ros::ServiceClient RobEnable_client;
    ros::ServiceClient handClaw_gesture_client;
    ros::ServiceClient handClaw_shakeHand_client;
    ros::ServiceClient handClaw_grabDoll_client;
    ros::ServiceClient rob_goHome_client;
    ros::Subscriber voiceSolveRes_subcriber;
    ros::Subscriber personDetectRes_subcriber;
    ros::Subscriber grabDollImagRes_subcriber;
    ros::Subscriber robStatus_subscriber;
    ros::Subscriber camera_subscriber;
    ros::Subscriber forceSensor_subscriber;
    ros::Publisher visionDetech_publisher;
    ros::Publisher rbGoHome_publisher;
    ros::Publisher flag_forceSensor_publisher;
    //线程句柄
    vector<rbQthread*> rbQthreadList;
    rbQthread* rbQthread_devConnOrRviz;
    rbQthread* rbQthread_beginRun;
    rbQthread* rbQthread_sysStop;
    rbQthread* rbQthread_sysReset;
    rbQthread* rbQthread_persondeteck;
    rbQthread* rbQthread_voicedeteck;
    rbQthread* rbQthread_shakehand;
    rbQthread* rbQthread_grepwawa;
    rbQthread* rbQthread_handClaw_gesture;

public:
    //初始化Ros话题与服务
    void initRosToptic();
private:
    //系统变量初始化
    void SysVarInit();
    //处理所有信号和槽函数
    void signalAndSlot();

//*****************************槽函数*******************************************
    //主界面槽函数
    void slot_btn_tabmain_devConnOrRviz();//设备连接
    void slot_btn_tabmain_beginRun();//开始运行
    void slot_btn_tabmain_sysStop();//设备停止
    void slot_btn_tabmain_sysReset();//系统复位
    //调试界面槽函数
    void slot_btn_rbSetEnable();//机器人上使能
    void slot_btn_rbReset();//机器人复位
    void slot_btn_gripper_open();//五指夹爪打开
    void slot_btn_gripper_close();//五指夹爪关闭
    void slot_btn_gripper_OK_Pose();//五指夹爪OK手势
    void slot_btn_gripper_Y_Pose();//五指夹爪Y手势
    void slot_btn_rbGoHomePose();//机器人回原点
    //功能界面槽函数
    void slot_btn_tabfunc_shakehand();//机器人握手
    void slot_btn_tabfunc_grepwawa();//机器人抓娃娃
    void slot_btn_tabfunc_persondeteck();//行人检测
    void slot_btn_tabfunc_voiceDetect();//语音检测
    //日志界面槽函数
    void slot_btn_tabrecord_outRecord();//导出日志
    void slot_btn_tabrecord_clearRecord();//清除日志
//*********************************************************************************
    //ros节点回调函数
    void callback_voiceSolveRes_subcriber(std_msgs::String msg);
    void callback_personDetectRes_subcriber(const sensor_msgs::Image::ConstPtr& msg);
    void callback_grabDollImagRes_subcriber(const sensor_msgs::Image::ConstPtr& msg);
    void callback_robStatus_subscriber(const industrial_msgs::RobotStatus::ConstPtr robot_status);
    void callback_camera_subscriber(const sensor_msgs::Image::ConstPtr& msg);
    void callback_forceSensor_subscriber(geometry_msgs::Wrench msg);
    //定时器槽函数
    void slot_timer_listen_status();
    void slot_timer_listen_SysResetThread();
    void slot_timer_listenSysErrThread();
    //线程函数
    void thread_rbQthread_devConnOrRviz();
    void thread_rbQthread_beginRun();
    void thread_rbQthread_sysStop();
    void thread_rbQthread_sysReset();
    void thread_rbQthread_persondeteck();//行人检测
    void thread_rbQthread_voicedeteck();//语音检测
    void thread_rbQthread_shakehand();//与人握手
    void thread_rbQthread_grepwawa();//抓娃娃
    void thread_rbQthread_handClaw_gesture(string gesture);//手势
    //其他工具函数
    QImage cvMat2QImage(const cv::Mat& mat);

signals:
    void emitTextControl(QString text) const;
    void emitQmessageBox(infoLevel level,QString info);
    void emitLightColor(QLabel* label,string color);
    void emitStartTimer(QTimer* timer);


private slots:
    void displayTextControl(QString text);
    void showQmessageBox(infoLevel level,QString info);
    void showLightColor(QLabel* label,string color);
    void slot_combox_chooseMode_Clicked(int index);
    void slot_rbQthread_listenSysResetStart();//监听系统复位线程启动
    void slot_rbQthread_listenFinish();//监听线程资源释放
    void slot_runTimer(QTimer* timer);


};



#endif //RB_UI_MAINWINDOW_H
