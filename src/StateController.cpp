#include "include/StateController.h"

StateController::~StateController() {

}
StateController::StateController() {
    //声控自动运行模式
    rbQthread_voiceCtl_AutoRun = new rbQthread();
    rbQthread_voiceCtl_AutoRun->setParm5(this,&StateController::voiceCtl_AutoRun);
    //
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

    rbQthread_spin= new rbQthread();
    rbQthread_spin->setParm5(this,&StateController::thread_rbQthread_spin);

    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_N1);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_1);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_3);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_4);
    rbQthreadList.push_back(rbQthread_voiceCtl_modeTask_5);
    rbQthreadList.push_back(rbQthread_rbGoShakeHandPose);
    cout<<"完成构造函数"<<endl;
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

//自动运行
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
                if(isStart){
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
                }
                if(voiceStep==13&&(!rbQthread_voiceCtl_modeTask_1->isRunning())){
                    mode=2;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式2");
                }
                break;
            //模式:2(行人减速设置模式)
            case 2:
                //a. 设置减速模式
                mode=3;
                plainTextEdit->appendPlainText("进入模式3,语音选择功能");
                break;
            //模式:3(等待握手抓娃娃/功能触发)
            case 3:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_3->isRunning())){
                    rbQthread_voiceCtl_modeTask_3->start();
                    assist_funcRunOnce= true;
                }
                if(voiceStep==32&&(!rbQthread_voiceCtl_modeTask_3->isRunning())){
                    mode=4;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式4,选择握手功能");
                }
                if(voiceStep==33&&(!rbQthread_voiceCtl_modeTask_3->isRunning())){
                    mode=5;
                    assist_funcRunOnce= false;
                }
            //模式:4(握手)
            case 4:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_4->isRunning())){
                    rbQthread_voiceCtl_modeTask_4->start();
                    assist_funcRunOnce= true;
                }
                if(voiceStep==43&&(!rbQthread_voiceCtl_modeTask_4->isRunning())){
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
                if(voiceStep==53&&(!rbQthread_voiceCtl_modeTask_5->isRunning())){
                    mode=1;
                    assist_funcRunOnce= false;
                    plainTextEdit->appendPlainText("进入模式1,等待行人检测");
                }
                break;
        }
    }
}


void StateController::initVal(){
    //数据初始初始化
    ctlState->isOk_robPreparePose= false;
    ctlState->flag_rbCtlBusy= false;
    ctlState->isEnd_shakeHand= false;
    ctlState->voice_order=-1;
    isStop= false;
    mode=0;
    voiceStep=0;
    assist_funcRunOnce= false;
    for(auto thread:rbQthreadList){
        if(thread->isRunning()){
            thread->terminate();
            cout<<"释放线程"<<endl;
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
    //如果正常/故障状态,切换模式
    if(ctlState->RobNormalState){
        mode=0;
    } else{
        mode=-1;
    }
    mutex_updateCtlState.unlock();
}

void StateController::ContrlMode_N1_task() {
    isStop= true;
    system("rosservice call /stop_motion");
    system("rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" &");
    system("rosservice call /set_mode_srv \"mode: 0\"");
    system("rostopic pub -1 /stop_move std_msgs/Bool \"data: true\"");
    //数据初始初始化
    initVal();
    //
    PersonDetect_Switch(false);
    VoiceDetect_Switch(false);
    //线程中断
    for(auto thread:rbQthreadList){
        if(thread->isRunning()){
            thread->terminate();
        }
    }

}

void StateController::thread_voiceCtl_modeTask_N1() {

}

void StateController::thread_voiceCtl_modeTask_1() {
    voiceStep=10;
    hirop_msgs::connectGripper srv;
    while ((!isStop)&&(voiceStep!=13))
    {
        switch (voiceStep)
        {
            //开启行人检测
            case 10:
                cout<<"开启行人检测"<<endl;
                PersonDetect_Switch(true);
                voiceStep=11;
                break;
            case 11:
//                //监听数据
                cout<<"监听数据"<<endl;
                if(rosTopicHd->personDetect_client->call(srv)){

                } else{
                    cout<<"服务失败"<<endl;
                };
                cout<<"判断结果"<<endl;
                //检测到行人
                if(srv.response.isConnected)
                {
                    cout<<"进入发消息中"<<endl;
                    std_msgs::String se_msg;
                    string voice_feedback = "检测到行人!";
                    se_msg.data = voice_feedback.c_str();
                    rosTopicHd->voice_order_publisher->publish(se_msg);

                    PersonDetect_Switch(false);
                    VoiceDetect_Switch(true);
                    voiceStep=12;
                }
                cout<<"判断完毕"<<endl;
                break;
            case 12:
                std_msgs::String se_msg;
                string voice_feedback = "你好!";
                se_msg.data = voice_feedback.c_str();
                rosTopicHd->voice_order_publisher->publish(se_msg);
                voiceStep=13;
                break;
        }

    }
}

//(等待握手抓娃娃/功能触发)任务子线程
void StateController::thread_voiceCtl_modeTask_3() {
    voiceStep=30;
    while ((!isStop)&&(voiceStep<32))
    {
        switch (voiceStep)
        {
            //开启声音检测
            case 30:
                VoiceDetect_Switch(true);
                voiceStep=31;
                break;
            case 31:
                if(ctlState->voice_order==2){
                    std_msgs::String se_msg;
                    string voice_feedback = "听到握手指令";
                    se_msg.data = voice_feedback.c_str();
                    rosTopicHd->voice_order_publisher->publish(se_msg);
                    voiceStep=32;
                }
                if(ctlState->voice_order==3){
                    std_msgs::String se_msg;
                    string voice_feedback = "听到抓娃娃指令";
                    se_msg.data = voice_feedback.c_str();
                    rosTopicHd->voice_order_publisher->publish(se_msg);
                    voiceStep=33;
                }
                break;
        }
    }

}

//握手任务子线程
void StateController::thread_voiceCtl_modeTask_4() {
    voiceStep=40;
    hsr_rosi_device::setModeSrv srv_SetMode;
    while ((!isStop)&&(voiceStep!=43))
    {
        switch (voiceStep){
            case 40:
                //判断机器人是否上使能
                if(!ctlState->RobEnableState)
                {
                    mode=-1;
                }
                else
                {
                    //机器人空闲中,执行去到抬手点位
                    if(!ctlState->flag_rbCtlBusy)
                    {
                        if(!rbQthread_rbGoShakeHandPose->isRunning()){
                            rbQthread_rbGoShakeHandPose->start();
                        }
                    }
                    //机器人去到点位成功
                    if(ctlState->isOk_robPreparePose){
                        ctlState->isOk_robPreparePose= false;
                        voiceStep=41;
                    }
                }
                break;
            case 41:
                if(!ctlState->flag_rbCtlBusy)
                {
                    //启动阻抗随动模式
                    srv_SetMode.request.mode=1;
                    if(rosTopicHd->RobSetMode_client->call(srv_SetMode)){
                        if(srv_SetMode.response.finsh){
                            cout<<"设置为随动模式"<<endl;
                        }
                    }else
                    {
                        cout<<"模式设置服务连接失败"<<endl;
                        mode=-1;
                    }
                    system("rosrun handrb_ui rbImpMoudle.sh &");
                    voiceStep=42;
                }
                break;
            case 42:
                //等待握手结束
                if(ctlState->isEnd_shakeHand){
                    system((char*)"rosservice call /stop_motion");
                    system((char*)"rostopic pub -1 /set_ready_exit std_msgs/Bool \"data: true\" &");
                    srv_SetMode.request.mode=0;
                    rosTopicHd->RobSetMode_client->call(srv_SetMode);
                    sleep(1);
                    std_msgs::Bool msg;
                    msg.data=true;
                    rosTopicHd->shakehandOver_publisher->publish(msg);
                    voiceStep=43;

                    std_msgs::String se_msg;
                    string voice_feedback = "握手完成,祝您生活愉快!";
                    se_msg.data = voice_feedback.c_str();
                    rosTopicHd->voice_order_publisher->publish(se_msg);
                }
                break;
        }
    }
}

//抓娃娃任务子线程
void StateController::thread_voiceCtl_modeTask_5() {

}


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

void StateController::thread_rbQthread_spin() {
    while (!isStop){
        cout<<"循环中"<<endl;
        sleep(1);
        lable_showinfo->setText(QString("模式:%1,步序:%2").arg(mode).arg(voiceStep));
    }
}

