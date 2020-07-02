
#include "BaseWindow.h"

BaseWindow::BaseWindow(ros::NodeHandle *node, QWidget *parent):QMainWindow(parent),Node(node) {
    initQtVal();
    //初始化UI
    initUi(this);
}

BaseWindow::~BaseWindow() {

}

void BaseWindow::initQtVal() {
    tab_qss=
            "QTabBar::tab{width:100}\n"
            "QTabBar::tab{height:40}";
    groupBox_qss=
            "QGroupBox{\n""\n"
            "border-width:2px;\n""\n"
            "border-style:solid;\n""\n"
            "border-radius: 10px;\n""\n"
            "border-color:gray;\n""\n"
            "margin-top:0.5ex;\n""\n""}\n""\n"
            "QGroupBox::title{\n""\n"
            "subcontrol-origin:margin;\n""\n"
            "subcontrol-position:top left;\n""\n"
            "left:10px;\n""\n"
            "margin-left:0px;\n""\n"
            "padding:0 1px;\n""\n""}"
            ;
    //获取工程文件路径
    photoPath= QDir::currentPath() +QString("/src/HS_HandGrasp/handrb_ui/photo/");
    logPath= QDir::currentPath();
    QPixmap tmp_pixmap_red=QPixmap(photoPath+"light_red.png");
    QPixmap tmp_pixmap_green=QPixmap(photoPath+"light_green.png");
    fitpixmap_redLight = tmp_pixmap_red.scaled(LABLE_STATUS_W,LABLE_STATUS_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    fitpixmap_greenLight = tmp_pixmap_green.scaled(LABLE_STATUS_W,LABLE_STATUS_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充

}


void BaseWindow::initUi(QMainWindow *MainWindow) {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(886, 650);
    centralWidget = new QWidget(MainWindow);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    verticalLayout_3 = new QVBoxLayout(centralWidget);
    verticalLayout_3->setSpacing(6);
    verticalLayout_3->setContentsMargins(11, 11, 11, 11);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    vLayout_main_1 = new QVBoxLayout();
    vLayout_main_1->setSpacing(6);
    vLayout_main_1->setObjectName(QString::fromUtf8("vLayout_main_1"));
    hLayout_main_11 = new QHBoxLayout();
    hLayout_main_11->setSpacing(6);
    hLayout_main_11->setObjectName(QString::fromUtf8("hLayout_main_11"));
    label_main_logo = new QLabel(centralWidget);
    label_main_logo->setObjectName(QString::fromUtf8("label_main_logo"));
    label_main_logo->setPixmap(QPixmap(photoPath+QString("logo.png")));

    hLayout_main_11->addWidget(label_main_logo);

    label_main_title = new QLabel(centralWidget);
    label_main_title->setObjectName(QString::fromUtf8("label_main_title"));
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    font.setItalic(false);
    font.setWeight(75);
    label_main_title->setFont(font);
    label_main_title->setAlignment(Qt::AlignCenter);

    hLayout_main_11->addWidget(label_main_title);

    hLayout_main_11->setStretch(1, 6);

    vLayout_main_1->addLayout(hLayout_main_11);

    hLayout_main_12 = new QHBoxLayout();
    hLayout_main_12->setSpacing(6);
    hLayout_main_12->setObjectName(QString::fromUtf8("hLayout_main_12"));
    tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setStyleSheet(tab_qss);
    tab_main = new QWidget();
    tab_main->setObjectName(QString::fromUtf8("tab_main"));
    verticalLayout_2 = new QVBoxLayout(tab_main);
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_4 = new QVBoxLayout();
    verticalLayout_4->setSpacing(6);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    hLayout_tabmain_1 = new QHBoxLayout();
    hLayout_tabmain_1->setSpacing(6);
    hLayout_tabmain_1->setObjectName(QString::fromUtf8("hLayout_tabmain_1"));
    groupBox_tabmain_1 = new QGroupBox(tab_main);
    groupBox_tabmain_1->setObjectName(QString::fromUtf8("groupBox_tabmain_1"));
    groupBox_tabmain_1->setStyleSheet(groupBox_qss);
    horizontalLayout_5 = new QHBoxLayout(groupBox_tabmain_1);
    horizontalLayout_5->setSpacing(6);
    horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
    gLayout_tabmain_1 = new QGridLayout();
    gLayout_tabmain_1->setSpacing(6);
    gLayout_tabmain_1->setObjectName(QString::fromUtf8("gLayout_tabmain_1"));
    label_tabmain_rbservoStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbservoStatus->setObjectName(QString::fromUtf8("label_tabmain_rbservoStatus"));
    label_tabmain_rbservoStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_rbservoStatus->setLayoutDirection(Qt::RightToLeft);
    label_tabmain_rbservoStatus->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gLayout_tabmain_1->addWidget(label_tabmain_rbservoStatus, 0, 3, 1, 1);

    label_tabmain_rbConnStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbConnStatus->setObjectName(QString::fromUtf8("label_tabmain_rbConnStatus"));
    label_tabmain_rbConnStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_rbConnStatus->setLayoutDirection(Qt::RightToLeft);
    label_tabmain_rbConnStatus->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gLayout_tabmain_1->addWidget(label_tabmain_rbConnStatus, 0, 1, 1, 1);

    label_tabmain_rbservoStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbservoStatusValue->setObjectName(QString::fromUtf8("label_tabmain_rbservoStatusValue"));
    label_tabmain_rbservoStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_rbservoStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_rbservoStatusValue, 0, 4, 1, 1);

    label_tabmain_caConnStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_caConnStatus->setObjectName(QString::fromUtf8("label_tabmain_caConnStatus"));
    label_tabmain_caConnStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_caConnStatus->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_tabmain_caConnStatus, 0, 7, 1, 1);

    label_tabmain_caConnStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_caConnStatusValue->setObjectName(QString::fromUtf8("label_tabmain_caConnStatusValue"));
    label_tabmain_caConnStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_caConnStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_caConnStatusValue, 0, 8, 1, 1);

    label_tabmain_rbConnStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbConnStatusValue->setObjectName(QString::fromUtf8("label_tabmain_rbConnStatusValue"));
    label_tabmain_rbConnStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_rbConnStatusValue->setPixmap(fitpixmap_redLight);
    label_tabmain_rbConnStatusValue->setScaledContents(false);

    gLayout_tabmain_1->addWidget(label_tabmain_rbConnStatusValue, 0, 2, 1, 1);

    label_tabmain_rbErrStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbErrStatusValue->setObjectName(QString::fromUtf8("label_tabmain_rbErrStatusValue"));
    label_tabmain_rbErrStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_rbErrStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_rbErrStatusValue, 0, 6, 1, 1);

    label_tabmain_rbErrStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbErrStatus->setObjectName(QString::fromUtf8("label_tabmain_rbErrStatus"));
    label_tabmain_rbErrStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_rbErrStatus->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_tabmain_rbErrStatus, 0, 5, 1, 1);

    label_tabmain_5fConnStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_5fConnStatus->setObjectName(QString::fromUtf8("label_tabmain_5fConnStatus"));
    label_tabmain_5fConnStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_5fConnStatus->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_tabmain_5fConnStatus, 1, 1, 1, 1);

    label_tabmain_5fConnStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_5fConnStatusValue->setObjectName(QString::fromUtf8("label_tabmain_5fConnStatusValue"));
    label_tabmain_5fConnStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_5fConnStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_5fConnStatusValue, 1, 2, 1, 1);

    label_tabmain_TsensorStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_TsensorStatus->setObjectName(QString::fromUtf8("label_tabmain_TsensorStatus"));
    label_tabmain_TsensorStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_TsensorStatus->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_tabmain_TsensorStatus, 1, 3, 1, 1);

    label_tabmain_TsensorStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_TsensorStatusValue->setObjectName(QString::fromUtf8("label_tabmain_TsensorStatusValue"));
    label_tabmain_TsensorStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_TsensorStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_TsensorStatusValue, 1, 4, 1, 1);

    label_13 = new QLabel(groupBox_tabmain_1);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setMaximumSize(QSize(150, 50));
    label_13->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_13, 1, 5, 1, 1);

    label_14 = new QLabel(groupBox_tabmain_1);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setMaximumSize(QSize(150, 50));
    label_14->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_14, 1, 6, 1, 1);

    label_15 = new QLabel(groupBox_tabmain_1);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setMaximumSize(QSize(150, 50));
    label_15->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_15, 1, 7, 1, 1);

    label_16 = new QLabel(groupBox_tabmain_1);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setMaximumSize(QSize(150, 50));
    label_16->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_16, 1, 8, 1, 1);


    horizontalLayout_5->addLayout(gLayout_tabmain_1);


    hLayout_tabmain_1->addWidget(groupBox_tabmain_1);


    verticalLayout_4->addLayout(hLayout_tabmain_1);

    hLayout_tabmain_2 = new QHBoxLayout();
    hLayout_tabmain_2->setSpacing(6);
    hLayout_tabmain_2->setObjectName(QString::fromUtf8("hLayout_tabmain_2"));
    groupBox_tabmain_2 = new QGroupBox(tab_main);
    groupBox_tabmain_2->setObjectName(QString::fromUtf8("groupBox_tabmain_2"));
    groupBox_tabmain_2->setStyleSheet(groupBox_qss);
    horizontalLayout_6 = new QHBoxLayout(groupBox_tabmain_2);
    horizontalLayout_6->setSpacing(6);
    horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
    cbox_tabmain_chooseMode = new QComboBox(groupBox_tabmain_2);
    cbox_tabmain_chooseMode->addItem(QString());
    cbox_tabmain_chooseMode->addItem(QString());
    cbox_tabmain_chooseMode->addItem(QString());
    cbox_tabmain_chooseMode->setObjectName(QString::fromUtf8("cbox_tabmain_chooseMode"));
    cbox_tabmain_chooseMode->setMaximumSize(QSize(200, 50));
    cbox_tabmain_chooseMode->setFixedSize(COMBOX_W,COMBOX_H);
    cbox_tabmain_chooseMode->setLayoutDirection(Qt::LeftToRight);

    horizontalLayout_6->addWidget(cbox_tabmain_chooseMode);


    hLayout_tabmain_2->addWidget(groupBox_tabmain_2);


    verticalLayout_4->addLayout(hLayout_tabmain_2);

    hLayout_tabmain_3 = new QHBoxLayout();
    hLayout_tabmain_3->setSpacing(6);
    hLayout_tabmain_3->setObjectName(QString::fromUtf8("hLayout_tabmain_3"));
    groupBox_tabmain_3 = new QGroupBox(tab_main);
    groupBox_tabmain_3->setObjectName(QString::fromUtf8("groupBox_tabmain_3"));
    groupBox_tabmain_3->setStyleSheet(groupBox_qss);
    horizontalLayout_7 = new QHBoxLayout(groupBox_tabmain_3);
    horizontalLayout_7->setSpacing(6);
    horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
    btn_tabmain_devConnOrRviz = new QPushButton(groupBox_tabmain_3);
    btn_tabmain_devConnOrRviz->setObjectName(QString::fromUtf8("btn_tabmain_devConnOrRviz"));
    btn_tabmain_devConnOrRviz->setMaximumSize(QSize(150, 50));
    btn_tabmain_devConnOrRviz->setFixedSize(BTN_W,BTN_H);
    horizontalLayout_7->addWidget(btn_tabmain_devConnOrRviz);

    btn_tabmain_beginRun = new QPushButton(groupBox_tabmain_3);
    btn_tabmain_beginRun->setObjectName(QString::fromUtf8("btn_tabmain_beginRun"));
    btn_tabmain_beginRun->setMaximumSize(QSize(150, 50));
    btn_tabmain_beginRun->setFixedSize(BTN_W,BTN_H);

    horizontalLayout_7->addWidget(btn_tabmain_beginRun);

    btn_tabmain_sysStop = new QPushButton(groupBox_tabmain_3);
    btn_tabmain_sysStop->setObjectName(QString::fromUtf8("btn_tabmain_sysStop"));
    btn_tabmain_sysStop->setMaximumSize(QSize(150, 50));
    btn_tabmain_sysStop->setFixedSize(BTN_W,BTN_H);

    horizontalLayout_7->addWidget(btn_tabmain_sysStop);

    btn_tabmain_sysReset = new QPushButton(groupBox_tabmain_3);
    btn_tabmain_sysReset->setObjectName(QString::fromUtf8("btn_tabmain_sysReset"));
    btn_tabmain_sysReset->setMaximumSize(QSize(150, 50));
    btn_tabmain_sysReset->setFixedSize(BTN_W,BTN_H);

    btn_tabmain_devConnOrRviz->setEnabled(false);
    btn_tabmain_beginRun->setEnabled(false);
    btn_tabmain_sysStop->setEnabled(false);
    btn_tabmain_sysReset->setEnabled(false);

    horizontalLayout_7->addWidget(btn_tabmain_sysReset);


    hLayout_tabmain_3->addWidget(groupBox_tabmain_3);


    verticalLayout_4->addLayout(hLayout_tabmain_3);

    verticalLayout_4->setStretch(0, 2);
    verticalLayout_4->setStretch(1, 1);
    verticalLayout_4->setStretch(2, 2);

    verticalLayout_2->addLayout(verticalLayout_4);

    tabWidget->addTab(tab_main, QString());
    tab_debug = new QWidget();
    tab_debug->setObjectName(QString::fromUtf8("tab_debug"));
    verticalLayout_5 = new QVBoxLayout(tab_debug);
    verticalLayout_5->setSpacing(6);
    verticalLayout_5->setContentsMargins(11, 11, 11, 11);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    groupBox_tabdebug_1 = new QGroupBox(tab_debug);
    groupBox_tabdebug_1->setObjectName(QString::fromUtf8("groupBox_tabdebug_1"));
    groupBox_tabdebug_1->setStyleSheet(groupBox_qss);
    horizontalLayout_14 = new QHBoxLayout(groupBox_tabdebug_1);
    horizontalLayout_14->setSpacing(6);
    horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
    btn_rbSetEnable = new QPushButton(groupBox_tabdebug_1);
    btn_rbSetEnable->setObjectName(QString::fromUtf8("btn_rbSetEnable"));
    btn_rbSetEnable->setMaximumSize(QSize(150, 50));
    btn_rbSetEnable->setFixedSize(BTN_W,BTN_H);

    horizontalLayout_14->addWidget(btn_rbSetEnable);

    btn_rbReset = new QPushButton(groupBox_tabdebug_1);
    btn_rbReset->setObjectName(QString::fromUtf8("btn_rbReset"));
    btn_rbReset->setMaximumSize(QSize(150, 50));
    btn_rbReset->setFixedSize(BTN_W,BTN_H);

    horizontalLayout_14->addWidget(btn_rbReset);


    verticalLayout_5->addWidget(groupBox_tabdebug_1);

    groupBox_tabdebug_2 = new QGroupBox(tab_debug);
    groupBox_tabdebug_2->setObjectName(QString::fromUtf8("groupBox_tabdebug_2"));
    groupBox_tabdebug_2->setStyleSheet(groupBox_qss);
    horizontalLayout_19 = new QHBoxLayout(groupBox_tabdebug_2);
    horizontalLayout_19->setSpacing(6);
    horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
    btn_gripper_open = new QPushButton(groupBox_tabdebug_2);
    btn_gripper_open->setObjectName(QString::fromUtf8("btn_gripper_open"));
    btn_gripper_open->setMaximumSize(QSize(150, 50));

    horizontalLayout_19->addWidget(btn_gripper_open);

    btn_gripper_close = new QPushButton(groupBox_tabdebug_2);
    btn_gripper_close->setObjectName(QString::fromUtf8("btn_gripper_close"));
    btn_gripper_close->setMaximumSize(QSize(150, 50));

    horizontalLayout_19->addWidget(btn_gripper_close);

    btn_gripper_OK_Pose = new QPushButton(groupBox_tabdebug_2);
    btn_gripper_OK_Pose->setObjectName(QString::fromUtf8("btn_gripper_OK_Pose"));
    btn_gripper_OK_Pose->setMaximumSize(QSize(150, 50));

    horizontalLayout_19->addWidget(btn_gripper_OK_Pose);

    btn_gripper_Y_Pose = new QPushButton(groupBox_tabdebug_2);
    btn_gripper_Y_Pose->setObjectName(QString::fromUtf8("btn_gripper_Y_Pose"));
    btn_gripper_Y_Pose->setMaximumSize(QSize(150, 50));

    horizontalLayout_19->addWidget(btn_gripper_Y_Pose);


    verticalLayout_5->addWidget(groupBox_tabdebug_2);

    groupBox_tabdebug_3 = new QGroupBox(tab_debug);
    groupBox_tabdebug_3->setObjectName(QString::fromUtf8("groupBox_tabdebug_3"));
    groupBox_tabdebug_3->setStyleSheet(groupBox_qss);
    horizontalLayout_20 = new QHBoxLayout(groupBox_tabdebug_3);
    horizontalLayout_20->setSpacing(6);
    horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
    btn_rbGoHomePose = new QPushButton(groupBox_tabdebug_3);
    btn_rbGoHomePose->setObjectName(QString::fromUtf8("btn_rbGoHomePose"));
    btn_rbGoHomePose->setMaximumSize(QSize(150, 50));
    btn_rbGoHomePose->setFixedSize(BTN_W,BTN_H);

    horizontalLayout_20->addWidget(btn_rbGoHomePose);


    verticalLayout_5->addWidget(groupBox_tabdebug_3);

    tabWidget->addTab(tab_debug, QString());
    tab_func = new QWidget();
    tab_func->setObjectName(QString::fromUtf8("tab_func"));
    horizontalLayout_9 = new QHBoxLayout(tab_func);
    horizontalLayout_9->setSpacing(6);
    horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
    hLayout_tabfunc_1 = new QHBoxLayout();
    hLayout_tabfunc_1->setSpacing(6);
    hLayout_tabfunc_1->setObjectName(QString::fromUtf8("hLayout_tabfunc_1"));
    vLayout_tabfunc_12 = new QVBoxLayout();
    vLayout_tabfunc_12->setSpacing(6);
    vLayout_tabfunc_12->setObjectName(QString::fromUtf8("vLayout_tabfunc_12"));
    groupBox_tabfunc_1 = new QGroupBox(tab_func);
    groupBox_tabfunc_1->setObjectName(QString::fromUtf8("groupBox_tabfunc_1"));
    groupBox_tabfunc_1->setStyleSheet(groupBox_qss);
    horizontalLayout_11 = new QHBoxLayout(groupBox_tabfunc_1);
    horizontalLayout_11->setSpacing(6);
    horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
    label_tabfunc_image = new QLabel(groupBox_tabfunc_1);
    label_tabfunc_image->setObjectName(QString::fromUtf8("label_tabfunc_image"));
    label_tabfunc_image->setFixedSize(512,424);
    //label_tabfunc_image->setMaximumSize(QSize(300, 300));
    label_tabfunc_image->setPixmap(QPixmap(QString::fromUtf8("../catkin_ws/src/HsDualAppBridge/rb_ui/photo/question.jpg")));

    horizontalLayout_11->addWidget(label_tabfunc_image);


    vLayout_tabfunc_12->addWidget(groupBox_tabfunc_1);


    hLayout_tabfunc_1->addLayout(vLayout_tabfunc_12);

    vLayout_tabfunc_11 = new QVBoxLayout();
    vLayout_tabfunc_11->setSpacing(6);
    vLayout_tabfunc_11->setObjectName(QString::fromUtf8("vLayout_tabfunc_11"));
    groupBox_tabfunc_2 = new QGroupBox(tab_func);
    groupBox_tabfunc_2->setObjectName(QString::fromUtf8("groupBox_tabfunc_2"));
    groupBox_tabfunc_2->setAutoFillBackground(false);
    groupBox_tabfunc_2->setStyleSheet(groupBox_qss);
    horizontalLayout_12 = new QHBoxLayout(groupBox_tabfunc_2);
    horizontalLayout_12->setSpacing(6);
    horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
    btn_tabfunc_shakehand = new QPushButton(groupBox_tabfunc_2);
    btn_tabfunc_shakehand->setObjectName(QString::fromUtf8("btn_tabfunc_shakehand"));
    btn_tabfunc_shakehand->setMaximumSize(QSize(150, 50));
    btn_tabfunc_shakehand->setFixedSize(BTN_W,BTN_H);
    btn_tabfunc_shakehand->setLayoutDirection(Qt::LeftToRight);

    horizontalLayout_12->addWidget(btn_tabfunc_shakehand);

    btn_tabfunc_grepwawa = new QPushButton(groupBox_tabfunc_2);
    btn_tabfunc_grepwawa->setObjectName(QString::fromUtf8("btn_tabfunc_grepwawa"));
    btn_tabfunc_grepwawa->setMaximumSize(QSize(150, 50));
    btn_tabfunc_grepwawa->setFixedSize(BTN_W,BTN_H);
    btn_tabfunc_grepwawa->setLayoutDirection(Qt::LeftToRight);

    horizontalLayout_12->addWidget(btn_tabfunc_grepwawa);

    btn_tabfunc_persondeteck = new QPushButton(groupBox_tabfunc_2);
    btn_tabfunc_persondeteck->setObjectName(QString::fromUtf8("btn_tabfunc_persondeteck"));
    btn_tabfunc_persondeteck->setMaximumSize(QSize(150, 50));
    btn_tabfunc_persondeteck->setFixedSize(BTN_W,BTN_H);
    btn_tabfunc_persondeteck->setLayoutDirection(Qt::LeftToRight);
    btn_tabfunc_persondeteck->setText("打开行人检测");

    horizontalLayout_12->addWidget(btn_tabfunc_persondeteck);


    vLayout_tabfunc_11->addWidget(groupBox_tabfunc_2);

    groupBox_tabfunc_3 = new QGroupBox(tab_func);
    groupBox_tabfunc_3->setObjectName(QString::fromUtf8("groupBox_tabfunc_3"));
    groupBox_tabfunc_3->setStyleSheet(groupBox_qss);
    verticalLayout_9 = new QVBoxLayout(groupBox_tabfunc_3);
    verticalLayout_9->setSpacing(6);
    verticalLayout_9->setContentsMargins(11, 11, 11, 11);
    verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
    gLayout_tabfunc_1 = new QGridLayout();
    gLayout_tabfunc_1->setSpacing(6);
    gLayout_tabfunc_1->setObjectName(QString::fromUtf8("gLayout_tabfunc_1"));
    label_tabfunc_grepCount = new QLabel(groupBox_tabfunc_3);
    label_tabfunc_grepCount->setObjectName(QString::fromUtf8("label_tabfunc_grepCount"));
    label_tabfunc_grepCount->setMaximumSize(QSize(100, 16777215));

    gLayout_tabfunc_1->addWidget(label_tabfunc_grepCount, 0, 0, 1, 1);

    label_tabfunc_shakehand = new QLabel(groupBox_tabfunc_3);
    label_tabfunc_shakehand->setObjectName(QString::fromUtf8("label_tabfunc_shakehand"));
    label_tabfunc_shakehand->setMaximumSize(QSize(100, 16777215));

    gLayout_tabfunc_1->addWidget(label_tabfunc_shakehand, 1, 0, 1, 1);

    label_tabfunc_grepCountValue = new QLabel(groupBox_tabfunc_3);
    label_tabfunc_grepCountValue->setObjectName(QString::fromUtf8("label_tabfunc_grepCountValue"));

    gLayout_tabfunc_1->addWidget(label_tabfunc_grepCountValue, 0, 1, 1, 1);

    label_tabfunc_shakehandValue = new QLabel(groupBox_tabfunc_3);
    label_tabfunc_shakehandValue->setObjectName(QString::fromUtf8("label_tabfunc_shakehandValue"));

    gLayout_tabfunc_1->addWidget(label_tabfunc_shakehandValue, 1, 1, 1, 1);

    label_tabfunc_voice = new QLabel(groupBox_tabfunc_3);
    label_tabfunc_voice->setObjectName(QString::fromUtf8("label_tabfunc_voice"));

    gLayout_tabfunc_1->addWidget(label_tabfunc_voice, 2, 0, 1, 1);

    label_tabfunc_voiceValue = new QLabel(groupBox_tabfunc_3);
    label_tabfunc_voiceValue->setObjectName(QString::fromUtf8("label_tabfunc_voiceValue"));

    gLayout_tabfunc_1->addWidget(label_tabfunc_voiceValue, 2, 1, 1, 1);


    verticalLayout_9->addLayout(gLayout_tabfunc_1);


    vLayout_tabfunc_11->addWidget(groupBox_tabfunc_3);


    hLayout_tabfunc_1->addLayout(vLayout_tabfunc_11);


    horizontalLayout_9->addLayout(hLayout_tabfunc_1);

    tabWidget->addTab(tab_func, QString());

    tab_record = new QWidget();
    tab_record->setObjectName(QString::fromUtf8("tab_record"));
    horizontalLayout = new QHBoxLayout(tab_record);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    hLayout_tabrecord_1 = new QHBoxLayout();
    hLayout_tabrecord_1->setSpacing(6);
    hLayout_tabrecord_1->setObjectName(QString::fromUtf8("hLayout_tabrecord_1"));
    vLayout_tabrecord_1 = new QVBoxLayout();
    vLayout_tabrecord_1->setSpacing(6);
    vLayout_tabrecord_1->setObjectName(QString::fromUtf8("vLayout_tabrecord_1"));
    plainText_tabrecord_1 = new QPlainTextEdit(tab_record);
    plainText_tabrecord_1->setObjectName(QString::fromUtf8("plainText_tabrecord_1"));

    vLayout_tabrecord_1->addWidget(plainText_tabrecord_1);


    hLayout_tabrecord_1->addLayout(vLayout_tabrecord_1);

    vLayout_tabrecord_2 = new QVBoxLayout();
    vLayout_tabrecord_2->setSpacing(6);
    vLayout_tabrecord_2->setObjectName(QString::fromUtf8("vLayout_tabrecord_2"));
    btn_tabrecord_outRecord = new QPushButton(tab_record);
    btn_tabrecord_outRecord->setObjectName(QString::fromUtf8("btn_tabrecord_outRecord"));
    btn_tabrecord_outRecord->setFixedSize(BTN_W,BTN_H);
    vLayout_tabrecord_2->addWidget(btn_tabrecord_outRecord);

    btn_tabrecord_clearRecord = new QPushButton(tab_record);
    btn_tabrecord_clearRecord->setObjectName(QString::fromUtf8("btn_tabrecord_clearRecord"));
    btn_tabrecord_clearRecord->setFixedSize(BTN_W,BTN_H);
    vLayout_tabrecord_2->addWidget(btn_tabrecord_clearRecord);


    hLayout_tabrecord_1->addLayout(vLayout_tabrecord_2);


    horizontalLayout->addLayout(hLayout_tabrecord_1);

    tabWidget->addTab(tab_record, QString());



    hLayout_main_12->addWidget(tabWidget);
    vLayout_main_1->addLayout(hLayout_main_12);
    vLayout_main_1->setStretch(0, 1);
    vLayout_main_1->setStretch(1, 9);

    verticalLayout_3->addLayout(vLayout_main_1);

    MainWindow->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(MainWindow);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 886, 31));
    MainWindow->setMenuBar(menuBar);
    statusBar = new QStatusBar(MainWindow);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    MainWindow->setStatusBar(statusBar);

    retranslateUi(MainWindow);
//    QMetaObject::connectSlotsByName(MainWindow);
}

void BaseWindow::retranslateUi(QMainWindow *MainWindow) {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
    label_main_logo->setText(QString());
    label_main_title->setText(QApplication::translate("MainWindow", "\346\231\272\350\203\275\344\272\272\346\234\272\344\272\244\344\272\222\345\271\263\345\217\260", nullptr));
    groupBox_tabmain_1->setTitle(QApplication::translate("MainWindow", "\347\263\273\347\273\237\347\212\266\346\200\201", nullptr));
    label_tabmain_rbservoStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\344\274\272\346\234\215\347\212\266\346\200\201", nullptr));
    label_tabmain_rbConnStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
    label_tabmain_rbservoStatusValue->setText(QString());
    label_tabmain_caConnStatus->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
    label_tabmain_caConnStatusValue->setText(QString());
    label_tabmain_rbConnStatusValue->setText(QString());
    label_tabmain_rbErrStatusValue->setText(QString());
    label_tabmain_rbErrStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\346\225\205\351\232\234\347\212\266\346\200\201", nullptr));
    label_tabmain_5fConnStatus->setText(QApplication::translate("MainWindow", "\344\272\224\346\214\207\345\244\271\347\210\252\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
    label_tabmain_5fConnStatusValue->setText(QString());
    label_tabmain_TsensorStatus->setText(QApplication::translate("MainWindow", "\345\205\255\350\275\264\344\274\240\346\204\237\345\231\250\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
    label_tabmain_TsensorStatusValue->setText(QString());
    label_13->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
    label_14->setText(QString());
    label_15->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
    label_16->setText(QString());
    groupBox_tabmain_2->setTitle(QApplication::translate("MainWindow", "\346\250\241\345\274\217\351\200\211\346\213\251", nullptr));
    cbox_tabmain_chooseMode->setItemText(0, QApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\350\277\220\350\241\214\346\250\241\345\274\217", nullptr));
    cbox_tabmain_chooseMode->setItemText(1, QApplication::translate("MainWindow", "\347\234\237\346\234\272\350\277\220\350\241\214", nullptr));
    cbox_tabmain_chooseMode->setItemText(2, QApplication::translate("MainWindow", "\344\273\277\347\234\237\350\277\220\350\241\214", nullptr));

    groupBox_tabmain_3->setTitle(QApplication::translate("MainWindow", "\347\263\273\347\273\237\345\212\237\350\203\275", nullptr));
    btn_tabmain_devConnOrRviz->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\350\277\236\346\216\245", nullptr));
    btn_tabmain_beginRun->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\350\277\220\350\241\214", nullptr));
    btn_tabmain_sysStop->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\345\201\234\346\255\242", nullptr));
    btn_tabmain_sysReset->setText(QApplication::translate("MainWindow", "\347\263\273\347\273\237\345\244\215\344\275\215", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_main), QApplication::translate("MainWindow", "\344\270\273\347\225\214\351\235\242", nullptr));
    groupBox_tabdebug_1->setTitle(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\350\260\203\350\257\225", nullptr));
    btn_rbSetEnable->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\344\270\212\344\275\277\350\203\275", nullptr));
    btn_rbReset->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\345\244\215\344\275\215", nullptr));
    groupBox_tabdebug_2->setTitle(QApplication::translate("MainWindow", "\344\272\224\346\214\207\345\244\271\347\210\252\346\211\213\345\236\213\350\260\203\350\257\225", nullptr));
    btn_gripper_open->setText(QApplication::translate("MainWindow", "\345\274\240\345\274\200", nullptr));
    btn_gripper_close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
    btn_gripper_OK_Pose->setText(QApplication::translate("MainWindow", "OK\346\211\213\345\236\213", nullptr));
    btn_gripper_Y_Pose->setText(QApplication::translate("MainWindow", "Y\346\211\213\345\236\213", nullptr));
    groupBox_tabdebug_3->setTitle(QApplication::translate("MainWindow", "\345\205\266\344\273\226\350\260\203\350\257\225", nullptr));
    btn_rbGoHomePose->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\345\233\236\345\216\237\347\202\271", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_debug), QApplication::translate("MainWindow", "\350\260\203\350\257\225\351\241\265\351\235\242", nullptr));
    groupBox_tabfunc_1->setTitle(QApplication::translate("MainWindow", "\345\233\276\345\203\217\346\230\276\347\244\272", nullptr));
    label_tabfunc_image->setText(QApplication::translate("MainWindow", "\345\233\276\347\211\207", nullptr));
    groupBox_tabfunc_2->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275\351\200\211\346\213\251", nullptr));
    btn_tabfunc_shakehand->setText(QApplication::translate("MainWindow", "\344\270\216\344\272\272\346\217\241\346\211\213", nullptr));
    btn_tabfunc_grepwawa->setText(QApplication::translate("MainWindow", "\346\212\223\345\250\203\345\250\203", nullptr));
    groupBox_tabfunc_3->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\347\273\237\350\256\241\344\270\216\350\257\255\351\237\263\346\230\276\347\244\272", nullptr));
    label_tabfunc_grepCount->setText(QApplication::translate("MainWindow", "\346\212\223\345\250\203\345\250\203\346\254\241\346\225\260:", nullptr));
    label_tabfunc_shakehand->setText(QApplication::translate("MainWindow", "\346\217\241\346\211\213\346\254\241\346\225\260:", nullptr));
    label_tabfunc_grepCountValue->setText(QApplication::translate("MainWindow", "0\346\254\241", nullptr));
    label_tabfunc_shakehandValue->setText(QApplication::translate("MainWindow", "0\346\254\241", nullptr));
    label_tabfunc_voice->setText(QApplication::translate("MainWindow", "\350\257\255\351\237\263\350\257\206\345\210\253\347\273\223\346\236\234:", nullptr));
    label_tabfunc_voiceValue->setText(QString());
    tabWidget->setTabText(tabWidget->indexOf(tab_func), QApplication::translate("MainWindow", "\345\212\237\350\203\275\351\241\265\351\235\242", nullptr));
    btn_tabrecord_outRecord->setText(QApplication::translate("MainWindow", "\346\227\245\345\277\227\345\257\274\345\207\272", nullptr));
    btn_tabrecord_clearRecord->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\346\227\245\345\277\227", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_record), QApplication::translate("MainWindow", "\346\227\245\345\277\227\347\225\214\351\235\242", nullptr));
}
