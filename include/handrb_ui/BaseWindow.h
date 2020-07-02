

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
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QHBoxLayout *hLayout_tabmain_2;
    QGroupBox *groupBox_tabmain_2;
    QHBoxLayout *horizontalLayout_6;
    QComboBox *cbox_tabmain_chooseMode;
    QHBoxLayout *hLayout_tabmain_3;
    QGroupBox *groupBox_tabmain_3;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btn_tabmain_devConnOrRviz;
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
    //功能界面
    QWidget *tab_func;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *hLayout_tabfunc_1;
    QVBoxLayout *vLayout_tabfunc_12;
    QGroupBox *groupBox_tabfunc_1;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_tabfunc_image;
    QVBoxLayout *vLayout_tabfunc_11;
    QGroupBox *groupBox_tabfunc_2;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *btn_tabfunc_shakehand;
    QPushButton *btn_tabfunc_grepwawa;
    QGroupBox *groupBox_tabfunc_3;
    QVBoxLayout *verticalLayout_9;
    QGridLayout *gLayout_tabfunc_1;
    QLabel *label_tabfunc_grepCount;
    QLabel *label_tabfunc_shakehand;
    QLabel *label_tabfunc_grepCountValue;
    QLabel *label_tabfunc_shakehandValue;
    QLabel *label_tabfunc_voice;
    QLabel *label_tabfunc_voiceValue;
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
