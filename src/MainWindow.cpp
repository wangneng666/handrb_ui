#include "MainWindow.h"

MainWindow::MainWindow(ros::NodeHandle *node, QWidget *parent):BaseWindow(node,parent){
    //系统变量初始化
    SysVarInit();
    //初始化rostopic
    initRosToptic();
    //信号与槽绑定
    signalAndSlot();
}

MainWindow::~MainWindow() {
    system("rosnode kill -a");
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

    //定时器初始化
    Timer_listen_status = new QTimer(this);
    Timer_listen_status->setInterval(100);
    connect(Timer_listen_status, &QTimer::timeout, this, &MainWindow::slot_timer_listen_status);
    Timer_listen_status->start();

    //线程初始化
    rbQthread_devConnOrRviz = new rbQthread();
    rbQthread_devConnOrRviz->setParm(this,&MainWindow::thread_rbQthread_devConnOrRviz);
    rbQthread_beginRun = new rbQthread();
    rbQthread_beginRun->setParm(this,&MainWindow::thread_rbQthread_beginRun);
    rbQthread_sysStop = new rbQthread();
    rbQthread_sysStop->setParm(this,&MainWindow::thread_rbQthread_sysStop);
    rbQthread_sysReset = new rbQthread();
    rbQthread_sysReset->setParm(this,&MainWindow::thread_rbQthread_sysReset);
    rbQthreadList.push_back(rbQthread_devConnOrRviz);
    rbQthreadList.push_back(rbQthread_beginRun);
    rbQthreadList.push_back(rbQthread_sysStop);
}

void MainWindow::initRosToptic(){
    //话题
    camera_subscriber=Node->subscribe<sensor_msgs::Image>("/usb_cam/image_raw",1,boost::bind(&MainWindow::callback_camera_subscriber, this, _1));
//    camera_subscriber=Node->subscribe<sensor_msgs::Image>("/usb_cam/image_raw",1,boost::bind(&MainWindow::callback_camera_subscriber, this, _1));
    forceSensor_subscriber=Node->subscribe<geometry_msgs::Wrench>("daq_data", 1000, &MainWindow::callback_forceSensor_subscriber, this);
    voiceSolveRes_subcriber=Node->subscribe<std_msgs::String>("voiceSolve_res",1,&MainWindow::callback_voiceSolveRes_subcriber, this);
    personDetectRes_subcriber=Node->subscribe<sensor_msgs::Image>("videphoto_feedback",1,boost::bind(&MainWindow::callback_personDetectRes_subcriber, this, _1));
    grabDollImagRes_subcriber=Node->subscribe<sensor_msgs::Image>("DollDetection_image",1,boost::bind(&MainWindow::callback_grabDollImagRes_subcriber, this, _1));
    robStatus_subscriber=Node->subscribe<industrial_msgs::RobotStatus>("/UR51/robot_status",1,boost::bind(&MainWindow::callback_robStatus_subscriber,this,_1));

    //服务
    RobReset_client = Node->serviceClient<hsr_rosi_device::ClearFaultSrv>("/UR51/clear_robot_fault");
    RobEnable_client = Node->serviceClient<hsr_rosi_device::SetEnableSrv>("/UR51/set_robot_enable");
    handClaw_gesture_client = Node->serviceClient<rb_msgAndSrv::rb_string>("handClaw_gesture");
    handClaw_shakeHand_client = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("handClaw_shakeHand");
    handClaw_grabDoll_client = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("handClaw_grabDoll");
    rob_goHome_client = Node->serviceClient<rb_msgAndSrv::rb_DoubleBool>("rob_goHome");
}

void MainWindow::signalAndSlot() {
/*********************************控件与槽函数绑定*************************************************/
    //主界面
    connect(btn_tabmain_devConnOrRviz,&QPushButton::clicked,this,&MainWindow::slot_btn_tabmain_devConnOrRviz);
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
    //功能界面
    connect(btn_tabfunc_shakehand,&QPushButton::clicked,this,&MainWindow::slot_btn_tabfunc_shakehand);
    connect(btn_tabfunc_grepwawa,&QPushButton::clicked,this,&MainWindow::slot_btn_tabfunc_grepwawa);
    connect(btn_tabfunc_persondeteck,&QPushButton::clicked,this,&MainWindow::slot_btn_tabfunc_persondeteck);
    //日志界面
    connect(btn_tabrecord_outRecord,&QPushButton::clicked,this,&MainWindow::slot_btn_tabrecord_outRecord);
    connect(btn_tabrecord_clearRecord,&QPushButton::clicked,this,&MainWindow::slot_btn_tabrecord_clearRecord);
/*************************************************************************************************/

/*********************************自定义信号与槽函数绑定*************************************************/
    connect(this, &MainWindow::emitLightColor,this, &MainWindow::showLightColor);
//    connect(rbQthread_devConnOrRviz, SIGNAL(signal_SendMsgBox(infoLevel ,QString)), this,SLOT(thread_slot_devConnOrRviz()));

/*************************************************************************************************/
}

void MainWindow::displayTextControl(QString text) {

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

//设备连接
void MainWindow::slot_btn_tabmain_devConnOrRviz() {
    cout<<"点击设备连接按钮"<<endl;
    if(rbQthread_devConnOrRviz->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复连接设备!"));
    } else{
        rbQthread_devConnOrRviz->start();
    }
}

//开始运行
void MainWindow::slot_btn_tabmain_beginRun() {
    if(rbQthread_beginRun->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复运行设备!"));
    } else{
        rbQthread_beginRun->start();
    }
}

//系统停止
void MainWindow::slot_btn_tabmain_sysStop() {
    if(rbQthread_sysStop->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复进行系统停止!"));
    } else{
        rbQthread_sysStop->start();
    }
}
//系统复位
void MainWindow::slot_btn_tabmain_sysReset() {
    for(auto thread:rbQthreadList){
        if(thread->isRunning()){
            thread->terminate();
        }
    }
    rbQthread_sysReset->start();
}

//设备连接子线程
void MainWindow::thread_rbQthread_devConnOrRviz() {
    switch (cbox_tabmain_chooseMode->currentIndex()){
        case 0:
            emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //启动真机连接launch文件

            break;
        case 2:
            //启动rviz文件
            break;
    }
}
//开始运行子线程
void MainWindow::thread_rbQthread_beginRun() {
    switch (cbox_tabmain_chooseMode->currentIndex()){
        case 0:
            emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //启动真机运行launch文件
            system("roslaunch hsr_handrobot handRobotGrab.launch");
            break;
        case 2:
            //启动rviz文件运行文件
            break;
    }
}
//系统停止子线程
void MainWindow::thread_rbQthread_sysStop() {

}
//系统复位子线程
void MainWindow::thread_rbQthread_sysReset() {

}

void MainWindow::slot_btn_rbSetEnable() {
//    LOG("ERRINFO")->logWarnMessage("右机器人报警");
}

void MainWindow::slot_btn_rbReset() {

}

void MainWindow::slot_btn_gripper_open() {

}

void MainWindow::slot_btn_gripper_close() {

}

void MainWindow::slot_btn_gripper_OK_Pose() {

}

void MainWindow::slot_btn_gripper_Y_Pose() {

}

void MainWindow::slot_btn_rbGoHomePose() {

}

void MainWindow::slot_btn_tabfunc_shakehand() {

}

void MainWindow::slot_btn_tabfunc_grepwawa() {

}

void MainWindow::slot_btn_tabrecord_outRecord() {

}

void MainWindow::slot_btn_tabrecord_clearRecord() {

}

void MainWindow::slot_combox_chooseMode_Clicked(int index) {
    index_sysMode = index;
    switch (index_sysMode) {
        case 0:
            btn_tabmain_devConnOrRviz->setEnabled(false);
            btn_tabmain_beginRun->setEnabled(false);
            btn_tabmain_sysStop->setEnabled(false);
            btn_tabmain_sysReset->setEnabled(false);
            break;
        case 1:
            btn_tabmain_devConnOrRviz->setEnabled(true);
            btn_tabmain_beginRun->setEnabled(true);
            btn_tabmain_sysStop->setEnabled(true);
            btn_tabmain_sysReset->setEnabled(true);
            btn_tabmain_devConnOrRviz->setText("设备连接");
            break;
        case 2:
            btn_tabmain_devConnOrRviz->setEnabled(true);
            btn_tabmain_beginRun->setEnabled(true);
            btn_tabmain_sysStop->setEnabled(true);
            btn_tabmain_sysReset->setEnabled(true);
            btn_tabmain_devConnOrRviz->setText("启动rviz");
            break;
    }
}



void MainWindow::callback_voiceSolveRes_subcriber(std_msgs::String msg) {
    label_tabfunc_voiceValue->setText(QString("当前语音识别结果是:")+QString::fromStdString(msg.data));
}

void MainWindow::callback_personDetectRes_subcriber(const sensor_msgs::Image::ConstPtr& msg) {
    //如果标志为关闭行人检测
    if(flag_switchPersonDecBtnText){
        return;
    }
    const cv_bridge::CvImageConstPtr &ptr = cv_bridge::toCvShare(msg, "bgr8");
    cv::Mat mat = ptr->image;
    QImage qimage = cvMat2QImage(mat);
    QPixmap tmp_pixmap = QPixmap::fromImage(qimage);
    QPixmap new_pixmap = tmp_pixmap.scaled(label_tabfunc_image->width(), label_tabfunc_image->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
//    QPixmap tmp_pixmap = pixmap1.scaled(label_picture1->width(), label_picture1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    label_tabfunc_image->setPixmap(new_pixmap);
}

void MainWindow::callback_grabDollImagRes_subcriber(const sensor_msgs::Image::ConstPtr& msg) {
    const cv_bridge::CvImageConstPtr &ptr = cv_bridge::toCvShare(msg, "bgr8");
    cv::Mat mat = ptr->image;
    QImage qimage = cvMat2QImage(mat);
    QPixmap tmp_pixmap = QPixmap::fromImage(qimage);
    QPixmap new_pixmap = tmp_pixmap.scaled(label_tabfunc_image->width(), label_tabfunc_image->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
//    QPixmap tmp_pixmap = pixmap1.scaled(label_picture1->width(), label_picture1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    label_tabfunc_image->setPixmap(new_pixmap);
}

void MainWindow::callback_robStatus_subscriber(const industrial_msgs::RobotStatus::ConstPtr robot_status) {
    mutex_devDetector.lock();
    RobConn_Detector.lifeNum=100;
    RobConn_Detector.status= true;
    RobErr_Detector.lifeNum=100;
    RobEnable_Detector.lifeNum=100;
    if(robot_status->in_error.val==0){
        RobErr_Detector.status= true;
    } else{
        RobErr_Detector.status= false;
    }
    if(robot_status->drives_powered.val==1){
        RobEnable_Detector.status= true;
    } else{
        RobEnable_Detector.status= false;
    }
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
}

void MainWindow::slot_btn_tabfunc_persondeteck() {
    ros::Publisher visionDetech=Node->advertise<std_msgs::Bool>("switch_of_vision_detect",1000);
    std_msgs::Bool msg;
    if(!flag_switchPersonDecBtnText){
        //打开行人检测(通过脚本打开)
        system("rosrun openni2_tracker peopledetection.sh");
        btn_tabfunc_persondeteck->setText("关闭行人检测");
    } else{
        //关闭行人检测
        msg.data= false;
        btn_tabfunc_persondeteck->setText("打开行人检测");
    }
    visionDetech.publish(msg);
    flag_switchPersonDecBtnText=!flag_switchPersonDecBtnText;
}





