
#ifndef HANDRB_UI_RBQTHREAD_H
#define HANDRB_UI_RBQTHREAD_H

#include "gloalVal.h"

#include <QThread>
#include <iostream>

class MainWindow;
class StateController;
//多线程,线程与主线程信号与槽通信
class rbQthread : public QThread{
    Q_OBJECT
public:
    int mod=0;//模式
    std::string str;//
    MainWindow* m;
    StateController* m1;
    controllerState* cstate;
    void (MainWindow::*f1)();
    void (*f2)();
    void (MainWindow::*f3)(std::string );
    std::function<void ()>  f4;
    void (StateController::*f5)();
    void (StateController::*f6)(controllerState* );
    //函数回调
    void setParm(MainWindow* m,void (MainWindow::*f1)()){
        this->m=m;
        this->f1=f1;
        mod=1;
    }

    void setParm2(void (*f2)()){
        this->f2=f2;
        mod=2;
    }

    void setParm3(MainWindow* m,void (MainWindow::*f3)(std::string),std::string str){
        this->m=m;
        this->f3=f3;
        this->str=str;
        mod=3;
    }

    void setParm4(std::function<void ()> f4){
        this->f4=f4;
        mod=4;
    }

    void setParm5(StateController* m1,void (StateController::*f5)()){
        this->m1=m1;
        this->f5=f5;
        mod=5;
    }

    void setParm6(StateController* m1,void (StateController::*f6)(controllerState* ),controllerState* cstate){
        this->m1=m1;
        this->f6=f6;
        this->cstate=cstate;
        mod=6;
    }


    void run(){
        switch (mod){
            case 1:(m->*f1)();break;
            case 2:f2();break;
            case 3:(m->*f3)(str);break;
            case 4:f4();break;
            case 5:(m1->*f5)();break;
            case 6:(m1->*f6)(cstate);break;

        }
    }
signals:
    void signal_SendMsgBox(infoLevel level,QString info);//infoLevel level,QString info

public:
    void deleteself(){
        delete this;
        std::cout<<"析构自身"<<std::endl;
    }
};


#endif
