#include "ThreadRos.h"
#include <ros/ros.h>
#include "MenuTest.h"

ThreadRos::ThreadRos(MenuTest& parent):
_parent(parent),
  _threadSpin(&ThreadRos::run, this)
{
  ros::NodeHandle nh;
  _subsJoy = nh.subscribe("joy", 1, &MenuTest::callBackJoy, &_parent);
}

ThreadRos::~ThreadRos()
{
  _threadSpin.join();
}

void ThreadRos::run()
{
  ros::spin();
}

void ThreadRos::callBackJoy(const sensor_msgs::Joy& joy)
{
  //emit this->sendJoy(joy);
}