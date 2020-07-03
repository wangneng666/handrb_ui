
#ifndef HANDRB_UI_RBQTHREAD_H
#define HANDRB_UI_RBQTHREAD_H

#include "gloalVal.h"

#include <QThread>
#include <iostream>



class MainWindow;
//多线程,线程与主线程信号与槽通信
class rbQthread : public QThread{
    Q_OBJECT
public:
    int mod=0;//模式
    string str;//
    MainWindow* m;
    void (MainWindow::*f1)();
    void (*f2)();
    void (MainWindow::*f3)(string );
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

    void setParm3(MainWindow* m,void (MainWindow::*f3)(string),string str){
        this->m=m;
        this->f3=f3;
        this->str=str;
        mod=3;
    }

    void run(){
        switch (mod){
            case 1:(m->*f1)();break;
            case 2:f2();break;
            case 3:(m->*f3)(str);break;
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
