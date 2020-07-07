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

    Timer_listen_SysResetThread = new QTimer(this);
    Timer_listen_SysResetThread->setInterval(5000);
    connect(Timer_listen_SysResetThread, &QTimer::timeout, this, &MainWindow::slot_timer_listen_SysResetThread);

    Timer_listen_SysErr = new QTimer(this);
    Timer_listen_SysErr->setInterval(6000);
    connect(Timer_listen_SysErr, &QTimer::timeout, this, &MainWindow::slot_timer_listenSysErrThread);

    //初始化节点观察者
    ob_node.setparm(this);
    //线程初始化
    rbQthread_devConnOrRviz = new rbQthread();
    rbQthread_devConnOrRviz->setParm(this,&MainWindow::thread_rbQthread_devConnOrRviz);

    rbQthread_beginRun = new rbQthread();
    rbQthread_beginRun->setParm(this,&MainWindow::thread_rbQthread_beginRun);

    rbQthread_sysStop = new rbQthread();
    rbQthread_sysStop->setParm(this,&MainWindow::thread_rbQthread_sysStop);

    rbQthread_sysReset = new rbQthread();
    rbQthread_sysReset->setParm(this,&MainWindow::thread_rbQthread_sysReset);

    rbQthread_persondeteck = new rbQthread();
    rbQthread_persondeteck->setParm(this,&MainWindow::thread_rbQthread_persondeteck);

    rbQthread_voicedeteck = new rbQthread();
    rbQthread_voicedeteck->setParm(this,&MainWindow::thread_rbQthread_voicedeteck);

    rbQthread_shakehand = new rbQthread();
    rbQthread_shakehand->setParm(this,&MainWindow::thread_rbQthread_shakehand);

    rbQthread_grepwawa = new rbQthread();
    rbQthread_grepwawa->setParm(this,&MainWindow::thread_rbQthread_grepwawa);

    rbQthread_handClaw_gesture = new rbQthread();

    rbQthreadList.push_back(rbQthread_devConnOrRviz);
    rbQthreadList.push_back(rbQthread_beginRun);
    rbQthreadList.push_back(rbQthread_sysStop);
    rbQthreadList.push_back(rbQthread_persondeteck);
    rbQthreadList.push_back(rbQthread_shakehand);
    rbQthreadList.push_back(rbQthread_grepwawa);
}

void MainWindow::initRosToptic(){
    //话题
    flag_forceSensor_publisher=Node->advertise<std_msgs::Bool>("forceSensor_moveFlag",1);
    rbGoHome_publisher=Node->advertise<std_msgs::Int8>("/back_home",1);
    visionDetech_publisher=Node->advertise<std_msgs::Bool>("switch_of_vision_detect",1000);
    camera_subscriber=Node->subscribe<sensor_msgs::Image>("/usb_cam/image_raw",1,boost::bind(&MainWindow::callback_camera_subscriber, this, _1));
//    camera_subscriber=Node->subscribe<sensor_msgs::Image>("/usb_cam/image_raw",1,boost::bind(&MainWindow::callback_camera_subscriber, this, _1));
    forceSensor_subscriber=Node->subscribe<geometry_msgs::Wrench>("daq_data", 1000, &MainWindow::callback_forceSensor_subscriber, this);
    voiceSolveRes_subcriber=Node->subscribe<std_msgs::String>("voiceSolve_res",1,&MainWindow::callback_voiceSolveRes_subcriber, this);
    personDetectRes_subcriber=Node->subscribe<sensor_msgs::Image>("videphoto_feedback",1,boost::bind(&MainWindow::callback_personDetectRes_subcriber, this, _1));
    grabDollImagRes_subcriber=Node->subscribe<sensor_msgs::Image>("DollDetection_image",1,boost::bind(&MainWindow::callback_grabDollImagRes_subcriber, this, _1));
    robStatus_subscriber=Node->subscribe<industrial_msgs::RobotStatus>("robot_status",1,boost::bind(&MainWindow::callback_robStatus_subscriber,this,_1));

    //服务
    RobReset_client = Node->serviceClient<hsr_rosi_device::ClearFaultSrv>("clear_robot_fault");
    RobEnable_client = Node->serviceClient<hsr_rosi_device::SetEnableSrv>("set_robot_enable");
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
    connect(btn_tabfunc_voiceDetect,&QPushButton::clicked,this,&MainWindow::slot_btn_tabfunc_voiceDetect);
    //日志界面
    connect(btn_tabrecord_outRecord,&QPushButton::clicked,this,&MainWindow::slot_btn_tabrecord_outRecord);
    connect(btn_tabrecord_clearRecord,&QPushButton::clicked,this,&MainWindow::slot_btn_tabrecord_clearRecord);
/*************************************************************************************************/

/*********************************自定义信号与槽函数绑定*************************************************/
    connect(this, &MainWindow::emitTextControl,this, &MainWindow::displayTextControl);
    connect(this, &MainWindow::emitLightColor,this, &MainWindow::showLightColor);
    connect(this, SIGNAL(emitQmessageBox(infoLevel ,QString)), this,SLOT(showQmessageBox(infoLevel,QString)));  //将自定义槽连接到自定义信号

//    connect(rbQthread_devConnOrRviz, SIGNAL(signal_SendMsgBox(infoLevel ,QString)), this,SLOT(thread_slot_devConnOrRviz()));

/*************************************************************************************************/
}

//设备连接
void MainWindow::slot_btn_tabmain_devConnOrRviz() {
    cout<<"点击设备连接按钮"<<endl;
    if(rbQthread_devConnOrRviz->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复连接设备!"));
    } else{
        rbQthread_devConnOrRviz->start();
        LOG("RUNINFO")->logInfoMessage("设备连接运行中");
    }
}

//开始运行
void MainWindow::slot_btn_tabmain_beginRun() {
    if(index_SysRunStep!=1){
        emit emitQmessageBox(infoLevel::warning,QString("请先进行设备连接或启动rviz!"));
        return;
    }

    if(rbQthread_beginRun->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复运行设备!"));
    } else{
        rbQthread_beginRun->start();
        LOG("RUNINFO")->logInfoMessage("设备开始运行中");
    }
}

//系统停止
void MainWindow::slot_btn_tabmain_sysStop() {
    if(rbQthread_sysStop->isRunning()){
        emit emitQmessageBox(infoLevel::warning,QString("请不要重复进行系统停止!"));
    } else{
        rbQthread_sysStop->start();
        LOG("RUNINFO")->logInfoMessage("设备停止");
    }
}
//系统复位 goto 等5s定时重启
void MainWindow::slot_btn_tabmain_sysReset() {
    if (rbQthread_sysReset->isRunning()) {
        emit emitQmessageBox(infoLevel::warning, QString("程序正在执行中,请不要重复启动!"));
    } else
    {
        index_SysRunStep=0;
        //掉机器人使能
        hsr_rosi_device::SetEnableSrv srv;
        srv.request.enable= false;
        RobEnable_client.call(srv);
        //释放线程资源
        for(auto thread:rbQthreadList){
            if(thread->isRunning()){
                thread->terminate();
                thread->wait(1000*1);
                cout<<"释放线程资源"<<endl;
            }
        }
        //启动复位线程,此线程可重复使用
//        rbQthread_sysReset->start();
        //开辟临时线程
        rbQthread* tmp_thread=new rbQthread();
        tmp_thread->setParm4([&]
        {
         flag_havedReset= true;
         ob_node.shutdownNode();
         system("rosnode kill -a");
         sleep(1);
         system("killall rosmaster");
         sleep(1);
         system("roscore");
        }
        );
        //5s后复位按钮才能再次使用
        connect(tmp_thread,SIGNAL(started()),this,SLOT(slot_rbQthread_listenSysResetStart()));
        connect(tmp_thread,SIGNAL(finished()),tmp_thread,SLOT(deleteLater()));
        connect(tmp_thread,SIGNAL(finished()),this,SLOT(slot_rbQthread_listenFinish()));
        tmp_thread->start();
        LOG("RUNINFO")->logInfoMessage("系统复位");

    }
}

//设备连接子线程
void MainWindow::thread_rbQthread_devConnOrRviz() {
    switch (cbox_tabmain_chooseMode->currentIndex()){
        case 0:
            emit emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //启动真机连接launch文件
            index_SysRunStep=1;
            system("roslaunch handrb_ui devconn.launch");
            break;
        case 2:
            //启动rviz文件
            index_SysRunStep=1;
            system("roslaunch handrb_ui devconn.launch");
            break;
    }
//            system("roslaunch co605_fight_moveit_config demo.launch");
}
//开始运行子线程
void MainWindow::thread_rbQthread_beginRun() {
    switch (cbox_tabmain_robmode->currentIndex()){
        case 0:
            emit emitQmessageBox(infoLevel::information,QString("请选择机器人模式!"));
            return;
        case 1:
            system("rosservice call /set_mode_srv \"mode: 0\"");
            break;
        case 2:
            //上使能
            hsr_rosi_device::ClearFaultSrv srv_clearF;
            hsr_rosi_device::SetEnableSrv srv2_setE;
            RobReset_client.call(srv_clearF);
            RobReset_client.call(srv2_setE);
            system("rosservice call /set_mode_srv \"mode: 1\"");
            break;
    }
    cout<<"开始运行模式"<<endl;
    switch (cbox_tabmain_chooseMode->currentIndex()){
        case 0:
            emit emitQmessageBox(infoLevel::information,QString("请选择运行模式!"));
            break;
        case 1:
            //启动真机运行launch文件
            system("roslaunch handrb_ui handRobotGrab.launch");
            //如果复位过程序,增需要重新启动本节点
            if(flag_havedReset){ob_node.rebootUiNode();}
            break;
        case 2:
            //启动rviz程序
            //如果复位过程序,增需要重新启动本节点
            if(flag_havedReset){ob_node.rebootUiNode();}
            system("roslaunch handrb_ui handRobotGrab.launch");
            //启动rviz文件运行文件
            break;
    }
}
//系统停止子线程
void MainWindow::thread_rbQthread_sysStop() {

    hsr_rosi_device::SetEnableSrv srv;
    srv.request.enable= false;
    if(RobEnable_client.call(srv)){
    } else{
        emit emitQmessageBox(infoLevel ::warning,"机器人掉使能服务连接失败");
    }
}
//系统复位子线程
void MainWindow::thread_rbQthread_sysReset() {
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
    std_msgs::Int8 msg;
    msg.data=0;
    rbGoHome_publisher.publish(msg);
}

void MainWindow::slot_btn_tabfunc_shakehand() {
    if(rbQthread_shakehand->isRunning()){
        emit emitQmessageBox(infoLevel::warning ,"与人握手程序运行中,请不要重复启动!");
    } else{
        rbQthread_shakehand->start();
    }
}

void MainWindow::slot_btn_tabfunc_grepwawa() {
    if(rbQthread_grepwawa->isRunning()){
        emit emitQmessageBox(infoLevel::warning ,"抓娃娃程序运行中,请不要重复启动!");
    } else{
        rbQthread_grepwawa->start();
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
    label_tabfunc_voiceValue->setText(QString("当前语音识别结果:")+QString::fromStdString(msg.data));
}

void MainWindow::callback_personDetectRes_subcriber(const sensor_msgs::Image::ConstPtr& msg) {
    //如果标志为关闭行人检测
    if(!flag_switchPersonDecBtnText){
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
}

void MainWindow::slot_btn_tabfunc_persondeteck() {
    mutex_devDetector.lock();
    cameraConn_Detector.lifeNum=100;
    cameraConn_Detector.status= true;
    mutex_devDetector.unlock();
    if(!flag_switchPersonDecBtnText){
        if(rbQthread_persondeteck->isRunning()){
            emit emitQmessageBox(infoLevel::information,QString("行人检测运行中"));
        } else{
            rbQthread_persondeteck->start();
        }
        btn_tabfunc_persondeteck->setText("关闭行人检测");
    } else{
        //关闭行人检测
        std_msgs::Bool msg;
        msg.data= false;
        visionDetech_publisher.publish(msg);
        btn_tabfunc_persondeteck->setText("打开行人检测");
    }
    flag_switchPersonDecBtnText=!flag_switchPersonDecBtnText;

}

void MainWindow::thread_rbQthread_persondeteck() {
    system("rosrun openni2_tracker peopledetection.sh");
}

void MainWindow::thread_rbQthread_shakehand() {
    emit emitQmessageBox(infoLevel::information,"与人握手信号发出");
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

void MainWindow::slot_btn_tabfunc_voiceDetect() {
    cout<<"点击了按钮"<<endl;
    if(rbQthread_voicedeteck->isRunning()){
        cout<<"语音程序正在运行中"<<endl;
        emit emitQmessageBox(infoLevel::warning,QString("语音程序正在运行中"));
    } else{
        rbQthread_voicedeteck->run();
    }
}

void MainWindow::thread_rbQthread_voicedeteck() {
    system("rosrun openni2_tracker voice.sh");
}

//重启UI节点
void observer_rebootUiNode::rebootUiNode(){
    sp=new ros::AsyncSpinner(1);
    sp->start();
    ros::start();
    mainwindow->initRosToptic();
}

