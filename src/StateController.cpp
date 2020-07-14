#include "include/StateController.h"

StateController::~StateController() {

}
StateController::StateController() {
    //声控自动运行模式
    rbQthread_voiceCtl_AutoRun = new rbQthread();
    rbQthread_voiceCtl_AutoRun->setParm5(this,&StateController::voiceCtl_AutoRun);
    //实时更新数据
    rbQthread_updateState = new rbQthread();
    rbQthread_updateState->setParm6(this,&StateController::updateState,ctlState);
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
}


void StateController::start() {
    if(!rbQthread_voiceCtl_AutoRun->isRunning()){
        rbQthread_voiceCtl_AutoRun->start();
    } else{
        cout<<"rbQthread_AutoRun线程正在运行中"<<endl;
    }
    if(!rbQthread_updateState->isRunning()){
        rbQthread_updateState->start();
    }
    rbQthread_spin->start();
}

//自动运行
void StateController::voiceCtl_AutoRun() {
    while (!isStop)
    {
        //主业务功能
        switch (mode)
        {
            //模式:-1(故障模式) 退出
            case -1:
                    ContrlMode_N1_task();//检测到故障马上停止运动
                break;
            //模式:0(等待模式)
            case 0:
                if(isStart){
                    mode=1;
                    isStart= false;
                }
                break;
            //模式:1(行人检测模式)
            case 1:
                if((!assist_funcRunOnce)&&(!rbQthread_voiceCtl_modeTask_1->isRunning())){
                    rbQthread_voiceCtl_modeTask_1->start();
                    assist_funcRunOnce= true;
                }
                if(voiceStep==13&&(!rbQthread_voiceCtl_modeTask_1->isRunning())){
                    mode=2;
                    assist_funcRunOnce= false;
                }
                break;
            //模式:2(行人减速设置模式)
            case 2:
                //a. 设置减速模式
                mode=2;
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
    ctlState->isOk_robPreparePose= false;
    ctlState->flag_rbCtlBusy= false;
    ctlState->isEnd_shakeHand= false;
    ctlState->voice_order=-1;
    //
    PersonDetect_Switch(false);
    VoiceDetect_Switch(false);
    //线程中断
    for(auto thread:rbQthreadList){
        if(thread->isRunning()){
            thread->quit();
        }
    }

}

void StateController::thread_voiceCtl_modeTask_N1() {

}

void StateController::thread_voiceCtl_modeTask_1() {
    voiceStep=10;
    rb_msgAndSrv::rb_EmptyAndArray srv;
    while ((!isStop)&&(voiceStep!=13))
    {
        switch (voiceStep)
        {
            //开启行人检测
            case 10:
                PersonDetect_Switch(true);
                voiceStep=11;
                break;
            case 11:
//                //监听数据
                rosTopicHd->personDetect_client->call(srv);
                //检测到行人
                if(srv.response.data[0]==1)
                {
                    PersonDetect_Switch(false);
                    VoiceDetect_Switch(true);
                    voiceStep=12;
                }
                break;
            case 12:
                //发出"你好!声音" 发出完毕,关闭行人检测,关闭语音模块
//                if("检测到"=="声音发出完毕"){
//                    VoiceDetect_Switch(false);
//                    voiceStep=13;
//                }
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
                    voiceStep=32;
                    //关闭声音检测
                    VoiceDetect_Switch(false);
                }
                if(ctlState->voice_order==3){
                    voiceStep=33;
                    //关闭声音检测
                    VoiceDetect_Switch(false);
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
    lable_showinfo->setText(QString("模式:%1,步序:%2").arg(mode).arg(voiceStep));
}

