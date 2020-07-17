#include "MainWindow.h"
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf/transform_listener.h>
#include "pick_place_bridge/PickPlacePose.h"


MainWindow::MainWindow(ros::NodeHandle *node, QWidget *parent):BaseWindow(node,parent){
    //系统变量初始化
    SysVarInit();
    //初始化rostopic
    initRosToptic();
    //信号与槽绑定
    signalAndSlot();

    stateController->lable_showinfo=label_tab_voiceMonitor_1;
    stateController->plainTextEdit=plainTextEdit_2;
    ctrlState.RobNormalState= true;
    stateController->updateState(&ctrlState);
}

MainWindow::~MainWindow() {
    system((char*)"rosnode kill -a");
    //关闭ros相关进程
//    system("kill $(ps -ef | grep ros|awk '{print  $2}')");
}

void MainWindow::SysVarInit() {
    //系统设备监控器
    devDetectorList.push_back(&RobConn_Detector);
    devDetectorList.push_back(&RobErr_Detector);
    devDetectorList.push_back(&RobEnable_Detector);
    devDetectorList.push_back(&cameraConn_Detector);
    devDetectorList.push_back(&handClawConn_Detector);
    devDetectorList.push_back(&forceSensorConn_Detector);
    devDetectorList.push_back(&impedenceConn_Detector);

    //定时器初始化
    Timer_listen_status = new QTimer(this);
    Timer_listen_status->setInterval(100);
    connect(Timer_listen_status, &QTimer::timeout, this, &MainWindow::slot_timer_listen_status);
    Timer_listen_status->start();

    Timer_listen_SysResetThread = new QTimer(this);
    Timer_listen_SysResetThread->setInterval(5000);
    connect(Timer_listen_SysResetThread, &QTimer::timeout, this, &MainWindow::slot_timer_listen_SysResetThread);

    Timer_listen_SysErr = new QTimer(this);
    Timer_listen_SysErr->setInterval(6000);
    connect(Timer_listen_SysErr, &QTimer::timeout, this, &MainWindow::slot_timer_listenSysErrThread);

    //一键启动自动握手模式
    Timer_forAutoRunShakeHand = new QTimer(this);
    Timer_forAutoRunShakeHand->setInterval(1000);
    connect(Timer_forAutoRunShakeHand, &QTimer::timeout, this, &MainWindow::slot_timer_AutoRun_shakeHand);

    qRegisterMetaType<infoLevel>("infoLevel");//信号与槽连接自定义类型需要注册

    //初始化节点观察者ROS_INFO_STREAM("----back home ...----");
    ob_node.setparm(this);
    //线程初始化

    rbQthread_devConn = new rbQthread();
    rbQthread_devConn->setParm(this,&MainWindow::thread_rbQthread_devConn);

    rbQthread_beginRun = new rbQthread();
    rbQthread_beginRun->setParm(this,&MainWindow::thread_rbQthread_beginRun);

    rbQthread_sysReset = new rbQthread();
    rbQthread_sysReset->setParm(this,&MainWindow::thread_rbQthread_sysReset);

    rbQthread_persondeteck = new rbQthread();
    rbQthread_persondeteck->setParm(this,&MainWindow::thread_rbQthread_persondeteck);

    rbQthread_voicedeteck = new rbQthread();
    rbQthread_voicedeteck->setParm(this,&MainWindow::thread_rbQthread_voicedeteck);

    rbQthread_handClaw_gesture = new rbQthread();

    //监听故障子线程
    rbQthread_lisionRbErrInfo= new rbQthread();
    rbQthread_lisionRbErrInfo->setParm(this,&MainWindow::thread_rbQthread_LisionRbErrInfo);
    /********************************************************************************************/
    rbQthread_rbRunMoudlePrepare = new rbQthread();
    rbQthread_rbCtlMoudlePrepare = new rbQthread();
    rbQthread_rbImpMoudlePrepare = new rbQthread();
    rbQthread_rbVoiceMoudlePrepare = new rbQthread();
    rbQthread_rbRunMoudlePrepare->setParm(this,&MainWindow::thread_rbQthread_rbRunMoudlePrepare);
    rbQthread_rbCtlMoudlePrepare->setParm(this,&MainWindow::thread_rbQthread_rbCtlMoudlePrepare);
    rbQthread_rbImpMoudlePrepare->setParm(this,&MainWindow::thread_rbQthread_rbImpMoudlePrepare);
    rbQthread_rbVoiceMoudlePrepare->setParm(this,&MainWindow::thread_rbQthread_rbVoiceMoudlePrepare);

    rbQthread_shakehand = new rbQthread();
    rbQthread_shakehand->setParm(this,&MainWindow::thread_rbQthread_shakehand);

    rbQthread_sysStop = new rbQthread();
    rbQthread_sysStop->setParm(this,&MainWindow::thread_rbQthread_sysStop);

    rbQthread_grepwawa = new rbQthread();
    rbQthread_grepwawa->setParm(this,&MainWindow::thread_rbQthread_grepwawa);

    rbQthreadList.push_back(rbQthread_devConn);
    rbQthreadList.push_back(rbQthread_beginRun);
    rbQthreadList.push_back(rbQthread_sysStop);
//    rbQthreadList.push_back(rbQthread_persondeteck);
//    rbQthreadList.push_back(rbQthread_voicedeteck);
//    rbQthreadList.push_back(rbQthread_shakehand);
//    rbQthreadList.push_back(rbQthread_grepwawa);
    stateController=new StateController();

}

void MainWindow::initRosToptic(){
    //话题
    flag_forceSensor_publisher=Node->advertise<std_msgs::Bool>("forceSensor_moveFlag",1);
    shakehandOver_publisher=Node->advertise<std_msgs::Bool>("shake_over", 10);
    robStatusSend_publisher=Node->advertise<std_msgs::Bool>("uipub_robStatus",1);
    impedenceLive_publisher=Node->advertise<std_msgs::Bool>("uipub_impedenceLive",1);
    rbGoHome_publisher=Node->advertise<std_msgs::Int8>("homePoint",1);
    visionDetech_publisher=Node->advertise<std_msgs::Bool>("switch_of_vision_detect",1000);
    voice_order_publisher = Node->advertise<std_msgs::String>("voiceSolve_res", 1);
    robSpeedSet_publisher = Node->advertise<std_msgs::Float32>("speedScale", 1);
    camera_subscriber=Node->subscribe<sensor_msgs::Image>("/usb_cam/image_raw",1,boost::bind(&MainWindow::callback_camera_subscriber, this, _1));
    rbCtlBusy_subscriber=Node->subscribe<std_msgs::Bool>("rbCtlBusy_status",1,&MainWindow::callback_rbCtlBusy_status_subscriber, this);
//    camera_subscriber=Node->subscribe<sensor_msgs::Image>("/usb_cam/image_raw",1,boost::bind(&MainWindow::callback_camera_subscriber, this, _1));
    forceSensor_subscriber=Node->subscribe<geometry_msgs::Wrench>("daq_data", 1000, &MainWindow::callback_forceSensor_subscriber, this);
    voiceSolveRes_subcriber=Node->subscribe<std_msgs::Int16>("voice_order",1,&MainWindow::callback_voiceSolveRes_subcriber, this);

    voice_order_publisher = Node->advertise<std_msgs::String>("voiceSolve_res", 1);

    personDetectRes_subcriber=Node->subscribe<sensor_msgs::Image>("videphoto_feedback",1,boost::bind(&MainWindow::callback_personDetectRes_subcriber, this, _1));
    grabDollImagRes_subcriber=Node->subscribe<sensor_msgs::Image>("DollDetection_image",1,boost::bind(&MainWindow::callback_grabDollImagRes_subcriber, this, _1));
    robStatus_subscriber=Node->subscribe<industrial_msgs::RobotStatus>("robot_status",1,boost::bind(&MainWindow::callback_robStatus_subscriber,this,_1));
    impedenceLive_subscriber=Node->subscribe<sensor_msgs::JointState>("impedance_err",1,&MainWindow::callback_impedenceLive_subscriber,this);
    isOpenFollow_subscriber=Node->subscribe<std_msgs::Bool>("is_follow",1,&MainWindow::callback_isOpenFollow_subscriber,this);
    getShakeHandResult_subscriber = Node->subscribe<std_msgs::Int16>("impedance_result",1,&MainWindow::callback_getShakeResult_subscriber,this);
    //服务
    RobReset_client = Node->serviceClient<hsr_rosi_device::ClearFaultSrv>("/clear_robot_fault");
    RobEnable_client = Node->serviceClient<hsr_rosi_device::SetEnableSrv>("/set_robot_enable");
    handClaw_gesture_client = Node->serviceClient<rb_msgAndSrv::rb_string>("handClaw_gesture");
    handClaw_shakeHand_client = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("handClaw_shakeHand");
    handClaw_grabDoll_client = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("handClaw_grabDoll");
    rob_goHome_client = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("rob_goHome");
    RobSetMode_client = Node->serviceClient<hsr_rosi_device::setModeSrv>("/set_mode_srv");
    robGetStatus_client = Node->serviceClient<hirop_msgs::robotError>("getRobotErrorFaultMsg");
    personDetect_client = Node->serviceClient<rb_msgAndSrv::rb_EmptyAndInt>("get_people_track_state");

    //开关语音检测与图像检测
    switch_personDetect_client=Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("switch_personDetect");
    switch_voiceDetect_client=Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("switch_voiceDetect");

    backHomeClient = Node->serviceClient<std_srvs::SetBool>("/back_home");
    detectePointClient = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("/handClaw_detectDoll");
    stopMotionClient = Node->serviceClient<industrial_msgs::StopMotion>("/stop_motion");
    //抓娃娃
    detectionClient = Node->serviceClient<hirop_msgs::detection>("/detection");
    // 订阅
    objectArraySub = Node->subscribe<hirop_msgs::ObjectArray>("object_array", 1, &MainWindow::callback_objectCallBack, this);
    pickServer_client = Node->serviceClient<pick_place_bridge::PickPlacePose>("pick");
    placeServer_client = Node->serviceClient<pick_place_bridge::PickPlacePose>("place");
    sayGoodByeAction_client = Node->serviceClient<std_srvs::SetBool>("sayGoodByeAction");

    rosTopicHd.RobReset_client=&RobReset_client;
    rosTopicHd.RobEnable_client=&RobEnable_client;
    rosTopicHd.handClaw_gesture_client=&handClaw_gesture_client;
    rosTopicHd.handClaw_shakeHand_client=&handClaw_shakeHand_client;
    rosTopicHd.handClaw_grabDoll_client=&handClaw_grabDoll_client;
    rosTopicHd.rob_goHome_client=&rob_goHome_client;
    rosTopicHd.RobSetMode_client=&RobSetMode_client;
    rosTopicHd.robGetStatus_client=&robGetStatus_client;
    rosTopicHd.personDetect_client=&personDetect_client;
    rosTopicHd.switch_personDetect_client=&switch_personDetect_client;
    rosTopicHd.switch_voiceDetect_client=&switch_voiceDetect_client;
    rosTopicHd.backHomeClient=&backHomeClient;
    rosTopicHd.detectePointClient=&detectePointClient;
    rosTopicHd.stopMotionClient=&stopMotionClient;
    rosTopicHd.detectionClient=&detectionClient;
    rosTopicHd.sayGoodByeAction_client=&sayGoodByeAction_client;


    rosTopicHd.flag_forceSensor_publisher=&flag_forceSensor_publisher;
    rosTopicHd.shakehandOver_publisher=&shakehandOver_publisher;
    rosTopicHd.robStatusSend_publisher=&robStatusSend_publisher;
    rosTopicHd.impedenceLive_publisher=&impedenceLive_publisher;
    rosTopicHd.rbGoHome_publisher=&rbGoHome_publisher;
    rosTopicHd.visionDetech_publisher=&visionDetech_publisher;
    rosTopicHd.voice_order_publisher=&voice_order_publisher;
    rosTopicHd.robSpeedSet_publisher=&robSpeedSet_publisher;

    stateController->ShareTopicHandle(&rosTopicHd);

}

void MainWindow::signalAndSlot() {
/*********************************控件与槽函数绑定*************************************************/
    //主界面
    connect(btn_tabmain_devConn,&QPushButton::clicked,this,&MainWindow::slot_btn_tabmain_devConn);
    connect(btn_tabmain_beginRun,&QPushButton::clicked,this,&MainWindow::slot_btn_tabmain_beginRun);
    connect(btn_tabmain_sysStop,&QPushButton::clicked,this,&MainWindow::slot_btn_tabmain_sysStop);
    connect(btn_tabmain_sysReset,&QPushButton::clicked,this,&MainWindow::slot_btn_tabmain_sysReset);
    connect(cbox_tabmain_chooseMode,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_combox_chooseMode_Clicked(int)));
    //调试界面
    connect(btn_rbSetEnable,&QPushButton::clicked,this,&MainWindow::slot_btn_rbSetEnable);
    connect(btn_rbReset,&QPushButton::clicked,this,&MainWindow::slot_btn_rbReset);
    connect(btn_gripper_open,&QPushButton::clicked,this,&MainWindow::slot_btn_gripper_open);
    connect(btn_gripper_close,&QPushButton::clicked,this,&MainWindow::slot_btn_gripper_close);
    connect(btn_gripper_OK_Pose,&QPushButton::clicked,this,&MainWindow::slot_btn_gripper_OK_Pose);
    connect(btn_gripper_Y_Pose,&QPushButton::clicked,this,&MainWindow::slot_btn_gripper_Y_Pose);
    connect(btn_rbGoHomePose,&QPushButton::clicked,this,&MainWindow::slot_btn_rbGoHomePose);
    connect(btn_startSensor,&QPushButton::clicked,this,&MainWindow::slot_btn_startSensor);
    //握手界面
    connect(btn_tabShakeHand_startRobRun,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_startRobRun);
    connect(btn_tabShakeHand_startRobCtl,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_startRobCtl);
    connect(btn_tabShakeHand_startimpedence,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_startimpedence);
    connect(btn_tabShakeHand_startvoice,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_startvoice);

    connect(btn_tabShakeHand_begin,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_begin);
    connect(btn_tabShakeHand_shakeHandEnd,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_shakeHandEnd);
    connect(btn_tabShakeHand_stop,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_stop);
    // connect(btn_tabShakeHand_close,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_close);
    connect(btn_tabShakeHand_AutoRun,&QPushButton::clicked,this,&MainWindow::slot_btn_tabShakeHand_AutoRun);
    connect(cBox_tabShakeHand_setMode,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_cBox_tabShakeHand_setMode(int )));

    //抓娃娃界面
    connect(btn_tabgrabToy_startRobRun,&QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_startRobRun);
    connect(btn_tabgrabToy_startRobCtl,&QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_startRobCtl);
    connect(btn_tabgrabToy_startvoice,&QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_startvoice);
    connect(btn_tab_grabToy_close,&QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_close);
//    connect(btn_tab_grabToy_GoDetectPose,&QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_run);
    connect(btn_tab_grabToy_GoDetectPose,&QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_stop);
    connect(btn_tab_grabToy_detect, &QPushButton::clicked,this,&MainWindow::slot_btn_tabgrabToy_grayDectectObj);

    //行人检测
    connect(btn_tab_personDetect_openPersonDetect,&QPushButton::clicked,this,&MainWindow::slot_btn_tab_personDetect_openPersonDetect);

    //语音控制页面
    connect(btn_tab_voiceDetect_run,&QPushButton::clicked,this,&MainWindow::slot_btn_tab_voiceDetect_run);

    //日志界面
    connect(btn_tabrecord_outRecord,&QPushButton::clicked,this,&MainWindow::slot_btn_tabrecord_outRecord);
    connect(btn_tabrecord_clearRecord,&QPushButton::clicked,this,&MainWindow::slot_btn_tabrecord_clearRecord);
/*************************************************************************************************/

/*********************************自定义信号与槽函数绑定*************************************************/
    connect(this, &MainWindow::emitTextControl,this, &MainWindow::displayTextControl);
    connect(this, &MainWindow::emitLightColor,this, &MainWindow::showLightColor);
    connect(this, SIGNAL(emitQmessageBox(infoLevel ,QString)), this,SLOT(showQmessageBox(infoLevel,QString)),Qt::QueuedConnection);  //将自定义槽连接到自定义信号

//    connect(rbQthread_devConnOrRviz, SIGNAL(signal_SendMsgBox(infoLevel ,QString)), this,SLOT(thread_slot_devConnOrRviz()));

/*************************************************************************************************/
}

//监听系统各设备状态
void MainWindow::slot_timer_listen_status() {
    mutex_devDetector.lock();
    for (auto detector : devDetectorList)
    {
        if(detector->lifeNum>0)
        {
            detector->lifeNum-=5;
        } else
        {
            detector->lifeNum=0;
            detector->status= false;
        }
        //刷新检测器标签状态
        if(detector->status)
        {
            emit emitLightColor(detector->lable_showStatus,"green");
        } else
        {
            emit emitLightColor(detector->lable_showStatus,"red");
        }
    }
    mutex_devDetector.unlock();
    //刷新页面标签状态
    if(RobEnable_Detector.status){
        label_tabShakeHand_rbStatusValue->setPixmap(fitpixmap_greenLight);
        label_tabgrabToy_rbStatusValue->setPixmap(fitpixmap_greenLight);
    } else{
        label_tabShakeHand_rbStatusValue->setPixmap(fitpixmap_redLight);
        label_tabgrabToy_rbStatusValue->setPixmap(fitpixmap_redLight);
    }

    if(rbQthread_rbCtlMoudlePrepare->isRunning()){
        label_tabShakeHand_rbCtlStatusValue->setPixmap(fitpixmap_greenLight);
        label_tab_grabToy_rbCtlStatusValue->setPixmap(fitpixmap_greenLight);
    } else{
        label_tabShakeHand_rbCtlStatusValue->setPixmap(fitpixmap_redLight);
        label_tab_grabToy_rbCtlStatusValue->setPixmap(fitpixmap_redLight);
    }

    if(rbQthread_rbImpMoudlePrepare->isRunning()){
        label_tabShakeHand_impStatusValue->setPixmap(fitpixmap_greenLight);
    } else{
        label_tabShakeHand_impStatusValue->setPixmap(fitpixmap_redLight);
    }

    if(rbQthread_rbVoiceMoudlePrepare->isRunning()){
        label_tabShakeHand_voiceStatusValue->setPixmap(fitpixmap_greenLight);
        label_tabgrabToy_voiceStatusValue->setPixmap(fitpixmap_greenLight);
    } else{
        label_tabShakeHand_voiceStatusValue->setPixmap(fitpixmap_redLight);
        label_tabgrabToy_voiceStatusValue->setPixmap(fitpixmap_redLight);
    }
    //刷新机器人控制模块状态
    if(rbQthread_rbCtlMoudlePrepare->isRunning()){
        flag_rbCtlStartUp=true;
    }else
    {
        flag_rbCtlStartUp=false;
    }
    // if(rbQthread_rbImpMoudlePrepare->isRunning()){
    //         mutex_devDetector.lock();
    //         impedenceConn_Detector.lifeNum=100;
    //         impedenceConn_Detector.status= true;
    //         mutex_devDetector.unlock();
    // }

    //机器人故障时下使能
    if(!Holdflag_RobDownEnable){
        hsr_rosi_device::SetEnableSrv srv2_setE;
        srv2_setE.request.enable=false;
        if(!RobErr_Detector.status){
            hsr_rosi_device::ClearFaultSrv srv_clearF;
            RobReset_client.call(srv_clearF);
            RobEnable_client.call(srv2_setE);
            Holdflag_RobDownEnable=true;
        }
    }

    //机器人状态发布
    hsr_rosi_device::setModeSrv srv;
    if(rbQthread_rbImpMoudlePrepare->isRunning())
    {
        if(!RobErr_Detector.status)
        {
            system((char*)"rosservice call /stop_motion");
            srv.request.mode=0;
            RobSetMode_client.call(srv);
        }
    }

    std_msgs::Bool rb_msg;
    if(RobErr_Detector.status){
        rb_msg.data= true;
        robStatusSend_publisher.publish(rb_msg);
    } else{
        rb_msg.data= false;
        robStatusSend_publisher.publish(rb_msg);
    }
    //机器人控制模块繁忙状态
    if(RobEnable_Detector.status&&flag_rbCtlBusy){
        label_tabmain_rbBusyStatusValue->setPixmap(fitpixmap_greenLight);
    }
    else{
        label_tabmain_rbBusyStatusValue->setPixmap(fitpixmap_redLight);
    }
    //发布阻抗模块运行状态
    std_msgs::Bool msg_imp;
    if(impedenceConn_Detector.status){
        msg_imp.data=true;
        impedenceLive_publisher.publish(msg_imp);
        flag_impedenceLive= true;
    } else{
        msg_imp.data= false;
        impedenceLive_publisher.publish(msg_imp);
        flag_impedenceLive= false;
    }


    if(RobConn_Detector.status)
    {
        if(!rbQthread_lisionRbErrInfo->isRunning()){
            rbQthread_lisionRbErrInfo->start();
        }
    }
}
//设备连接
void MainWindow::slot_btn_tabmain_devConn() {
    if(rbQthread_devConn->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复连接设备!"));
    } else{
        rbQthread_devConn->start();
        LOG("RUNINFO")->logInfoMessage("设备开始运行中");
    }
}
//开始连接子线程
void MainWindow::thread_rbQthread_devConn() {
    switch (cbox_tabmain_chooseMode->currentIndex()){
        case 0:
            emit emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //声音控制模式
            system("rosrun handrb_ui devConn.sh");
            //如果复位过程序,增需要重新启动本节点
//            if(flag_havedReset){ob_node.rebootUiNode();}
            break;
        case 2:
            //按钮控制模式
            system("rosrun handrb_ui devConn.sh");
            //如果复位过程序,增需要重新启动本节点
//            if(flag_havedReset){ob_node.rebootUiNode();}
            break;
    }
}
//开始运行
void MainWindow::slot_btn_tabmain_beginRun() {

    if(rbQthread_beginRun->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复运行设备!"));
    } else{
        rbQthread_beginRun->start();
        LOG("RUNINFO")->logInfoMessage("设备开始运行中");
    }
}

//开始运行子线程
void MainWindow::thread_rbQthread_beginRun() {
    hsr_rosi_device::ClearFaultSrv srv_clearF;
    hsr_rosi_device::SetEnableSrv srv2_setE;
    srv2_setE.request.enable=true;
    if(!RobErr_Detector.status){
        RobReset_client.call(srv_clearF);
    }
    if(!RobEnable_Detector.status){
        RobEnable_client.call(srv2_setE);
    }

    switch (cbox_tabmain_chooseMode->currentIndex()){
        case 0:
            emit emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //声音控制模式
            if(!stateController->isRunning){
                stateController->start();
                stateController->isStart= true;
                stateController->isStop= false;
                stateController->sub_isStop= false;


            } else{
                emit emitQmessageBox(infoLevel::information,QString("总控系统正在运行中，请不要重复运行!"));
            }
            break;
        case 2:
            //按钮控制模式
    //    system("rosrun handrb_ui rbCtlMoudle.sh");
            break;
    }
}

//系统停止
void MainWindow::slot_btn_tabmain_sysStop() {
    Timer_forAutoRunShakeHand->stop();
    stateController->setCloseVoice();
    if(rbQthread_sysStop->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复进行系统停止!"));
    } else{
        rbQthread_sysStop->start();
        LOG("RUNINFO")->logInfoMessage("设备停止");
    }
}

//系统停止子线程
void MainWindow::thread_rbQthread_sysStop() {
    system((char*)"rosservice call /stop_motion");
    system((char*)"rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" &");
    // system("rosservice call /set_mode_srv \"mode: 0\"");
    hsr_rosi_device::setModeSrv srv;
    srv.request.mode=0;
    if(RobSetMode_client.call(srv)){
        if(srv.response.finsh){
            emit emitQmessageBox(infoLevel::warning, "模式设置为点动模式!");
        }
    }
    system((char*)"rostopic pub -1 /stop_move std_msgs/Bool \"data: true\"");
    stateController->isStop= true;
}

//系统复位 goto 等5s定时重启
void MainWindow::slot_btn_tabmain_sysReset() {
//    Timer_forAutoRunShakeHand->stop();
//    if(rbQthread_sysReset->isRunning()){
//        emit emitQmessageBox(infoLevel::warning, QString("程序正在执行中,请不要重复启动!"));
//    } else{
//        rbQthread_sysReset->start();
//    }
        for(auto thread:rbQthreadList){
             if(thread->isRunning()){
                 thread->terminate();
                 cout<<"释放线程资源"<<endl;
             }
         }
            //开辟临时线程
        rbQthread* tmp_thread=new rbQthread();
        tmp_thread->setParm4([&]
        {
        Timer_forAutoRunShakeHand->stop();
//         flag_havedReset= true;
        system("rosrun openni2_tracker voice_shutdown.sh &");
        system("rosrun openni2_tracker vision_shutdown.sh &");
        system("rosnode kill $(rosnode list |grep -v handrb_ui)");
        
        }
        );
        //5s后复位按钮才能再次使用
        connect(tmp_thread,SIGNAL(started()),this,SLOT(slot_rbQthread_listenSysResetStart()));
        connect(tmp_thread,SIGNAL(finished()),tmp_thread,SLOT(deleteLater()));
        connect(tmp_thread,SIGNAL(finished()),this,SLOT(slot_rbQthread_listenFinish()));
        tmp_thread->start();
        LOG("RUNINFO")->logInfoMessage("系统复位");

        rbQthread_rbCtlMoudlePrepare->exit();
//    if (rbQthread_sysReset->isRunning()) {
//        emit emitQmessageBox(infoLevel::warning, QString("程序正在执行中,请不要重复启动!"));
//    } else
//    {
//        //掉机器人使能``
//        hsr_rosi_device::SetEnableSrv srv;
//        srv.request.enable= false;
//        RobEnable_client.call(srv);
//        //释放线程资源
//        // for(auto thread:rbQthreadList){
//        //     if(thread->isRunning()){
//        //         thread->terminate();
//        //         thread->wait(1000*1);
//        //         cout<<"释放线程资源"<<endl;
//        //     }
//        // }
}


//系统复位子线程
void MainWindow::thread_rbQthread_sysReset() {
    system("rosnode kill $(rosnode list |grep -v handrb_ui)");
    rbQthread_sysReset->terminate();
//    flag_havedReset= true;
//    ob_node.shutdownNode();
//    system("rosnode kill -a");
//    sleep(1);
//    system("killall rosmaster");
//    sleep(1);
//    system("roscore");
//    sleep(1);
}

void MainWindow::slot_btn_rbSetEnable() {
    flag_rbEnable=!flag_rbEnable;
    hsr_rosi_device::SetEnableSrv srv;
    if(flag_rbEnable){
        srv.request.enable= true;
        RobEnable_client.call(srv);
        btn_rbSetEnable->setText("机器人下使能");
        LOG("RUNINFO")->logInfoMessage("机器人下使能");
    } else{
        srv.request.enable= false;
        RobEnable_client.call(srv);
        btn_rbSetEnable->setText("机器人上使能");
        LOG("RUNINFO")->logInfoMessage("机器人上使能");

    }
}

void MainWindow::slot_btn_rbReset() {
    hsr_rosi_device::ClearFaultSrv srv;
    RobReset_client.call(srv);
}

void MainWindow::slot_btn_gripper_open() {
    if (rbQthread_handClaw_gesture->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_handClaw_gesture->setParm3(this,&MainWindow::thread_rbQthread_handClaw_gesture,"open");
        rbQthread_handClaw_gesture->start();
    }
}

void MainWindow::slot_btn_gripper_close() {
    if (rbQthread_handClaw_gesture->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_handClaw_gesture->setParm3(this,&MainWindow::thread_rbQthread_handClaw_gesture,"close");
        rbQthread_handClaw_gesture->start();
    }
}

void MainWindow::slot_btn_gripper_OK_Pose() {
    if (rbQthread_handClaw_gesture->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_handClaw_gesture->setParm3(this,&MainWindow::thread_rbQthread_handClaw_gesture,"ok");
        rbQthread_handClaw_gesture->start();
    }
}

void MainWindow::slot_btn_gripper_Y_Pose() {
    if (rbQthread_handClaw_gesture->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_handClaw_gesture->setParm3(this,&MainWindow::thread_rbQthread_handClaw_gesture,"y");
        rbQthread_handClaw_gesture->start();
    }
}

void MainWindow::slot_btn_rbGoHomePose() {
    std_srvs::SetBool srv;
    srv.request.data=true;
    if(backHomeClient.call(srv))
    {
        if(srv.response.success){
            ROS_INFO_STREAM("back home SUCCESS");
        }
    }
    else
    {
        ROS_INFO_STREAM("check back home server");
        return;
    }
    // system("rosservice call /back_home \"data: true\" ");
}

//void MainWindow::slot_btn_tabfunc_shakehand() {
//    cout<<"点击握手按钮"<<endl;
//    if(rbQthread_shakehand->isRunning()){
//        emit emitQmessageBox(infoLevel::warning ,"与人握手程序运行中,请不要重复启动!");
//    } else{
//        rbQthread_shakehand->start();
//    }
//}
//
//void MainWindow::slot_btn_tabfunc_grepwawa() {
//    if(rbQthread_grepwawa->isRunning()){
//        emit emitQmessageBox(infoLevel::warning ,"抓娃娃程序运行中,请不要重复启动!");
//    } else{
//        rbQthread_grepwawa->start();
//    }
//}

void MainWindow::callback_getShakeResult_subscriber(std_msgs::Int16 msg){
    std::cout << "get ShakeReult :"<< msg.data<<std::endl;
    int ret = msg.data;
    if(ret == 0)
    {     
        switch (cbox_tabmain_chooseMode->currentIndex())
            {
                case 1:
                    ctrlState.isEnd_shakeHand= true;
                    stateController->updateState(&ctrlState);
                    break;
                case 2:
                    slot_btn_tabShakeHand_shakeHandEnd();
                    break;
        
            }
        // slot_btn_tabShakeHand_shakeHandEnd();
//        impedenceLive_publisher.publish(false);
    }
}

void MainWindow::callback_objectCallBack(hirop_msgs::ObjectArray obj)
{
    grab_ok=false;
    hirop_msgs::ObjectInfo pose = obj.objects.at(0);
    geometry_msgs::PoseStamped pp2 = pose.pose;
    transformFrame(pp2, "world");
    retObj = pp2;
    std::cout << "<-- objectCallBack ShakeReult ---"<<std::endl;
    /*************************************************************************/
    pick_place_bridge::PickPlacePose srv;
    srv.request.Pose = retObj;
    std::cout << "<- objectCallBack ShakeReult Z "<<srv.request.Pose.pose.position.z<<std::endl;

    if(srv.request.Pose.pose.position.z < 1.1){
        
        cout<<"轨迹规划点位过低,请检测";
        ctrlState.grab_ok=false;
        ctrlState.err_PlanPose=true;
        stateController->updateState(&ctrlState); 
        return;
    }
        // srv.request.Pose.pose.position.z = 1.08;
    pickServer_client.call(srv);
    if(srv.response.result != true){
        std::cout << "planning pick error  ---"<<std::endl;
        ctrlState.grab_ok=false;
        stateController->updateState(&ctrlState);
        return ;
    }
    ROS_INFO_STREAM("****************************************************");
    /*******************************/
    srv.request.Pose.pose.position.x = 0.95;
    srv.request.Pose.pose.position.y = -0.45;
    srv.request.Pose.pose.position.z = 1.51;
    srv.request.Pose.pose.orientation.x = 0;
    srv.request.Pose.pose.orientation.y = 0;
    srv.request.Pose.pose.orientation.z = 0;
    srv.request.Pose.pose.orientation.w = 1;
    placeServer_client.call(srv);
    if(srv.response.result != true){
        std::cout << "place pick error  ---"<<std::endl;
        ctrlState.grab_ok=false;
        stateController->updateState(&ctrlState);
        return ;
    }
    else
    {
        cout<<"-------------------55555555抓取完成--555555555--------------------"<<endl;
        ctrlState.grab_ok=true;
        stateController->updateState(&ctrlState);
    }
}

void MainWindow::slot_btn_tabrecord_outRecord() {
    QString file_path = QFileDialog::getOpenFileName(this,"选择文件",logPath, "Files(*.log)");
    QString displayString;
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
//        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        displayString.append(str);
    }
    file.close();
    plainText_tabrecord_1->clear();
    plainText_tabrecord_1->setPlainText(displayString);
}

void MainWindow::slot_btn_tabrecord_clearRecord() {
    plainText_tabrecord_1->clear();
}

void MainWindow::slot_combox_chooseMode_Clicked(int index) {
    switch (index) {
        case 0:
            btn_tabmain_beginRun->setEnabled(false);
            btn_tabmain_sysStop->setEnabled(false);
            btn_tabmain_sysReset->setEnabled(false);
            btn_tabmain_devConn->setEnabled(false);
            break;
        case 1:
            btn_tabmain_beginRun->setEnabled(true);
            btn_tabmain_sysStop->setEnabled(true);
            btn_tabmain_sysReset->setEnabled(true);
            btn_tabmain_devConn->setEnabled(true);
            break;
        case 2:
            btn_tabmain_beginRun->setEnabled(true);
            btn_tabmain_sysStop->setEnabled(true);
            btn_tabmain_sysReset->setEnabled(true);
            btn_tabmain_devConn->setEnabled(true);
            break;
    }
}


void MainWindow::callback_voiceSolveRes_subcriber(const std_msgs::Int16::ConstPtr& msg) {
//    label_tabfunc_voiceValue->setText(QString("当前语音识别结果:")+QString::fromStdString(msg.data));
    std_msgs::String se_msg;
    ctrlState.voice_order= msg->data;
    stateController->updateState(&ctrlState);
    cout<<"收到指令:"<<msg->data<<endl;

//    string voice_feedback;
//    int voice_order = msg->data;
//    //收到上使能语音指令
//    if (voice_order == 0)
//    {
//        if (RobConn_Detector.status == true)
//        {
//            //调用上使能按钮
//            voice_feedback = "机器人上使能成功";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//        else
//        {
//            voice_feedback = "机器人上使能失败，请重启设备连接";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//
//    }
//
//    //收到下使能语音指令
//    if (voice_order == 1)
//    {
//        if (RobConn_Detector.status == true)
//        {
//            //调用下使能按钮
//            voice_feedback = "机器人下使能成功";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//        else
//        {
//            voice_feedback = "机器人下使能失败，请重启设备连接";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//    }
//
//    //收到握手语音指令
//    if (voice_order == 2)
//    {
//        if ((flag_rbEnable&&flag_rbCtlStartUp)== true)
//        {
//            //调用握手按钮
//            voice_feedback = "再见，祝你生活愉快";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//        else
//        {
//            voice_feedback = "抱歉，机器人此时无法进行握手操作";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//    }
//
//    //收到抓娃娃语音指令
//    if (voice_order == 3)
//    {
//        if ((flag_rbEnable&&flag_rbCtlStartUp) == true)
//        {
//            //调用握手按钮
//            voice_feedback = "给，你要的娃娃，祝你生活愉快";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//        else
//        {
//            voice_feedback = "抱歉，机器人此时无法进行抓娃娃操作";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//    }
//
//    //收到回原点语音指令
//    if (voice_order == 6)
//    {
//        if ((flag_rbEnable&&flag_rbCtlStartUp) == true)
//        {
//            //调用回原点按钮
//            voice_feedback = "机器人已回原点";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//        else
//        {
//            voice_feedback = "抱歉，机器人状态出现故障";
//            se_msg.data = voice_feedback.c_str();
//            voice_order_publisher.publish(se_msg);
//        }
//    }

}
//
void MainWindow::callback_personDetectRes_subcriber(const sensor_msgs::Image::ConstPtr& msg) {
    //如果标志为关闭行人检测
    // if(!flag_switchPersonDecBtnText){
    //     return;
    // }
    const cv_bridge::CvImageConstPtr &ptr = cv_bridge::toCvShare(msg, "bgr8");
    cv::Mat mat = ptr->image;
    QImage qimage = cvMat2QImage(mat);
    QPixmap tmp_pixmap = QPixmap::fromImage(qimage);
    QPixmap new_pixmap = tmp_pixmap.scaled(label_tab_voiceMonitor_2->width(), label_tab_voiceMonitor_2->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
//    QPixmap tmp_pixmap = pixmap1.scaled(label_picture1->width(), label_picture1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    label_tab_personDetect_showImag->setPixmap(new_pixmap);
    label_tab_voiceMonitor_2->setPixmap(new_pixmap);
}
//
void MainWindow::callback_grabDollImagRes_subcriber(const sensor_msgs::Image::ConstPtr& msg) {
    const cv_bridge::CvImageConstPtr &ptr = cv_bridge::toCvShare(msg, "bgr8");
    cv::Mat mat = ptr->image;
    QImage qimage = cvMat2QImage(mat);
    QPixmap tmp_pixmap = QPixmap::fromImage(qimage);
    QPixmap new_pixmap = tmp_pixmap.scaled(lable_tabgrabToy_showImg->width(), lable_tabgrabToy_showImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
//    QPixmap tmp_pixmap = pixmap1.scaled(label_picture1->width(), label_picture1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    lable_tabgrabToy_showImg->setPixmap(new_pixmap);
}

void MainWindow::callback_robStatus_subscriber(const industrial_msgs::RobotStatus::ConstPtr robot_status) {

    mutex_devDetector.lock();
    RobConn_Detector.lifeNum=100;
    RobConn_Detector.status= true;
    if(robot_status->in_error.val==0){
        RobErr_Detector.lifeNum=100;
        RobErr_Detector.status= true;

        ctrlState.RobNormalState= true;
    } else{
        RobErr_Detector.status= false;

        ctrlState.RobNormalState= false;
    }
    if(robot_status->drives_powered.val==1){
        RobEnable_Detector.lifeNum=100;
        RobEnable_Detector.status= true;

        ctrlState.RobEnableState= true;
    } else{
        RobEnable_Detector.status= false;
        ctrlState.RobEnableState= false;
    }
    stateController->updateState(&ctrlState);
    mutex_devDetector.unlock();
}

void MainWindow::callback_camera_subscriber(const sensor_msgs::Image::ConstPtr& msg){
    mutex_devDetector.lock();
    cameraConn_Detector.lifeNum=100;
    cameraConn_Detector.status= true;
    mutex_devDetector.unlock();

    //显示图片
//    const cv_bridge::CvImageConstPtr &ptr = cv_bridge::toCvShare(msg, "bgr8");
//    cv::Mat mat = ptr->image;
//    QImage qimage = cvMat2QImage(mat);
//    QPixmap tmp_pixmap = QPixmap::fromImage(qimage);
//    QPixmap new_pixmap = tmp_pixmap.scaled(label_preImag->width(), label_preImag->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
//    QPixmap tmp_pixmap = pixmap1.scaled(label_picture1->width(), label_picture1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
//    label_preImag->setPixmap(new_pixmap);
}

void MainWindow::callback_forceSensor_subscriber(geometry_msgs::Wrench msg) {
    mutex_devDetector.lock();
    forceSensorConn_Detector.lifeNum=100;
    forceSensorConn_Detector.status= true;
    mutex_devDetector.unlock();
}

QImage MainWindow::cvMat2QImage(const cv::Mat &mat) {
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
//        image.setNumColors(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
        // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}
void MainWindow::displayTextControl(QString text) {
    plainText_tabrecord_1->appendPlainText(text);
}

void MainWindow::showQmessageBox(infoLevel level, QString info) {
    switch (level){
        case infoLevel ::information:
            QMessageBox::information(this,"提示",info,QMessageBox::Ok);break;
        case infoLevel ::warning:
            QMessageBox::warning(this,"警告",info,QMessageBox::Ok);break;
    }
}

void MainWindow::showLightColor(QLabel *label, string color) {
    if(color=="red"){
        label->setPixmap(fitpixmap_redLight);
    } else if(color=="green"){
        label->setPixmap(fitpixmap_greenLight);
    }
}


//void MainWindow::slot_btn_tabfunc_persondeteck() {
//    mutex_devDetector.lock();
//    cameraConn_Detector.lifeNum=100;
//    cameraConn_Detector.status= true;
//    mutex_devDetector.unlock();
//    if(!flag_switchPersonDecBtnText){
//        if(rbQthread_persondeteck->isRunning()){
//            emit emitQmessageBox(infoLevel::information,QString("行人检测运行中"));
//        } else{
//            rbQthread_persondeteck->start();
//        }
//        btn_tabfunc_persondeteck->setText("关闭行人检测");
//    } else{
//        //关闭行人检测
//        std_msgs::Bool msg;
//        msg.data= false;
//        visionDetech_publisher.publish(msg);
//        btn_tabfunc_persondeteck->setText("打开行人检测");
//    }
//    flag_switchPersonDecBtnText=!flag_switchPersonDecBtnText;
//
//}

void MainWindow::thread_rbQthread_persondeteck() {
    system((char*)"rosrun openni2_tracker peopledetection.sh");
}

void MainWindow::thread_rbQthread_shakehand() {
    // emit emitQmessageBox(infoLevel::information,"与人握手信号发出");
    rb_msgAndSrv::rb_DoubleBool srv;
    srv.request.request= true;

    if(handClaw_shakeHand_client.call(srv)){
        if(srv.response.respond){
             emit emitQmessageBox(infoLevel::information ,"机器人握手姿势准备好!");
        } else{
             emit emitQmessageBox(infoLevel::warning ,"机器人握手姿势运动出错!");
        }
    } else{
         emit emitQmessageBox(infoLevel::warning ,"与人握手服务端连接失败!");
    }
}

void MainWindow::thread_rbQthread_grepwawa() {
    emit emitQmessageBox(infoLevel::information,"抓娃娃信号发出");
    rb_msgAndSrv::rb_DoubleBool srv;
    srv.request.request= true;
    if(handClaw_grabDoll_client.call(srv)){
    } else{
        emit emitQmessageBox(infoLevel::warning ,"抓娃娃服务端连接失败!");
    }
}

void MainWindow::thread_rbQthread_handClaw_gesture(string gesture) {
    rb_msgAndSrv::rb_string srv;
    srv.request.data.data=gesture;
    if(handClaw_gesture_client.call(srv)){
    } else{
        emit emitQmessageBox(infoLevel::warning ,"手势功能服务端连接失败!");
    }

}

//系统复位线程启动
void MainWindow::slot_rbQthread_listenSysResetStart() {
    btn_tabmain_sysReset->setEnabled(false);
    Timer_listen_SysResetThread->start();
}

void MainWindow::slot_rbQthread_listenFinish() {
    cout<<"线程资源释放"<<endl;
}

void MainWindow::slot_timer_listen_SysResetThread() {
    btn_tabmain_sysReset->setEnabled(true);
    Timer_listen_SysResetThread->stop();
}

void MainWindow::slot_timer_listenSysErrThread() {

}

void MainWindow::slot_runTimer(QTimer *timer) {
    timer->start();
}

//void MainWindow::slot_btn_tabfunc_voiceDetect() {
//    cout<<"点击了按钮 "<<endl;
//    if(rbQthread_voicedeteck->isRunning()){
//        emit emitQmessageBox(infoLevel::warning,QString("语音程序正在运行中"));
//    } else{
//        rbQthread_voicedeteck->start();
//    }
//}

void MainWindow::thread_rbQthread_voicedeteck() {
    system((char*)"rosrun openni2_tracker voice.sh");
}

void MainWindow::slot_btn_tabShakeHand_startRobRun() {
    cout<<"上使能"<<endl;
    if (rbQthread_rbRunMoudlePrepare->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "机器人系统程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_rbRunMoudlePrepare->start();
    }
}

void MainWindow::slot_btn_tabShakeHand_startRobCtl() {
    if (rbQthread_rbCtlMoudlePrepare->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "机器人控制模块程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_rbCtlMoudlePrepare->start();
    }
}

void MainWindow::slot_btn_tabShakeHand_startimpedence() {
    if (rbQthread_rbImpMoudlePrepare->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "阻抗模块程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_rbImpMoudlePrepare->start();
    }
    Holdflag_RobSetMode= false;
}

void MainWindow::slot_btn_tabShakeHand_startvoice() {
    if (rbQthread_rbVoiceMoudlePrepare->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "语音模块程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_rbVoiceMoudlePrepare->start();
    }
}

void MainWindow::slot_btn_tabShakeHand_begin() {
    bool flag = sendSignal_RbPreparePose();
    if(flag){
        emit emitQmessageBox(infoLevel::information,"握手抬起指令发出");
    }

}

void MainWindow::slot_btn_tabShakeHand_stop() {
    if (rbQthread_sysStop->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "停止正在执行中,请不要重复启动!");
    } else {
        rbQthread_sysStop->start();
    }
    Timer_forAutoRunShakeHand->stop();
}

void MainWindow::slot_btn_tabShakeHand_close() {
    Timer_forAutoRunShakeHand->stop();
    if(rbQthread_rbImpMoudlePrepare->isRunning()){
        system((char*)"rosservice call /stop_motion");
        system((char*)"rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" &");
    }
    hsr_rosi_device::setModeSrv srv;
    srv.request.mode=0;
    if(RobSetMode_client.call(srv)){
        if(srv.response.finsh){
            emit emitQmessageBox(infoLevel::warning, "模式设置为点动模式!");
        }
    }else
    {
        emit emitQmessageBox(infoLevel::warning, "模式设置服务连接失败!");
    }
}


void MainWindow::slot_btn_tabgrabToy_startRobRun() {
    if (rbQthread_rbRunMoudlePrepare->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "机器人系统程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_rbRunMoudlePrepare->start();
    }
}

void MainWindow::slot_btn_tabgrabToy_startRobCtl() {
    if (rbQthread_rbCtlMoudlePrepare->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "机器人控制模块程序正在执行中,请不要重复启动!");
    } else {
        rbQthread_rbCtlMoudlePrepare->start();
    }
}

void MainWindow::slot_btn_tabgrabToy_startvoice() {
    //
    if(flag_switchimpedenceText)
    {
        if (rbQthread_rbImpMoudlePrepare->isRunning())
        {
            emit emitQmessageBox(infoLevel::warning, "阻抗模块程序正在执行中,请不要重复启动!");
        }
        else
        {
            rbQthread_rbImpMoudlePrepare->start();
            //切换标签
            flag_switchimpedenceText=!flag_switchimpedenceText;
            btn_tabShakeHand_startimpedence->setText("关闭");
        }
    }
    else
    {
        if (rbQthread_rbImpMoudlePrepare->isRunning())
        {
            //关闭阻抗
            system((char*)"rosservice call /stop_motion");
            system((char*)"rosnode kill /hsr_impedance");
            btn_tabShakeHand_startimpedence->setText("开启");
        }
    }

}

void MainWindow::slot_btn_tabgrabToy_run() {
    bool flag= sendSignal_RbGrabtoy();
    if(flag){
        emit emitQmessageBox(infoLevel::warning, "抓娃娃指令已经发出!");
    }
}

void MainWindow::slot_btn_tabgrabToy_stop() {
//    if (rbQthread_sysStop->isRunning()) {
//        emit emitQmessageBox(infoLevel::warning, "停止正在执行中,请不要重复启动!");
//    } else {
//        rbQthread_sysStop->start();
//    }

    //回原点
    std_srvs::SetBool srv;
    srv.request.data=true;
    if(backHomeClient.call(srv))
    {
        if(srv.response.success){
            ROS_INFO_STREAM("back home SUCCESS");
        }
    }
    else
    {
        ROS_INFO_STREAM("check back home server");
        return;
    }
    //检测点
    rb_msgAndSrv::rb_DoubleBool  dSrv;
    dSrv.request.request = true;
    if(!detectePointClient.call(dSrv))
    {
        ROS_INFO_STREAM("check handClaw_detectDoll server");
    }
    else
    {
        if(dSrv.response.respond)
        {
            ROS_INFO_STREAM("move to detete point SUCCESS");
        }
        else
        {
            ROS_INFO("move to detect point FAILURE");
            return ;
        }
    }
}

void MainWindow::slot_btn_tabgrabToy_close() {
    industrial_msgs::StopMotion srv;
    if(stopMotionClient.call(srv))
    {
        ROS_ERROR_STREAM("check stop_motion");
        return ;
    }
    else
    {
        if(srv.response.code.val == srv.response.code.SUCCESS)
        {
            ROS_ERROR_STREAM("stop motion SUCCESS");
        }
        else
        {
            ROS_ERROR_STREAM("stop motion FAILURE");
        }
    }
}

void MainWindow::thread_rbQthread_rbRunMoudlePrepare() {
//    system("rosrun handrb_ui rbRunMoudle.sh");
     hsr_rosi_device::ClearFaultSrv srv_clearF;
    hsr_rosi_device::SetEnableSrv srv2_setE;
    srv2_setE.request.enable=true;
    if(!RobErr_Detector.status){
        RobReset_client.call(srv_clearF);
    }
    if(!RobEnable_Detector.status){
        RobEnable_client.call(srv2_setE);
    }
}

void MainWindow::thread_rbQthread_rbCtlMoudlePrepare() {
    system((char*)"rosrun handrb_ui rbCtlMoudle.sh");
}

void MainWindow::thread_rbQthread_rbImpMoudlePrepare() {
    hsr_rosi_device::setModeSrv srv;
    srv.request.mode=1;
    if(RobSetMode_client.call(srv)){
        if(srv.response.finsh){
            emit emitQmessageBox(infoLevel::warning, "模式设置为随动模式!");
        }
    }else
    {
        emit emitQmessageBox(infoLevel::warning, "模式设置服务连接失败!");
    }    
    system((char*)"rosrun handrb_ui rbImpMoudle.sh");
}

void MainWindow::thread_rbQthread_rbVoiceMoudlePrepare() {
    system((char*)"rosrun handrb_ui rbVoiceMoudle.sh");
    sleep(2);
    sayHi_awakeVoice();
}

void MainWindow::slot_cBox_tabShakeHand_setMode(int index) {
 switch (index){
     case 0:
         label_tabShakeHand_voiceStatus->setVisible(false);
         label_tabShakeHand_voiceStatusValue->setVisible(false);
         btn_tabShakeHand_startvoice->setVisible(false);
         btn_tabShakeHand_begin->setVisible(true);
         break;
     case 1:
         label_tabShakeHand_voiceStatus->setVisible(true);
         label_tabShakeHand_voiceStatusValue->setVisible(true);
         btn_tabShakeHand_startvoice->setVisible(true);
         btn_tabShakeHand_begin->setVisible(false);
         break;

 }
}

void MainWindow::callback_impedenceLive_subscriber(sensor_msgs::JointState msg) {
    mutex_devDetector.lock();
    impedenceConn_Detector.lifeNum=100;
    impedenceConn_Detector.status= true;
    mutex_devDetector.unlock();
}

void MainWindow::callback_rbCtlBusy_status_subscriber(std_msgs::Bool msg) {
    //机器人控制模块运动中
    if(msg.data){
        flag_rbCtlBusy= true;
        ctrlState.flag_rbCtlBusy= true;
    }
     else{
        flag_rbCtlBusy= false;
        ctrlState.flag_rbCtlBusy= false;
    }
    stateController->updateState(&ctrlState);
}

bool MainWindow::sendSignal_RbPreparePose() {
    //启动startmove
    system((char*)"rostopic pub -d 1 /start_move std_msgs/Bool \"data: true\"");
    if(!RobEnable_Detector.status){
        emit emitQmessageBox(infoLevel::warning, "机器人未上使能!");
        return false;
    }
    if(!flag_rbCtlStartUp){
        emit emitQmessageBox(infoLevel::warning, "机器人控制模块未启动!");
        return false;
    }
    if(flag_rbCtlBusy){
        emit emitQmessageBox(infoLevel::warning, "机器人不在空闲中!");
        return false;
    }
    if (rbQthread_shakehand->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "握手正在执行中,请不要重复启动!");
    } else {
        rbQthread_shakehand->start();
    }
    return true;

}

bool MainWindow::sendSignal_RbGrabtoy() {
    if(!RobEnable_Detector.status){
        emit emitQmessageBox(infoLevel::warning, "机器人未上使能!");
        return false;
    }
    if(!flag_rbCtlStartUp){
        emit emitQmessageBox(infoLevel::warning, "机器人控制模块未启动!");
        return false;
    }
    if(flag_rbCtlBusy){
        emit emitQmessageBox(infoLevel::warning, "机器人不在空闲中!");
        return false;
    }
    if (rbQthread_grepwawa->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, "停止正在执行中,请不要重复启动!");
    } else {
        rbQthread_grepwawa->start();
    }
    return true;
}

void MainWindow::thread_rbQthread_LisionRbErrInfo() {
    hirop_msgs::robotError srv;
    robGetStatus_client.call(srv);
    uint64_t level=srv.response.errorLevel;
    int errorLevel=level;
    string errorMsg=srv.response.errorMsg;
    string isError=srv.response.isError?"true":"false";
    string dealMsg=srv.response.dealMsg;
    QString tmp=QString("errorLevel:%1\nerrorMsg:%2\nisError:%3\ndealMsg:%4").arg(errorLevel).arg(QString().fromStdString(errorMsg)).arg(QString().fromStdString(isError)).arg(QString().fromStdString(dealMsg));
    if(srv.response.isError){
        emit emitQmessageBox(infoLevel::information,tmp);
    }
}
/**
 * 任务停止
 * 
 */
void MainWindow::slot_btn_tabShakeHand_shakeHandEnd()
{
//    if(rbQthread_rbImpMoudlePrepare->isRunning()){
    switch (cbox_tabmain_chooseMode->currentIndex())
    {
        case 0:
            emit emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //自动控制模式
            ctrlState.isEnd_shakeHand= true;
            stateController->updateState(&ctrlState);
            break;
        case 2:
            //按钮控制模式
            hsr_rosi_device::setModeSrv srv;
            srv.request.mode=0;
            RobSetMode_client.call(srv);

            if(rbQthread_rbImpMoudlePrepare->isRunning()){
                cout<<"执行停止"<<endl;
                system((char*)"rosservice call /stop_motion");
                system((char*)"rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" ");
            }
            // impedenceLive_publisher.publish(false);
            // sleep(1);
            // sleep(2);
            cout<<"-----------------------发出握手结束信号--------------------------"<<endl;
            std_msgs::Bool msg;
            msg.data=true;
            shakehandOver_publisher.publish(msg);
            break;
    }

}

void MainWindow::slot_btn_tab_voiceDetect_run() {
    //运行语音控制脚本
    flag_switchVoiceBtnText=!flag_switchVoiceBtnText;
    if(flag_switchVoiceBtnText){
        if(rbQthread_rbVoiceMoudlePrepare->isRunning()){
            lable_tab_voiceDetect_showImg->setPixmap(pixmap_voiceAwake);
        } else{
            rbQthread_rbVoiceMoudlePrepare->start();
            lable_tab_voiceDetect_showImg->setPixmap(pixmap_voiceAwake);
        }
        btn_tab_voiceDetect_run->setText("关闭");
    } else{
        if(rbQthread_rbVoiceMoudlePrepare->isRunning()){
            //关闭语音
            system((char*)"rosrun openni2_tracker voice_shutdown");
            lable_tab_voiceDetect_showImg->setPixmap(pixmap_voicesleep);
        }
        btn_tab_voiceDetect_run->setText("激活");
    }
}

void MainWindow::sayHi_awakeVoice() {
    //启动语音助手
    string voice_assistant_setup = "大家好，我是华数智能机器人001，很开心为你们服务";
    std_msgs::String setup_msg;
    setup_msg.data = voice_assistant_setup.c_str();
    voice_order_publisher.publish(setup_msg);

}

//抓娃娃一键执行
void MainWindow::AutoRun_GrabToy() {
    //1.启动机器人控制模块

    //2.执行抓娃娃命令

}

//握手功能一键执行
void MainWindow::AutoRun_shakeHand() {
    switch (index_AutoRunShakeHand){
        case 0:
            if((!RobEnable_Detector.status)&&(!rbQthread_rbRunMoudlePrepare->isRunning())){
                slot_btn_tabShakeHand_startRobRun();//上使能
            }
            if(RobEnable_Detector.status){
                index_AutoRunShakeHand=1;
            }
            break;
        case 1:
            if(!rbQthread_shakehand->isRunning()){
                slot_btn_tabShakeHand_begin();//执行抬手动作
            }
            if(flag_robPreparePose){
                index_AutoRunShakeHand=2;
            }
            break;
        case 2:
            if(!rbQthread_rbImpMoudlePrepare->isRunning()){
                slot_btn_tabShakeHand_startimpedence();//启动阻抗随动模式
            } else{
                Timer_forAutoRunShakeHand->stop();
                emit emitQmessageBox(infoLevel::information,"一键启动握功能完成");
                index_AutoRunShakeHand=0;
            }
            break;
    }



}

//行人检测按钮
void MainWindow::slot_btn_tab_personDetect_openPersonDetect() {

}

//定时刷新执行握手任务
void MainWindow::slot_timer_AutoRun_shakeHand() {
    AutoRun_shakeHand();
}

//按钮启动定时器
void MainWindow::slot_btn_tabShakeHand_AutoRun() {
    if(!Timer_forAutoRunShakeHand->isActive()){
        Timer_forAutoRunShakeHand->start();
    }
}

void MainWindow::callback_isOpenFollow_subscriber(std_msgs::Bool msg) {
    flag_robPreparePose=msg.data;
    ctrlState.isOk_robPreparePose=flag_robPreparePose;
    stateController->updateState(&ctrlState);
}

void MainWindow::slot_btn_startSensor() {
    system("rosrun gripper_bridge opensensor.sh  &");
}

void MainWindow::slot_btn_tabgrabToy_grayDectectObj()
{
    hirop_msgs::detection d;
    d.request.detectorName = "Yolo6d";
    d.request.detectorType = 1;
    d.request.objectName = "toy1";
    if(!detectionClient.call(d)){

        return;
    }
}

//重启UI节点
void observer_rebootUiNode::rebootUiNode(){
    sp=new ros::AsyncSpinner(1);
    sp->start();
    ros::start();
    mainwindow->initRosToptic();

}


bool MainWindow::transformFrame(geometry_msgs::PoseStamped& poseStamped, std::string frame_id)
{

    geometry_msgs::PoseStamped* worldFramePose = new geometry_msgs::PoseStamped[1];
    geometry_msgs::PoseStamped* otherFramePose = new geometry_msgs::PoseStamped[1];
    tf::TransformListener listener;

    otherFramePose[0] = poseStamped;
    for(int i=0; i < 5; ++i)
    {
        try
        {
            listener.transformPose(frame_id, otherFramePose[0], worldFramePose[0]);
            break;
        }
        catch(tf::TransformException& ex)
        {
            ROS_INFO_STREAM(ex.what());
            ros::WallDuration(1).sleep();
            continue;
        }
    }
    poseStamped = worldFramePose[0];
    poseStamped.pose.orientation.x = 0;
    poseStamped.pose.orientation.y = 0;
    poseStamped.pose.orientation.z = 0;
    poseStamped.pose.orientation.w = 1;
    delete[] worldFramePose;
    delete[] otherFramePose;
    double add[3] = {0};
    Node->getParam("/grasp_place/position_x_add", add[0]);
    Node->getParam("/grasp_place/position_y_add", add[1]);
    Node->getParam("/grasp_place/position_z_add", add[2]);

    poseStamped.pose.position.x += add[0];
    poseStamped.pose.position.y += add[1];
    poseStamped.pose.position.z += add[2];
    if(poseStamped.header.frame_id == "world")
    {
        return true;
    }
    else
    {
        return false;
    }
}

