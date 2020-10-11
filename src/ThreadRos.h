#ifndef THREADROS_H_ 
#define THREADROS_H_

#include <QtCore/QObject>
#include <thread>
#include <sensor_msgs/Joy.h>
#include <ros/ros.h>

class MenuTest;

class ThreadRos 
{

  public: 
    ThreadRos(MenuTest& parent);
    virtual ~ThreadRos();
   
    private:
      void run();
      void callBackJoy(const sensor_msgs::Joy& joy);
      MenuTest& _parent;
      ros::Subscriber _subsJoy;
      std::thread _threadSpin;
};

#endif