#include "MainWindow.h"
#include "logmanager.h"
#include "globalvar.h"
#include "messagehandler.h"
using namespace std;


int main(int args,char** argv){
    //ros节点
    string nodeName = "hdrb_ui";
    ros::init(args, argv, nodeName);
    //创建节点
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle node;
    //应用程序
    QApplication app(args, argv);
    MainWindow mainwindow(&node);
    mainwindow.show();
    //日志初始化
    log4cplus::Initializer initializer;
    gloalMain = &mainwindow;
    qInstallMessageHandler(customMessageHandler);
    return app.exec();
}



