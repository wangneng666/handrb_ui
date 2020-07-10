

#ifndef RB_UI_BASEWINDOW_H
#define RB_UI_BASEWINDOW_H
//qt库
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QMutex>
#include "QDebug"
#include "qdebug.h"

#include <iostream>
#include <ros/node_handle.h>

using namespace std;

#define  BTN_W 150
#define  BTN_H 50
#define  COMBOX_W 200
#define  COMBOX_H 50
#define  LABLE_STATUS_W 30
#define  LABLE_STATUS_H 30


class BaseWindow: public QMainWindow {
public:
    BaseWindow(ros::NodeHandle* node,QWidget* parent = Q_NULLPTR);
    ~BaseWindow();

public:
    //UI流程
    void initQtVal();
    void initUi(QMainWindow *MainWindow);
    void retranslateUi(QMainWindow *MainWindow);

public:
    //ros节点
    ros::NodeHandle* Node;
    //全局变量
    QString tab_qss;
    QString groupBox_qss;
    QString photoPath;
    QString logPath;
    QPixmap fitpixmap_redLight ;
    QPixmap fitpixmap_greenLight;
    QPixmap pixmap_voiceAwake;
    QPixmap pixmap_voicesleep;
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *vLayout_main_1;
    QHBoxLayout *hLayout_main_11;
    QLabel *label_main_logo;
    QLabel *label_main_title;
    QHBoxLayout *hLayout_main_12;
    QTabWidget *tabWidget;
    //主界面
    QWidget *tab_main;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *hLayout_tabmain_1;
    QGroupBox *groupBox_tabmain_1;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gLayout_tabmain_1;
    QLabel *label_tabmain_rbservoStatus;
    QLabel *label_tabmain_rbConnStatus;
    QLabel *label_tabmain_rbservoStatusValue;
    QLabel *label_tabmain_caConnStatus;
    QLabel *label_tabmain_caConnStatusValue;
    QLabel *label_tabmain_rbConnStatusValue;
    QLabel *label_tabmain_rbErrStatusValue;
    QLabel *label_tabmain_rbErrStatus;
    QLabel *label_tabmain_5fConnStatus;
    QLabel *label_tabmain_5fConnStatusValue;
    QLabel *label_tabmain_TsensorStatus;
    QLabel *label_tabmain_TsensorStatusValue;
    QLabel *label_tabmain_impedenceConnStatus;
    QLabel *label_tabmain_impedenceConnStatusValue;
    QLabel *label_tabmain_rbBusyStatus;
    QLabel *label_tabmain_rbBusyStatusValue;
    QHBoxLayout *hLayout_tabmain_2;
    QGroupBox *groupBox_tabmain_2;
    QHBoxLayout *horizontalLayout_6;
    QComboBox *cbox_tabmain_chooseMode;
    QHBoxLayout *hLayout_tabmain_3;
    QGroupBox *groupBox_tabmain_3;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btn_tabmain_beginRun;
    QPushButton *btn_tabmain_sysStop;
    QPushButton *btn_tabmain_sysReset;
    //调试界面
    QWidget *tab_debug;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_tabdebug_1;
    QHBoxLayout *horizontalLayout_14;
    QPushButton *btn_rbSetEnable;
    QPushButton *btn_rbReset;
    QGroupBox *groupBox_tabdebug_2;
    QHBoxLayout *horizontalLayout_19;
    QPushButton *btn_gripper_open;
    QPushButton *btn_gripper_close;
    QPushButton *btn_gripper_OK_Pose;
    QPushButton *btn_gripper_Y_Pose;
    QGroupBox *groupBox_tabdebug_3;
    QHBoxLayout *horizontalLayout_20;
    QPushButton *btn_rbGoHomePose;
    QPushButton *btn_startSensor;
    //握手界面
    QWidget *tab_shakeHand;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout;
    QGroupBox *gBox_tabShakeHand_status;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gLayout_tabShakeHand;
    QLabel *label_tabShakeHand_impStatusValue;
    QLabel *label_tabShakeHand_voiceStatusValue;
    QLabel *label_tabShakeHand_rbStatusValue;
    QLabel *label_tabShakeHand_voiceStatus;
    QLabel *label_tabShakeHand_rbCtlStatusValue;
    QLabel *label_tabShakeHand_rbCtlStatus;
    QLabel *label_tabShakeHand_rbStatus;
    QLabel *label_tabShakeHand_impStatus;
    QPushButton *btn_tabShakeHand_startRobRun;
    QPushButton *btn_tabShakeHand_startRobCtl;
    QPushButton *btn_tabShakeHand_startimpedence;
    QPushButton *btn_tabShakeHand_startvoice;
    QGroupBox *gBox_tabShakeHand_mode;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *cBox_tabShakeHand_setMode;
    QGroupBox *gBox_tabShakeHand_func;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btn_tabShakeHand_begin;
    QPushButton *btn_tabShakeHand_stop;
    QPushButton *btn_tabShakeHand_close;
    QPushButton *btn_tabShakeHand_shakeHandEnd;
    QPushButton *btn_tabShakeHand_AutoRun;
    //抓娃娃界面
    QWidget *tab_grabToy;
    QHBoxLayout *horizontalLayout_13;
    QHBoxLayout *hLayout_tab_grabToy;
    QVBoxLayout *vLayout_tab_grabToy_1;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_8;
    QLabel *lable_tabgrabToy_showImg;
    QVBoxLayout *vLayout_tab_grabToy_2;
    QGroupBox *gBox_tab_grabToy_1;
    QHBoxLayout *horizontalLayout_17;
    QGridLayout *gLayout_tab_grabToy;
    QLabel *label_tabgrabToy_voiceStatusValue;
    QLabel *label_tabgrabToy_rbStatusValue;
    QLabel *label_tab_grabToy_rbCtlStatus;
    QLabel *label_tabgrabToy_voiceStatus;
    QLabel *label_tab_grabToy_rbCtlStatusValue;
    QLabel *label_tabgrabToy_rbStatus;
    QPushButton *btn_tabgrabToy_startRobRun;
    QPushButton *btn_tabgrabToy_startRobCtl;
    QPushButton *btn_tabgrabToy_startvoice;
    QGroupBox *gBox_tab_grabToy_2;
    QHBoxLayout *horizontalLayout_18;
    QComboBox *cBox_tab_grabToy;
    QGroupBox *gBox_tab_grabToy_3;
    QHBoxLayout *horizontalLayout_16;
    QHBoxLayout *hLayout_tab_grabToy_3;
    QPushButton *btn_tab_grabToy_detect;
    QPushButton *btn_tab_grabToy_run;
    QPushButton *btn_tab_grabToy_close;
    //行人检测界面
    QWidget *tab_personDetect;
    QHBoxLayout *horizontalLayout_12;
    QHBoxLayout *hLayout_tab_personDetect_1;
    QVBoxLayout *vLayout_tab_personDetect_1;
    QGroupBox *gBox_tab_personDetect_1;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_tab_personDetect_showImag;
    QVBoxLayout *vLayout_tab_personDetect_2;
    QGroupBox *gBox_tab_personDetect_2;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *hLayout_tab_personDetect_21;
    QPushButton *btn_tab_personDetect_openPersonDetect;
    //声音检测界面
    QWidget *tab_voiceDetect;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *hLayout_tab_voiceDetect;
    QVBoxLayout *vLayout_tab_voiceDetect_2;
    QGroupBox *gBox_tab_voiceDetect_voiceCtl;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *hLayout_tab_voiceDetect_ctl1;
    QLabel *lable_tab_voiceDetect_showImg;
    QHBoxLayout *hLayout_tab_voiceDetect_ctl2;
    QPushButton *btn_tab_voiceDetect_run;
    QVBoxLayout *vLayout_tab_voiceDetect_1;
    QGroupBox *gBox_tab_voiceDetect_voiceInfo;
    QHBoxLayout *horizontalLayout_11;
    QPlainTextEdit *plainTextEdit;
    //日志界面
    QWidget *tab_record;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *hLayout_tabrecord_1;
    QVBoxLayout *vLayout_tabrecord_1;
    QPlainTextEdit *plainText_tabrecord_1;
    QVBoxLayout *vLayout_tabrecord_2;
    QPushButton *btn_tabrecord_outRecord;
    QPushButton *btn_tabrecord_clearRecord;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
};


#endif //RB_UI_BASEWINDOW_H
