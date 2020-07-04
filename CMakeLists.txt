cmake_minimum_required(VERSION 3.0.2)
project(handrb_ui)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosmsg
  rospy
  std_msgs
  std_srvs
rb_msgAndSrv
  sensor_msgs
  cv_bridge
  image_transport
)
find_package(OpenCV)
MESSAGE(STATUS "OpenCV--include path: ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "    version: ${OpenCV_VERSION}")
# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 设置find__xxx的路径
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qt)
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/Qt5.12.0/5.12.0/gcc_64)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/lib/x86_64-linux-gnu/qt5)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Network REQUIRED)

SET(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES}
        ${Qt5Network_LIBRARIES} ${Qt5Xml_LIBRARIES} ${Qt5Qml_LIBRARIES})
include_directories(
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS})

#线程
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lpthread -Wl,--no-as-needed  -g3 -Wall")
# 与位置无关
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

catkin_package(
CATKIN_DEPENDS message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
    include/handrb_ui
    3rd/log4_cplus/include
)

link_directories("3rd/log4_cplus/lib")

#
add_executable(main
        src/main.cpp
        include/handrb_ui/MainWindow.h
        include/handrb_ui/RbQthread.h
        src/MainWindow.cpp
        src/BaseWindow.cpp
        )

target_link_libraries(
        main
        log4cplus
        logTest
        ${catkin_LIBRARIES}
        ${QT_LIBRARIES}
        ${OpenCV_LIBS}
)

