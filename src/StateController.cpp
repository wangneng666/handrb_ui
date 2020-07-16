#include "include/StateController.h"

StateController::~StateController() {

}
StateController::StateController() {
    //声控自动运行模式
    rbQthread_voiceCtl_AutoRun = new rbQthread();
    rbQthread_voiceCtl_AutoRun->setParm5(this,&StateController::voiceCtl_AutoRun);
    //监控新城，一直运行
    rbQthread_spin= new rbQthread();
    rbQthread_spin->setParm5(this,&StateController::thread_rbQthread_spin);

    rbQthread_voiceCtl_modeTask_N1= new rbQthread();
    rbQthread_voiceCtl_modeTask_1= new rbQthread();
    rbQthread_voiceCtl_modeTask_3= new rbQthread();
    rbQthread_voiceCtl_modeTask_4= new rbQthread();
    rbQthread_voiceCtl_modeTask_5= new rbQthread();
    rbQthread_voiceCtl_modeTask_N1->setParm5(this,&StateController::thread_voiceCtl_modeTask_N1);
    rbQthread_voiceCtl_modeTask_1->setParm5(this,&StateController::thread_voiceCtl_modeTask_1);
    rbQthread_voiceCtl_modeTask_3->setParm5(this,&StateController::thread_voiceCtl_modeTask_3);
    rbQthread_voiceCtl_modeTask_4->setParm5(this,&StateController::thread_voiceCtl_modeTask_4);
    rbQthread_voiceCtl_modeTask_5->setParm5(this,&StateController::thread_voiceCtl_modeTask_5);

    rbQthread_rbGoShakeHandPose= new rbQthread();
    rbQthread_rbGoShakeHandPose->setParm5(this,&StateController::thread_rbGoShakeHandPose);

    rbQthread_timerFor10s= new rbQthread();
    rbQthread_timerFor10s->setParm5(this,&StateController::thread_timerFor10s);

    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_N1);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_1);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_3);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_4);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_5);
    rbQthreadList.push_back(rbQthread_rbGoShakeHandPose);
    rbQthreadList.push_back(rbQthread_timerFor10s);
    cout<<"完成构造函数"<<endl;
}

void StateController::initVal(){
    //数据初始初始化
    ctlState->isOk_robPreparePose= false;
    ctlState->flag_rbCtlBusy= false;
    ctlState->isEnd_shakeHand= false;
    ctlState->voice_order=-1;
    isStop= false;
    sub_isStop= false;
    mode=0;
    subStep=0;
    assist_funcRunOnce= false;
    // for(auto thread:rbQthreadList){
    //     if(thread->isRunning()){
    //         thread->terminate();
    //         cout<<"释放线程"<<endl;
    //     }
    // }
}

void StateController::start() {
    cout<<"开始运行"<<endl;
    if(!rbQthread_voiceCtl_AutoRun->isRunning()){
        cout<<"启动开始线程"<<endl;
        initVal();
        rbQthread_voiceCtl_AutoRun->start();
    } else{
        cout<<"rbQthread_AutoRun线程正在运行中,请不要重新启动"<<endl;
    }
    if(!rbQthread_spin->isRunning()){
        rbQthread_spin->start();
    }
}

//声控自动运行模式
void StateController::voiceCtl_AutoRun() {
    while (!isStop)
    {
        sleep(1);
        cout<<"mode="<<mode<<endl;
        cout<<"进入主业务逻辑"<<endl;
        //主业务功能
        switch (mode)
        {
            //模式:-1(故障模式) 退出
            case -1:
                    plainTextEdit->appendPlainText("进入故障模式-1");
                    ContrlMode_N1_task();//检测到故障马上停止运动
                break;
            //模式:0(等待模式)
            case 0:
                if(isStart)
                {
                    cout<<"开始"<<endl;
                    mode=1;
                    isStart= false;
                    plainTextEdit->appendPlainText("进入模式1,行人检测");
                }
                break;
            //模式:1(行人检测模式)
            case 1:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_1->isRunning())){
                    cout<<"启动线程rbQthread_voiceCtl_modeTask_1"<<endl;
                    rbQthread_voiceCtl_modeTask_1->start();
                    assist_funcRunOnce= true;
                    //开启10s定时
                    rbQthread_timerFor10s->start();
                }
                if(subStep==13&&(!rbQthread_voiceCtl_modeTask_1->isRunning())){
                    mode=2;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式2");
                }
                //如果10s定时完成还未检测到人
                if(!rbQthread_timerFor10s->isRunning()){
                    mode=0;
                    RobSayWords("十秒未检测到人，系统回到休眠状态，请按启动按钮重新激活");
                }
                break;
            //模式:2(行人减速设置模式)
            case 2:
                //设置减速比例
                // setRobSpeed(0.1);
                mode=3;
                plainTextEdit->appendPlainText("进入模式3,语音选择功能");
                break;
            //模式:3(等待握手抓娃娃/功能触发)
            case 3:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_3->isRunning())){
                    rbQthread_voiceCtl_modeTask_3->start();
                    assist_funcRunOnce= true;
                }
                if(subStep==32&&(!rbQthread_voiceCtl_modeTask_3->isRunning())){
                    mode=4;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式4,选择握手功能");
                }
                if(subStep==33&&(!rbQthread_voiceCtl_modeTask_3->isRunning())){
                    mode=5;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式5,选择抓娃娃功能");
                }
                break;
            //模式:4(握手)
            case 4:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_4->isRunning())){
                    rbQthread_voiceCtl_modeTask_4->start();
                    assist_funcRunOnce= true;
                }
                if(subStep==44&&(!rbQthread_voiceCtl_modeTask_4->isRunning())){
                    mode=1;
                    plainTextEdit->appendPlainText("进入模式1,等待行人检测");
                    assist_funcRunOnce= false;
                }
                break;
            //模式:5(抓娃娃)
            case 5:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_5->isRunning())){
                    rbQthread_voiceCtl_modeTask_5->start();
                    assist_funcRunOnce= true;
                }
                if(subStep==54&&(!rbQthread_voiceCtl_modeTask_5->isRunning())){
                    mode=1;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式1,等待行人检测");
                }
                break;
        }
    }
}



void StateController::ShareTopicHandle(rosTopicHandle* _rosTopicHd){
    rosTopicHd=_rosTopicHd;
}

//行人检测切换
void StateController::PersonDetect_Switch(bool flag) {
    if(flag){
        system("rosrun openni2_tracker peopledetection.sh &");
    } else{
        std_msgs::Bool msg;
        msg.data= false;
        rosTopicHd->visionDetech_publisher->publish(msg);
    }
}

//声音检测切换
void StateController::VoiceDetect_Switch(bool flag){
    if(flag){
        system("rosrun openni2_tracker voice.sh &");
    } else{
        system("rosrun openni2_tracker voice_shutdown.sh &");
    }
}

//实时获取更新状态
void StateController::updateState(controllerState* _ctlState) {
    mutex_updateCtlState.lock();
    ctlState = _ctlState;
    //如果故障状态,切换模式为-1
    if(!ctlState->RobNormalState){
        mode=-1;
    }
    mutex_updateCtlState.unlock();
}

void StateController::ContrlMode_N1_task() {
    isStop= true;
    sub_isStop= true;
    system("rosservice call /set_mode_srv \"mode: 0\"");
    system("rosservice call /stop_motion");
    system("rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" &");
//    system("rostopic pub -1 /stop_move std_msgs/Bool \"data: true\"");
    //关闭声音与图像检测
    rb_msgAndSrv::rb_DoubleBool srv;
    srv.request.request= false;
//    rosTopicHd->switch_personDetect_client->call(srv);
    rosTopicHd->switch_voiceDetect_client->call(srv);
    //关闭行人检测
    PersonDetect_Switch(false);
    //数据初始初始化
    initVal();
}

void StateController::thread_voiceCtl_modeTask_N1() {

}

void StateController::thread_voiceCtl_modeTask_1() {
    subStep=10;
    rb_msgAndSrv::rb_EmptyAndInt srv;
    rb_msgAndSrv::rb_DoubleBool srv_witchPersonDetect;
    rb_msgAndSrv::rb_DoubleBool srv_witchVoiceDetect;
    while ((!isStop)&&(!sub_isStop)&&(subStep!=13))
    {
        switch (subStep)
        {
            //开启声音和行人检测
            case 10:
                srv_witchVoiceDetect.request.request= true;
                if(rosTopicHd->switch_voiceDetect_client->call(srv_witchVoiceDetect)){
                    cout<<"声音检测开启成功"<<endl;
                }

//                srv_witchPersonDetect.request.request= true;
//                if(rosTopicHd->switch_personDetect_client->call(srv_witchPersonDetect))
//                {
//                    cout<<"行人检测开启成功"<<endl;
//                    subStep=11;
//                }
                //打开行人检测
                PersonDetect_Switch(true);
                subStep=11;
                break;
            case 11:
                //监听数据
                cout<<"监听数据"<<endl;
                if(rosTopicHd->personDetect_client->call(srv))
                {
                    //检测到行人
                    if(srv.response.res_data==1)
                    {
                        subStep=12;
                    }
                    else
                    {
                        cout<<"没有检测到行人"<<endl;
                    }
                } 
                else
                {
                    cout<<"行人检测服务连接失败"<<endl;
                }

                break;
            case 12:
                //发出声音
                RobSayWords("我可以和你做朋友吗");
                subStep=13;
                break;
        }

    }
}

//(等待握手抓娃娃/功能触发)任务子线程
void StateController::thread_voiceCtl_modeTask_3() {
    subStep=30;
    while ((!isStop)&&(!sub_isStop)&&(subStep<32))
    {
        switch (subStep)
        {
            case 30:
                //清理历史数据
                ctlState->voice_order=-1;
                //发出声音
                RobSayWords("请说出你的功能需求!");
                subStep=31;
                break;
            case 31:
                if(ctlState->voice_order==2){
                    subStep=32;
                }
                if(ctlState->voice_order==3){
                    subStep=33;
                }
                break;
            default:
                break;
        }
    }

}

//握手任务子线程
void StateController::thread_voiceCtl_modeTask_4() {
    subStep=40;
    while ((!isStop)&&(!sub_isStop)&&(subStep!=44))
    {
        switch (subStep){
            //机器人去到抬手点
            case 40:
                //判断机器人是否上使能
                if(!ctlState->RobEnableState)
                {
                    mode=0;
                    sub_isStop= true;
                    RobSayWords("准备握手,机器人没上使能,退到模式0,等待重新按开始按钮");
                    cout<<"准备握手,但是机器人没上使能,退到模式0"<<endl;
                }
                else
                {
                    //机器人空闲中,执行去到抬手点位
                    if(!ctlState->flag_rbCtlBusy)
                    {
                        if(!rbQthread_rbGoShakeHandPose->isRunning())
                        {
                            rbQthread_rbGoShakeHandPose->start();
                        }
                    }
                    //机器人去到点位成功
                    if(ctlState->isOk_robPreparePose)
                    {
                        ctlState->isOk_robPreparePose= false;
                        subStep=41;
                    }
                }
                break;
            //开启阻抗模式
            case 41:
                //当机器人空闲时
                if(!ctlState->flag_rbCtlBusy)
                {
                    //开启阻抗
                    startImpedence();
                    subStep=42;
                }
                break;
            case 42:
                //等待握手结束
                if(ctlState->isEnd_shakeHand){
                    ctlState->isEnd_shakeHand= false;
                    //关闭阻抗
                    closeImpedence();
                    sleep(1);
                    //给机器人控制模块发送握手结束信号
                    std_msgs::Bool msg;
                    msg.data=true;
                    rosTopicHd->shakehandOver_publisher->publish(msg);
                    RobSayWords("祝您生活愉快,再见!");
                    sleep(2);
                    subStep=43;
                }
            case 43:
                if(!ctlState->flag_rbCtlBusy){
                    subStep=44;
                }
                break;
        }
    }
}

//抓娃娃任务子线程
void StateController::thread_voiceCtl_modeTask_5() {
    subStep=50;
    while ((!isStop)&&(!sub_isStop)&&(subStep!=53))
    {
        switch (subStep)
        {
            case 50:
                //判断机器人是否上使能
                if(!ctlState->RobEnableState)
                {
                    mode=0;
                    sub_isStop= true;
                    RobSayWords("准备握手,机器人没上使能,退到模式0,等待重新按开始按钮");
                    cout<<"准备握手,但是机器人没上使能,退到模式0"<<endl;
                }
                else
                {
                    subStep=51;
                }

            case 51:
                //当机器人空闲时，去到检测点
                if(rb_grabToy_goPhotoPose()){
                    subStep=52;
                    cout<<"机器人去到检测点成功"<<endl;
                }
                else
                {
                    mode=0;
                    RobSayWords("去到检测点失败，退回模式0");
                    cout<<"去掉检测点,退到模式0"<<endl;
                }
                break;
            case 52:
                //拍照识别
                if(rb_grabToy_detectAndGrab()){
                    subStep=53;
                } else{
                    mode=0;
                    RobSayWords("抓取失败，退回模式0");
                    cout<<"抓取失败,退到模式0"<<endl;
                }
                break;
            case 53:
                if(!ctlState->flag_rbCtlBusy){
                    subStep=54;
                }
                break;
        }
    }

}

//去到机器人握手抬起点
void StateController::thread_rbGoShakeHandPose() {
    rb_msgAndSrv::rb_DoubleBool srv_shakeHand;
    srv_shakeHand.request.request= true;
    if(rosTopicHd->handClaw_shakeHand_client->call(srv_shakeHand))
    {
        if(srv_shakeHand.response.respond)
        {
            cout<<"机器人抬手服务阻塞结束"<<endl;
        }
    }
    else
    {
        cout<<"与人握手服务端连接失败"<<endl;
        mode=-1;
    }
}

//数据信息采集，监控流程完成度
void StateController::thread_rbQthread_spin() {
    while (flag_run){

        //刷新步序状态
        lable_showinfo->setText(QString("模式:%1,步序:%2").arg(mode).arg(subStep));

        //刷新模块运行状态标志
        int count=0;
        for(auto thread:rbQthreadList){
            if(thread->isRunning()){
                count++;
            }
        }
        if(count>0){
            isRunning=true;
        } else{
            isRunning= false;
        }

    }
}

void StateController::thread_timerFor10s(){
    sleep(30);
}

//说话
void StateController::RobSayWords(std::string words) {
    std_msgs::String se_msg;
    se_msg.data = words.c_str();
    rosTopicHd->voice_order_publisher->publish(se_msg);
}

//关闭阻抗
void StateController::closeImpedence() {
    cout<<"关闭阻抗"<<endl;
    hsr_rosi_device::setModeSrv srv_SetMode;
    srv_SetMode.request.mode=0;
    if(rosTopicHd->RobSetMode_client->call(srv_SetMode)){
        cout<<"设置为点动模式"<<endl;
    } else
    {
        cout<<"模式设置服务连接失败"<<endl;
    }
    //system((char*)"rosservice call /stop_motion");
     system("rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" &");
}

//开启阻抗
void StateController::startImpedence() {
    cout<<"开启阻抗"<<endl;
    hsr_rosi_device::setModeSrv srv_SetMode;
    //设置随动模式
    srv_SetMode.request.mode = 1;
    if (rosTopicHd->RobSetMode_client->call(srv_SetMode)) {
        if (srv_SetMode.response.finsh) {
            cout << "设置为随动模式" << endl;
        }
        //启动阻抗
        system("rosrun handrb_ui rbImpMoudle.sh &");
    }
    else
    {
        cout<<"模式设置服务连接失败"<<endl;
        mode=-1;
    }
}

void StateController::setRobSpeed(float data) {
    std_msgs::Float32 msg_speed;
    msg_speed.data=data;
    rosTopicHd->robSpeedSet_publisher->publish(msg_speed);
}

bool StateController::rb_grabToy_goPhotoPose(){
    std_srvs::Empty srv;
    if(!rosTopicHd->backHomeClient->call(srv))
    {
        ROS_INFO_STREAM("check back home server");
    }
    else
    {
        ROS_INFO_STREAM("back home SUCCESS");
    }
    rb_msgAndSrv::rb_DoubleBool  dSrv;
    dSrv.request.request = true;
    if(!rosTopicHd->detectePointClient->call(dSrv))
    {
        ROS_INFO_STREAM("check handClaw_detectDoll server");
    }
    else
    {
        if(dSrv.response.respond)
        {
            return true;
            ROS_INFO_STREAM("move to detete point SUCCESS");
        }
        else
        {
            ROS_INFO("move to detect point FAILURE");
        }
    }
    return false;
}

bool StateController::rb_grabToy_detectAndGrab(){
    hirop_msgs::detection d;
    d.request.detectorName = "Yolo6d";
    d.request.detectorType = 1;
    d.request.objectName = "toy1";
    if(rosTopicHd->detectionClient->call(d)){
        if(d.response.result){
            return true;
        }
    }
    return false;
}