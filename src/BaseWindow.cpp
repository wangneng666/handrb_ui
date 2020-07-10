
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

    pixmap_voicesleep=QPixmap(photoPath+"icon/voice_sleep.svg");
    pixmap_voicesleep = pixmap_voicesleep.scaled(300,300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    pixmap_voiceAwake=QPixmap(photoPath+"icon/voice_awake.svg");
    pixmap_voiceAwake = pixmap_voiceAwake.scaled(300,300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
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

    label_tabmain_impedenceConnStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_impedenceConnStatus->setObjectName(QString::fromUtf8("label_tabmain_impedenceConnStatus"));
    label_tabmain_impedenceConnStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_impedenceConnStatus->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_tabmain_impedenceConnStatus, 1, 5, 1, 1);

    label_tabmain_impedenceConnStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_impedenceConnStatusValue->setObjectName(QString::fromUtf8("label_tabmain_impedenceConnStatusValue"));
    label_tabmain_impedenceConnStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_impedenceConnStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_impedenceConnStatusValue, 1, 6, 1, 1);

    label_tabmain_rbBusyStatus = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbBusyStatus->setObjectName(QString::fromUtf8("label_tabmain_rbBusyStatus"));
    label_tabmain_rbBusyStatus->setMaximumSize(QSize(150, 50));
    label_tabmain_rbBusyStatus->setLayoutDirection(Qt::RightToLeft);

    gLayout_tabmain_1->addWidget(label_tabmain_rbBusyStatus, 1, 7, 1, 1);

    label_tabmain_rbBusyStatusValue = new QLabel(groupBox_tabmain_1);
    label_tabmain_rbBusyStatusValue->setObjectName(QString::fromUtf8("label_tabmain_rbBusyStatusValue"));
    label_tabmain_rbBusyStatusValue->setMaximumSize(QSize(150, 50));
    label_tabmain_rbBusyStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabmain_1->addWidget(label_tabmain_rbBusyStatusValue, 1, 8, 1, 1);


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

    /*****************************************************/
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

    btn_startSensor = new QPushButton(groupBox_tabdebug_3);
    btn_startSensor->setObjectName(QString::fromUtf8("btn_startSensor"));
    btn_startSensor->setMaximumSize(QSize(150, 50));
    btn_startSensor->setFixedSize(BTN_W,BTN_H);
    btn_startSensor->setText("开启传感器");
    horizontalLayout_20->addWidget(btn_startSensor);


    verticalLayout_5->addWidget(groupBox_tabdebug_3);

    tabWidget->addTab(tab_debug, QString());

    /*****************************************************/
    tab_shakeHand = new QWidget();
    tab_shakeHand->setObjectName(QString::fromUtf8("tab_shakeHand"));
    verticalLayout_6 = new QVBoxLayout(tab_shakeHand);
    verticalLayout_6->setSpacing(6);
    verticalLayout_6->setContentsMargins(11, 11, 11, 11);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    gBox_tabShakeHand_mode = new QGroupBox(tab_shakeHand);
    gBox_tabShakeHand_mode->setObjectName(QString::fromUtf8("gBox_tabShakeHand_mode"));
    gBox_tabShakeHand_mode->setStyleSheet(groupBox_qss);
    horizontalLayout_3 = new QHBoxLayout(gBox_tabShakeHand_mode);
    horizontalLayout_3->setSpacing(6);
    horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    cBox_tabShakeHand_setMode = new QComboBox(gBox_tabShakeHand_mode);
    cBox_tabShakeHand_setMode->addItem(QString());
    cBox_tabShakeHand_setMode->addItem(QString());
    cBox_tabShakeHand_setMode->setObjectName(QString::fromUtf8("cBox_tabShakeHand_setMode"));
    cBox_tabShakeHand_setMode->setMaximumSize(QSize(200, 50));

    horizontalLayout_3->addWidget(cBox_tabShakeHand_setMode);


    verticalLayout->addWidget(gBox_tabShakeHand_mode);

    gBox_tabShakeHand_status = new QGroupBox(tab_shakeHand);
    gBox_tabShakeHand_status->setObjectName(QString::fromUtf8("gBox_tabShakeHand_status"));
    gBox_tabShakeHand_status->setStyleSheet(groupBox_qss);
    horizontalLayout_2 = new QHBoxLayout(gBox_tabShakeHand_status);
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    gLayout_tabShakeHand = new QGridLayout();
    gLayout_tabShakeHand->setSpacing(6);
    gLayout_tabShakeHand->setObjectName(QString::fromUtf8("gLayout_tabShakeHand"));
    label_tabShakeHand_impStatusValue = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_impStatusValue->setObjectName(QString::fromUtf8("label_tabShakeHand_impStatusValue"));
    label_tabShakeHand_impStatusValue->setMaximumSize(QSize(150, 50));
    label_tabShakeHand_impStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_impStatusValue, 2, 1, 1, 1);

    label_tabShakeHand_voiceStatusValue = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_voiceStatusValue->setObjectName(QString::fromUtf8("label_tabShakeHand_voiceStatusValue"));
    label_tabShakeHand_voiceStatusValue->setMaximumSize(QSize(150, 50));
    label_tabShakeHand_voiceStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_voiceStatusValue, 3, 1, 1, 1);

    label_tabShakeHand_rbStatusValue = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_rbStatusValue->setObjectName(QString::fromUtf8("label_tabShakeHand_rbStatusValue"));
    label_tabShakeHand_rbStatusValue->setMaximumSize(QSize(150, 50));
    label_tabShakeHand_rbStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_rbStatusValue, 0, 1, 1, 1);

    label_tabShakeHand_voiceStatus = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_voiceStatus->setObjectName(QString::fromUtf8("label_tabShakeHand_voiceStatus"));
    label_tabShakeHand_voiceStatus->setMaximumSize(QSize(150, 50));

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_voiceStatus, 3, 0, 1, 1);

    label_tabShakeHand_rbCtlStatusValue = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_rbCtlStatusValue->setObjectName(QString::fromUtf8("label_tabShakeHand_rbCtlStatusValue"));
    label_tabShakeHand_rbCtlStatusValue->setMaximumSize(QSize(150, 50));
    label_tabShakeHand_rbCtlStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_rbCtlStatusValue, 1, 1, 1, 1);

    label_tabShakeHand_rbCtlStatus = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_rbCtlStatus->setObjectName(QString::fromUtf8("label_tabShakeHand_rbCtlStatus"));
    label_tabShakeHand_rbCtlStatus->setMaximumSize(QSize(150, 50));

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_rbCtlStatus, 1, 0, 1, 1);

    label_tabShakeHand_rbStatus = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_rbStatus->setObjectName(QString::fromUtf8("label_tabShakeHand_rbStatus"));
    label_tabShakeHand_rbStatus->setMaximumSize(QSize(150, 50));

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_rbStatus, 0, 0, 1, 1);

    label_tabShakeHand_impStatus = new QLabel(gBox_tabShakeHand_status);
    label_tabShakeHand_impStatus->setObjectName(QString::fromUtf8("label_tabShakeHand_impStatus"));
    label_tabShakeHand_impStatus->setMaximumSize(QSize(150, 50));

    gLayout_tabShakeHand->addWidget(label_tabShakeHand_impStatus, 2, 0, 1, 1);

    btn_tabShakeHand_startRobRun = new QPushButton(gBox_tabShakeHand_status);
    btn_tabShakeHand_startRobRun->setObjectName(QString::fromUtf8("btn_tabShakeHand_startRobRun"));
    btn_tabShakeHand_startRobRun->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_startRobRun->setFixedSize(BTN_W,BTN_H);

    gLayout_tabShakeHand->addWidget(btn_tabShakeHand_startRobRun, 0, 2, 1, 1);

    btn_tabShakeHand_startRobCtl = new QPushButton(gBox_tabShakeHand_status);
    btn_tabShakeHand_startRobCtl->setObjectName(QString::fromUtf8("btn_tabShakeHand_startRobCtl"));
    btn_tabShakeHand_startRobCtl->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_startRobCtl->setFixedSize(BTN_W,BTN_H);

    gLayout_tabShakeHand->addWidget(btn_tabShakeHand_startRobCtl, 1, 2, 1, 1);

    btn_tabShakeHand_startimpedence = new QPushButton(gBox_tabShakeHand_status);
    btn_tabShakeHand_startimpedence->setObjectName(QString::fromUtf8("btn_tabShakeHand_startimpedence"));
    btn_tabShakeHand_startimpedence->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_startimpedence->setFixedSize(BTN_W,BTN_H);

    gLayout_tabShakeHand->addWidget(btn_tabShakeHand_startimpedence, 2, 2, 1, 1);

    btn_tabShakeHand_startvoice = new QPushButton(gBox_tabShakeHand_status);
    btn_tabShakeHand_startvoice->setObjectName(QString::fromUtf8("btn_tabShakeHand_startvoice"));
    btn_tabShakeHand_startvoice->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_startvoice->setFixedSize(BTN_W,BTN_H);

    gLayout_tabShakeHand->addWidget(btn_tabShakeHand_startvoice, 3, 2, 1, 1);


    horizontalLayout_2->addLayout(gLayout_tabShakeHand);


    verticalLayout->addWidget(gBox_tabShakeHand_status);


    gBox_tabShakeHand_func = new QGroupBox(tab_shakeHand);
    gBox_tabShakeHand_func->setObjectName(QString::fromUtf8("gBox_tabShakeHand_func"));
    gBox_tabShakeHand_func->setStyleSheet(groupBox_qss);
    horizontalLayout_4 = new QHBoxLayout(gBox_tabShakeHand_func);
    horizontalLayout_4->setSpacing(6);
    horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    btn_tabShakeHand_begin = new QPushButton(gBox_tabShakeHand_func);
    btn_tabShakeHand_begin->setObjectName(QString::fromUtf8("btn_tabShakeHand_begin"));
    btn_tabShakeHand_begin->setMaximumSize(QSize(150, 50));

    horizontalLayout_4->addWidget(btn_tabShakeHand_begin);

    btn_tabShakeHand_shakeHandEnd = new QPushButton(gBox_tabShakeHand_func);
    btn_tabShakeHand_shakeHandEnd->setObjectName(QString::fromUtf8("btn_tabShakeHand_shakeHandEnd"));
    btn_tabShakeHand_shakeHandEnd->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_shakeHandEnd->setText("握手结束");
    btn_tabShakeHand_shakeHandEnd->setFixedSize(BTN_W,BTN_H);
    horizontalLayout_4->addWidget(btn_tabShakeHand_shakeHandEnd);

    btn_tabShakeHand_AutoRun = new QPushButton(gBox_tabShakeHand_func);
    btn_tabShakeHand_AutoRun->setObjectName(QString::fromUtf8("btn_tabShakeHand_AutoRun"));
    btn_tabShakeHand_AutoRun->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_AutoRun->setFixedSize(BTN_W,BTN_H);
    btn_tabShakeHand_AutoRun->setText("一键启动握手");
    horizontalLayout_4->addWidget(btn_tabShakeHand_AutoRun);

    btn_tabShakeHand_stop = new QPushButton(gBox_tabShakeHand_func);
    btn_tabShakeHand_stop->setObjectName(QString::fromUtf8("btn_tabShakeHand_stop"));
    btn_tabShakeHand_stop->setMaximumSize(QSize(150, 50));

    horizontalLayout_4->addWidget(btn_tabShakeHand_stop);

    btn_tabShakeHand_close = new QPushButton(gBox_tabShakeHand_func);
    btn_tabShakeHand_close->setObjectName(QString::fromUtf8("btn_tabShakeHand_close"));
    btn_tabShakeHand_close->setMaximumSize(QSize(150, 50));
    btn_tabShakeHand_close->setFixedSize(BTN_W,BTN_H);

    horizontalLayout_4->addWidget(btn_tabShakeHand_close);


    verticalLayout->addWidget(gBox_tabShakeHand_func);

    verticalLayout->setStretch(0, 2);
    verticalLayout->setStretch(1, 1);
    verticalLayout->setStretch(2, 2);

    verticalLayout_6->addLayout(verticalLayout);

    tabWidget->addTab(tab_shakeHand, QString());
    /*****************************************************/
    tab_grabToy = new QWidget();
    tab_grabToy->setObjectName(QString::fromUtf8("tab_grabToy"));
    horizontalLayout_13 = new QHBoxLayout(tab_grabToy);
    horizontalLayout_13->setSpacing(6);
    horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
    hLayout_tab_grabToy = new QHBoxLayout();
    hLayout_tab_grabToy->setSpacing(6);
    hLayout_tab_grabToy->setObjectName(QString::fromUtf8("hLayout_tab_grabToy"));
    vLayout_tab_grabToy_1 = new QVBoxLayout();
    vLayout_tab_grabToy_1->setSpacing(6);
    vLayout_tab_grabToy_1->setObjectName(QString::fromUtf8("vLayout_tab_grabToy_1"));
    groupBox = new QGroupBox(tab_grabToy);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setStyleSheet(groupBox_qss);
    verticalLayout_8 = new QVBoxLayout(groupBox);
    verticalLayout_8->setSpacing(6);
    verticalLayout_8->setContentsMargins(11, 11, 11, 11);
    verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
    lable_tabgrabToy_showImg = new QLabel(groupBox);
    lable_tabgrabToy_showImg->setObjectName(QString::fromUtf8("lable_tabgrabToy_showImg"));

    verticalLayout_8->addWidget(lable_tabgrabToy_showImg);


    vLayout_tab_grabToy_1->addWidget(groupBox);


    hLayout_tab_grabToy->addLayout(vLayout_tab_grabToy_1);

    vLayout_tab_grabToy_2 = new QVBoxLayout();
    vLayout_tab_grabToy_2->setSpacing(6);
    vLayout_tab_grabToy_2->setObjectName(QString::fromUtf8("vLayout_tab_grabToy_2"));
    gBox_tab_grabToy_1 = new QGroupBox(tab_grabToy);
    gBox_tab_grabToy_1->setObjectName(QString::fromUtf8("gBox_tab_grabToy_1"));
    gBox_tab_grabToy_1->setStyleSheet(groupBox_qss);
    horizontalLayout_17 = new QHBoxLayout(gBox_tab_grabToy_1);
    horizontalLayout_17->setSpacing(6);
    horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
    gLayout_tab_grabToy = new QGridLayout();
    gLayout_tab_grabToy->setSpacing(6);
    gLayout_tab_grabToy->setObjectName(QString::fromUtf8("gLayout_tab_grabToy"));
    label_tabgrabToy_voiceStatusValue = new QLabel(gBox_tab_grabToy_1);
    label_tabgrabToy_voiceStatusValue->setObjectName(QString::fromUtf8("label_tabgrabToy_voiceStatusValue"));
    label_tabgrabToy_voiceStatusValue->setMaximumSize(QSize(150, 50));
    label_tabgrabToy_voiceStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tab_grabToy->addWidget(label_tabgrabToy_voiceStatusValue, 2, 1, 1, 1);

    label_tabgrabToy_rbStatusValue = new QLabel(gBox_tab_grabToy_1);
    label_tabgrabToy_rbStatusValue->setObjectName(QString::fromUtf8("label_tabgrabToy_rbStatusValue"));
    label_tabgrabToy_rbStatusValue->setMaximumSize(QSize(150, 50));
    label_tabgrabToy_rbStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tab_grabToy->addWidget(label_tabgrabToy_rbStatusValue, 0, 1, 1, 1);

    label_tab_grabToy_rbCtlStatus = new QLabel(gBox_tab_grabToy_1);
    label_tab_grabToy_rbCtlStatus->setObjectName(QString::fromUtf8("label_tab_grabToy_rbCtlStatus"));
    label_tab_grabToy_rbCtlStatus->setMaximumSize(QSize(150, 50));

    gLayout_tab_grabToy->addWidget(label_tab_grabToy_rbCtlStatus, 1, 0, 1, 1);

    label_tabgrabToy_voiceStatus = new QLabel(gBox_tab_grabToy_1);
    label_tabgrabToy_voiceStatus->setObjectName(QString::fromUtf8("label_tabgrabToy_voiceStatus"));
    label_tabgrabToy_voiceStatus->setMaximumSize(QSize(150, 50));

    gLayout_tab_grabToy->addWidget(label_tabgrabToy_voiceStatus, 2, 0, 1, 1);

    label_tab_grabToy_rbCtlStatusValue = new QLabel(gBox_tab_grabToy_1);
    label_tab_grabToy_rbCtlStatusValue->setObjectName(QString::fromUtf8("label_tab_grabToy_rbCtlStatusValue"));
    label_tab_grabToy_rbCtlStatusValue->setMaximumSize(QSize(150, 50));
    label_tab_grabToy_rbCtlStatusValue->setPixmap(fitpixmap_redLight);

    gLayout_tab_grabToy->addWidget(label_tab_grabToy_rbCtlStatusValue, 1, 1, 1, 1);

    label_tabgrabToy_rbStatus = new QLabel(gBox_tab_grabToy_1);
    label_tabgrabToy_rbStatus->setObjectName(QString::fromUtf8("label_tabgrabToy_rbStatus"));
    label_tabgrabToy_rbStatus->setMaximumSize(QSize(150, 50));

    gLayout_tab_grabToy->addWidget(label_tabgrabToy_rbStatus, 0, 0, 1, 1);

    btn_tabgrabToy_startRobRun = new QPushButton(gBox_tab_grabToy_1);
    btn_tabgrabToy_startRobRun->setObjectName(QString::fromUtf8("btn_tabgrabToy_startRobRun"));
    btn_tabgrabToy_startRobRun->setMaximumSize(QSize(150, 50));
    btn_tabgrabToy_startRobRun->setFixedSize(BTN_W,BTN_H);

    gLayout_tab_grabToy->addWidget(btn_tabgrabToy_startRobRun, 0, 2, 1, 1);

    btn_tabgrabToy_startRobCtl = new QPushButton(gBox_tab_grabToy_1);
    btn_tabgrabToy_startRobCtl->setObjectName(QString::fromUtf8("btn_tabgrabToy_startRobCtl"));
    btn_tabgrabToy_startRobCtl->setMaximumSize(QSize(150, 50));
    btn_tabgrabToy_startRobCtl->setFixedSize(BTN_W,BTN_H);

    gLayout_tab_grabToy->addWidget(btn_tabgrabToy_startRobCtl, 1, 2, 1, 1);

    btn_tabgrabToy_startvoice = new QPushButton(gBox_tab_grabToy_1);
    btn_tabgrabToy_startvoice->setObjectName(QString::fromUtf8("btn_tabgrabToy_startvoice"));
    btn_tabgrabToy_startvoice->setMaximumSize(QSize(150, 50));
    btn_tabgrabToy_startvoice->setFixedSize(BTN_W,BTN_H);

    gLayout_tab_grabToy->addWidget(btn_tabgrabToy_startvoice, 2, 2, 1, 1);


    horizontalLayout_17->addLayout(gLayout_tab_grabToy);


    vLayout_tab_grabToy_2->addWidget(gBox_tab_grabToy_1);

    gBox_tab_grabToy_2 = new QGroupBox(tab_grabToy);
    gBox_tab_grabToy_2->setObjectName(QString::fromUtf8("gBox_tab_grabToy_2"));
    gBox_tab_grabToy_2->setStyleSheet(groupBox_qss);
    horizontalLayout_18 = new QHBoxLayout(gBox_tab_grabToy_2);
    horizontalLayout_18->setSpacing(6);
    horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
    cBox_tab_grabToy = new QComboBox(gBox_tab_grabToy_2);
    cBox_tab_grabToy->addItem(QString());
    cBox_tab_grabToy->addItem(QString());
    cBox_tab_grabToy->setObjectName(QString::fromUtf8("cBox_tab_grabToy"));
    cBox_tab_grabToy->setMaximumSize(QSize(200, 50));

    horizontalLayout_18->addWidget(cBox_tab_grabToy);


    vLayout_tab_grabToy_2->addWidget(gBox_tab_grabToy_2);

    gBox_tab_grabToy_3 = new QGroupBox(tab_grabToy);
    gBox_tab_grabToy_3->setObjectName(QString::fromUtf8("gBox_tab_grabToy_3"));
    gBox_tab_grabToy_3->setStyleSheet(groupBox_qss);
    horizontalLayout_16 = new QHBoxLayout(gBox_tab_grabToy_3);
    horizontalLayout_16->setSpacing(6);
    horizontalLayout_16->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
    hLayout_tab_grabToy_3 = new QHBoxLayout();
    hLayout_tab_grabToy_3->setSpacing(6);
    hLayout_tab_grabToy_3->setObjectName(QString::fromUtf8("hLayout_tab_grabToy_3"));
    btn_tab_grabToy_detect = new QPushButton(gBox_tab_grabToy_3);
    btn_tab_grabToy_detect->setObjectName(QString::fromUtf8("btn_tab_grabToy_detect"));
    btn_tab_grabToy_detect->setMaximumSize(QSize(150, 50));
    btn_tab_grabToy_detect->setFixedSize(BTN_W,BTN_H);
    hLayout_tab_grabToy_3->addWidget(btn_tab_grabToy_detect);

    btn_tab_grabToy_run = new QPushButton(gBox_tab_grabToy_3);
    btn_tab_grabToy_run->setObjectName(QString::fromUtf8("btn_tab_grabToy_run"));
    btn_tab_grabToy_run->setMaximumSize(QSize(150, 50));
    btn_tab_grabToy_run->setFixedSize(BTN_W,BTN_H);

    hLayout_tab_grabToy_3->addWidget(btn_tab_grabToy_run);

    btn_tab_grabToy_close = new QPushButton(gBox_tab_grabToy_3);
    btn_tab_grabToy_close->setObjectName(QString::fromUtf8("btn_tab_grabToy_close"));
    btn_tab_grabToy_close->setMaximumSize(QSize(150, 50));
    btn_tab_grabToy_close->setFixedSize(BTN_W,BTN_H);

    hLayout_tab_grabToy_3->addWidget(btn_tab_grabToy_close);


    horizontalLayout_16->addLayout(hLayout_tab_grabToy_3);


    vLayout_tab_grabToy_2->addWidget(gBox_tab_grabToy_3);


    hLayout_tab_grabToy->addLayout(vLayout_tab_grabToy_2);


    horizontalLayout_13->addLayout(hLayout_tab_grabToy);

    tabWidget->addTab(tab_grabToy, QString());
    /*****************************************************/
    tab_personDetect = new QWidget();
    tab_personDetect->setObjectName(QString::fromUtf8("tab_personDetect"));
    horizontalLayout_12 = new QHBoxLayout(tab_personDetect);
    horizontalLayout_12->setSpacing(6);
    horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
    hLayout_tab_personDetect_1 = new QHBoxLayout();
    hLayout_tab_personDetect_1->setSpacing(6);
    hLayout_tab_personDetect_1->setObjectName(QString::fromUtf8("hLayout_tab_personDetect_1"));
    vLayout_tab_personDetect_1 = new QVBoxLayout();
    vLayout_tab_personDetect_1->setSpacing(6);
    vLayout_tab_personDetect_1->setObjectName(QString::fromUtf8("vLayout_tab_personDetect_1"));
    gBox_tab_personDetect_1 = new QGroupBox(tab_personDetect);
    gBox_tab_personDetect_1->setObjectName(QString::fromUtf8("gBox_tab_personDetect_1"));
    gBox_tab_personDetect_1->setStyleSheet(groupBox_qss);
    horizontalLayout_15 = new QHBoxLayout(gBox_tab_personDetect_1);
    horizontalLayout_15->setSpacing(6);
    horizontalLayout_15->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
    label_tab_personDetect_showImag = new QLabel(gBox_tab_personDetect_1);
    label_tab_personDetect_showImag->setObjectName(QString::fromUtf8("label_tab_personDetect_showImag"));

    horizontalLayout_15->addWidget(label_tab_personDetect_showImag);


    vLayout_tab_personDetect_1->addWidget(gBox_tab_personDetect_1);


    hLayout_tab_personDetect_1->addLayout(vLayout_tab_personDetect_1);

    vLayout_tab_personDetect_2 = new QVBoxLayout();
    vLayout_tab_personDetect_2->setSpacing(6);
    vLayout_tab_personDetect_2->setObjectName(QString::fromUtf8("vLayout_tab_personDetect_2"));
    gBox_tab_personDetect_2 = new QGroupBox(tab_personDetect);
    gBox_tab_personDetect_2->setObjectName(QString::fromUtf8("gBox_tab_personDetect_2"));
    gBox_tab_personDetect_2->setStyleSheet(groupBox_qss);
    verticalLayout_11 = new QVBoxLayout(gBox_tab_personDetect_2);
    verticalLayout_11->setSpacing(6);
    verticalLayout_11->setContentsMargins(11, 11, 11, 11);
    verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
    hLayout_tab_personDetect_21 = new QHBoxLayout();
    hLayout_tab_personDetect_21->setSpacing(6);
    hLayout_tab_personDetect_21->setObjectName(QString::fromUtf8("hLayout_tab_personDetect_21"));
    btn_tab_personDetect_openPersonDetect = new QPushButton(gBox_tab_personDetect_2);
    btn_tab_personDetect_openPersonDetect->setObjectName(QString::fromUtf8("btn_tab_personDetect_openPersonDetect"));
    btn_tab_personDetect_openPersonDetect->setMaximumSize(QSize(150, 50));

    hLayout_tab_personDetect_21->addWidget(btn_tab_personDetect_openPersonDetect);


    verticalLayout_11->addLayout(hLayout_tab_personDetect_21);


    vLayout_tab_personDetect_2->addWidget(gBox_tab_personDetect_2);


    hLayout_tab_personDetect_1->addLayout(vLayout_tab_personDetect_2);


    horizontalLayout_12->addLayout(hLayout_tab_personDetect_1);

    tabWidget->addTab(tab_personDetect, QString());


    //语音界面
    tab_voiceDetect = new QWidget();
    tab_voiceDetect->setObjectName(QString::fromUtf8("tab_voiceDetect"));
    horizontalLayout_10 = new QHBoxLayout(tab_voiceDetect);
    horizontalLayout_10->setSpacing(6);
    horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
    hLayout_tab_voiceDetect = new QHBoxLayout();
    hLayout_tab_voiceDetect->setSpacing(6);
    hLayout_tab_voiceDetect->setObjectName(QString::fromUtf8("hLayout_tab_voiceDetect"));
    vLayout_tab_voiceDetect_2 = new QVBoxLayout();
    vLayout_tab_voiceDetect_2->setSpacing(6);
    vLayout_tab_voiceDetect_2->setObjectName(QString::fromUtf8("vLayout_tab_voiceDetect_2"));
    gBox_tab_voiceDetect_voiceCtl = new QGroupBox(tab_voiceDetect);
    gBox_tab_voiceDetect_voiceCtl->setObjectName(QString::fromUtf8("gBox_tab_voiceDetect_voiceCtl"));
    gBox_tab_voiceDetect_voiceCtl->setStyleSheet(groupBox_qss);
    verticalLayout_9 = new QVBoxLayout(gBox_tab_voiceDetect_voiceCtl);
    verticalLayout_9->setSpacing(6);
    verticalLayout_9->setContentsMargins(11, 11, 11, 11);
    verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
    hLayout_tab_voiceDetect_ctl1 = new QHBoxLayout();
    hLayout_tab_voiceDetect_ctl1->setSpacing(6);
    hLayout_tab_voiceDetect_ctl1->setObjectName(QString::fromUtf8("hLayout_tab_voiceDetect_ctl1"));
    lable_tab_voiceDetect_showImg = new QLabel(gBox_tab_voiceDetect_voiceCtl);
    lable_tab_voiceDetect_showImg->setObjectName(QString::fromUtf8("lable_tab_voiceDetect_showImg"));
    lable_tab_voiceDetect_showImg->setFixedSize(300,300);
    lable_tab_voiceDetect_showImg->setPixmap(pixmap_voicesleep);
    hLayout_tab_voiceDetect_ctl1->addWidget(lable_tab_voiceDetect_showImg,0,Qt::AlignHCenter);


    verticalLayout_9->addLayout(hLayout_tab_voiceDetect_ctl1);

    hLayout_tab_voiceDetect_ctl2 = new QHBoxLayout();
    hLayout_tab_voiceDetect_ctl2->setSpacing(6);
    hLayout_tab_voiceDetect_ctl2->setObjectName(QString::fromUtf8("hLayout_tab_voiceDetect_ctl2"));
    btn_tab_voiceDetect_run = new QPushButton(gBox_tab_voiceDetect_voiceCtl);
    btn_tab_voiceDetect_run->setObjectName(QString::fromUtf8("btn_tab_voiceDetect_run"));
//    btn_tab_voiceDetect_run->setMaximumSize(QSize(200, 100));
    btn_tab_voiceDetect_run->setFixedSize(200,100);

    hLayout_tab_voiceDetect_ctl2->addWidget(btn_tab_voiceDetect_run);


    verticalLayout_9->addLayout(hLayout_tab_voiceDetect_ctl2);


    vLayout_tab_voiceDetect_2->addWidget(gBox_tab_voiceDetect_voiceCtl);


    hLayout_tab_voiceDetect->addLayout(vLayout_tab_voiceDetect_2);

    vLayout_tab_voiceDetect_1 = new QVBoxLayout();
    vLayout_tab_voiceDetect_1->setSpacing(6);
    vLayout_tab_voiceDetect_1->setObjectName(QString::fromUtf8("vLayout_tab_voiceDetect_1"));
    gBox_tab_voiceDetect_voiceInfo = new QGroupBox(tab_voiceDetect);
    gBox_tab_voiceDetect_voiceInfo->setObjectName(QString::fromUtf8("gBox_tab_voiceDetect_voiceInfo"));
    gBox_tab_voiceDetect_voiceInfo->setStyleSheet(groupBox_qss);
    horizontalLayout_11 = new QHBoxLayout(gBox_tab_voiceDetect_voiceInfo);
    horizontalLayout_11->setSpacing(6);
    horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
    plainTextEdit = new QPlainTextEdit(gBox_tab_voiceDetect_voiceInfo);
    plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

    horizontalLayout_11->addWidget(plainTextEdit);


    vLayout_tab_voiceDetect_1->addWidget(gBox_tab_voiceDetect_voiceInfo);


    hLayout_tab_voiceDetect->addLayout(vLayout_tab_voiceDetect_1);

    hLayout_tab_voiceDetect->setStretch(0, 1);
    hLayout_tab_voiceDetect->setStretch(1, 1);

    horizontalLayout_10->addLayout(hLayout_tab_voiceDetect);

    tabWidget->addTab(tab_voiceDetect, QString());
    /*****************************************************/

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

    //
    label_tabShakeHand_voiceStatus->setVisible(false);
    label_tabShakeHand_voiceStatusValue->setVisible(false);
    btn_tabShakeHand_startvoice->setVisible(false);

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
    label_tabmain_5fConnStatus->setText(QApplication::translate("MainWindow", "五指爪连接状态", nullptr));
    label_tabmain_5fConnStatusValue->setText(QString());
    label_tabmain_TsensorStatus->setText(QApplication::translate("MainWindow", "传感器连接状态", nullptr));
    label_tabmain_TsensorStatusValue->setText(QString());
    label_tabmain_impedenceConnStatus->setText(QApplication::translate("MainWindow", "\351\230\273\346\212\227\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
    label_tabmain_impedenceConnStatusValue->setText(QString());
    label_tabmain_rbBusyStatus->setText(QApplication::translate("MainWindow", "机器人繁忙状态", nullptr));
    label_tabmain_rbBusyStatusValue->setText(QString());
    groupBox_tabmain_2->setTitle(QApplication::translate("MainWindow", "\346\250\241\345\274\217\351\200\211\346\213\251", nullptr));
    cbox_tabmain_chooseMode->setItemText(0, QApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\350\277\220\350\241\214\346\250\241\345\274\217", nullptr));
    cbox_tabmain_chooseMode->setItemText(1, QApplication::translate("MainWindow", "\347\234\237\346\234\272\350\277\220\350\241\214", nullptr));
    cbox_tabmain_chooseMode->setItemText(2, QApplication::translate("MainWindow", "\344\273\277\347\234\237\350\277\220\350\241\214", nullptr));

    groupBox_tabmain_3->setTitle(QApplication::translate("MainWindow", "\347\263\273\347\273\237\345\212\237\350\203\275", nullptr));
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
    tabWidget->setTabText(tabWidget->indexOf(tab_shakeHand), QApplication::translate("MainWindow", "\346\217\241\346\211\213\344\272\244\344\272\222\347\225\214\351\235\242", nullptr));

    gBox_tabShakeHand_status->setTitle(QApplication::translate("MainWindow", "\345\207\206\345\244\207\347\212\266\346\200\201", nullptr));
    label_tabShakeHand_impStatusValue->setText(QString());
    label_tabShakeHand_voiceStatusValue->setText(QString());
    label_tabShakeHand_rbStatusValue->setText(QString());
    label_tabShakeHand_voiceStatus->setText(QApplication::translate("MainWindow", "\345\243\260\351\237\263\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
    label_tabShakeHand_rbCtlStatusValue->setText(QString());
    label_tabShakeHand_rbCtlStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\346\216\247\345\210\266\346\250\241\345\235\227\345\207\206\345\244\207\347\212\266\346\200\201", nullptr));
    label_tabShakeHand_rbStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\350\277\220\350\241\214\345\207\206\345\244\207\347\212\266\346\200\201", nullptr));
    label_tabShakeHand_impStatus->setText(QApplication::translate("MainWindow", "\351\230\273\346\212\227\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
    btn_tabShakeHand_startRobRun->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    btn_tabShakeHand_startRobCtl->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    btn_tabShakeHand_startimpedence->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    btn_tabShakeHand_startvoice->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    gBox_tabShakeHand_mode->setTitle(QApplication::translate("MainWindow", "\346\250\241\345\274\217\351\200\211\346\213\251", nullptr));
    cBox_tabShakeHand_setMode->setItemText(0, QApplication::translate("MainWindow", "\347\233\264\346\216\245\345\224\244\351\206\222", nullptr));
    cBox_tabShakeHand_setMode->setItemText(1, QApplication::translate("MainWindow", "\350\257\255\351\237\263\345\224\244\351\206\222", nullptr));

    gBox_tabShakeHand_func->setTitle(QApplication::translate("MainWindow", "\350\277\220\350\241\214\345\212\237\350\203\275", nullptr));
    btn_tabShakeHand_begin->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250", nullptr));
    btn_tabShakeHand_stop->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242", nullptr));
    btn_tabShakeHand_close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_shakeHand), QApplication::translate("MainWindow", "\346\217\241\346\211\213\344\272\244\344\272\222\347\225\214\351\235\242", nullptr));


    groupBox->setTitle(QApplication::translate("MainWindow", "\345\233\276\345\203\217\346\230\276\347\244\272", nullptr));
    lable_tabgrabToy_showImg->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
    gBox_tab_grabToy_1->setTitle(QApplication::translate("MainWindow", "\345\207\206\345\244\207\347\212\266\346\200\201", nullptr));
    label_tabgrabToy_voiceStatusValue->setText(QString());
    label_tabgrabToy_rbStatusValue->setText(QString());
    label_tab_grabToy_rbCtlStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\346\216\247\345\210\266\346\250\241\345\235\227\345\207\206\345\244\207\347\212\266\346\200\201", nullptr));
    label_tabgrabToy_voiceStatus->setText(QApplication::translate("MainWindow", "\345\243\260\351\237\263\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
    label_tab_grabToy_rbCtlStatusValue->setText(QString());
    label_tabgrabToy_rbStatus->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\350\277\220\350\241\214\345\207\206\345\244\207\347\212\266\346\200\201", nullptr));
    btn_tabgrabToy_startRobRun->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    btn_tabgrabToy_startRobCtl->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    btn_tabgrabToy_startvoice->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257", nullptr));
    gBox_tab_grabToy_2->setTitle(QApplication::translate("MainWindow", "\346\250\241\345\274\217\351\200\211\346\213\251", nullptr));
    cBox_tab_grabToy->setItemText(0, QApplication::translate("MainWindow", "\347\233\264\346\216\245\350\247\246\345\217\221", nullptr));
    cBox_tab_grabToy->setItemText(1, QApplication::translate("MainWindow", "\345\243\260\346\216\247\350\247\246\345\217\221", nullptr));

    gBox_tab_grabToy_3->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275\346\214\211\351\222\256", nullptr));
    btn_tab_grabToy_detect->setText(QApplication::translate("MainWindow", "识别检测", nullptr));
    btn_tab_grabToy_run->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242", nullptr));
    btn_tab_grabToy_close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_grabToy), QApplication::translate("MainWindow", "\346\212\223\345\250\203\345\250\203\347\225\214\351\235\242", nullptr));

    gBox_tab_personDetect_1->setTitle(QApplication::translate("MainWindow", "\345\233\276\345\203\217\346\230\276\347\244\272", nullptr));
    label_tab_personDetect_showImag->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217", nullptr));
    gBox_tab_personDetect_2->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275\346\211\247\350\241\214", nullptr));
    btn_tab_personDetect_openPersonDetect->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\241\214\344\272\272\346\243\200\346\265\213", nullptr));

    tabWidget->setTabText(tabWidget->indexOf(tab_personDetect), QApplication::translate("MainWindow", "\350\241\214\344\272\272\346\243\200\346\265\213\347\225\214\351\235\242", nullptr));
    gBox_tab_voiceDetect_voiceCtl->setTitle(QApplication::translate("MainWindow", "\350\257\255\351\237\263\346\216\247\345\210\266\346\240\217", nullptr));
    btn_tab_voiceDetect_run->setText(QApplication::translate("MainWindow", "\346\277\200\346\264\273", nullptr));
    gBox_tab_voiceDetect_voiceInfo->setTitle(QApplication::translate("MainWindow", "\350\257\255\351\237\263\344\277\241\346\201\257\346\240\217", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_voiceDetect), QApplication::translate("MainWindow", "\350\257\255\351\237\263\346\243\200\346\265\213\347\225\214\351\235\242", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_voiceDetect), QApplication::translate("MainWindow", "\350\257\255\351\237\263\346\243\200\346\265\213\347\225\214\351\235\242", nullptr));
    btn_tabrecord_outRecord->setText(QApplication::translate("MainWindow", "\346\227\245\345\277\227\345\257\274\345\207\272", nullptr));
    btn_tabrecord_clearRecord->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\346\227\245\345\277\227", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_record), QApplication::translate("MainWindow", "\346\227\245\345\277\227\347\225\214\351\235\242", nullptr));
}
