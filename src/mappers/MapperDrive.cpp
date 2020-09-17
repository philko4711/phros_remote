/*
 * MapperDrive.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: phil
 */

#include "MapperDrive.h"
#include "MapperController.h"
#include "ohm_teleop_msgs/FlipperAngle.h"
#include "ohm_actors_msgs/SensorHeadJoy.h"
#include <std_srvs/Empty.h>
#include "Hud.h"
#include "Communication.h"

#include <geometry_msgs/Twist.h>

#include <string>
#include "PsProfiles.h"



namespace phros_remote
{

static double _threshSwitchDir = 0.25;

MapperDrive::MapperDrive(ros::NodeHandle& nh):
    _nh(nh),
    _homingIters(0),
    _homingYawFor(0.0),
    _homingPitchFor(0.0)
{
  std::string topicTwist;
  std::string topicFlippers;
  std::string topicSensorHead;
  double threshSpeedLinearVar  = 0.0;
  double threshSpeedAngVar     = 0.0;
  double threshSpeedSensorHead = 0.0;
  double speedFlipperManual    = 0.0;

  ros::NodeHandle prvNh("~");

  prvNh.param<std::string>("topic_twist", topicTwist, "vel/cmd");
  prvNh.param<std::string>("topic_flippers", topicFlippers, "flipper/manual");
  prvNh.param<std::string>("topic_sensor_head", topicSensorHead, "sensorhead_joy");
  prvNh.param<double>("thresh_speed_linear", threshSpeedLinearVar,       1.3);
  prvNh.param<double>("thresh_speed_linear", threshSpeedAngVar,          0.8);
  prvNh.param<double>("thresh_speed_sensor_head", threshSpeedSensorHead, 0.6);
  prvNh.param<double>("speed_flipper_manual", speedFlipperManual, 2.4);
  prvNh.param<double>("homing_yaw_for"   , _homingYawFor  , 0.0);
  prvNh.param<double>("homing_pitch_for" , _homingPitchFor, -0.4);

  _threshSpeedLinear     = static_cast<float>(threshSpeedLinearVar );
  _threshSpeedAngular    = static_cast<float>(threshSpeedAngVar    );
  _threshSpeedSensorHead = static_cast<float>(threshSpeedSensorHead);
  _speedFlipperManual    = static_cast<float>(speedFlipperManual);

//  _pubTwist = _nh.advertise<geometry_msgs::Twist>(topicTwist, 1);
//  _pubFlippers = _nh.advertise<ohm_teleop_msgs::FlipperAngle>(topicFlippers, 1);
//  _pubSensorHead = _nh.advertise<ohm_actors_msgs::SensorHeadJoy>(topicSensorHead, 1);
  _clientMap = _nh.serviceClient<std_srvs::Empty>("map_victim");


  // _timerSwitchDriveMode = ros::Time::now();
}

MapperDrive::~MapperDrive()
{
  // TODO Auto-generated destructor stub
}

void MapperDrive::map(const sensor_msgs::Joy& joy)
{
  std::cout << __PRETTY_FUNCTION__ << "call" << std::endl;
  auto hud = Hud::getInstance();
  hud->setArmActive(false);
  if(_reset)
  {
    _last = joy;
    //this->emptyLast();
    _reset = false;
    return;
  }
  //static sensor_msgs::Joy last = joy;
  if(joy.buttons[B_PS] && !_last.buttons[B_PS])
  {
    MapperController::getInstance()->switchMapper(IMapper::RemoteType::HUD);
    return;
  }
  //  else
  //    std::cout << __PRETTY_FUNCTION__ << joy.buttons[B_SELECT] << " " << last.buttons[B_SELECT] << std::endl;

  ohm_actors_msgs::SensorHeadJoy sensorHead;
  if(_homingIters)   //this is kind of a hack. As long as still some "iters" are left, the homing mode is still active so
  {                  //the head will turn itself in the new homing position
    sensorHead.home = true;
    _homingIters--;
  }
  else
    sensorHead.home = false;
  geometry_msgs::Twist twist;
  twist.linear.x =  -0.5 * (joy.axes[R2] - joy.axes[L2]) * _threshSpeedLinear;

  twist.angular.z =  joy.axes[A1_X];
  //map sensor head

  sensorHead.pitch = joy.axes[A2_Y] * _threshSpeedSensorHead;
  sensorHead.yaw   = joy.axes[A2_X] * _threshSpeedSensorHead;
  if(joy.buttons[B_A2])
    sensorHead.home = true;
  //map flippers
  ohm_teleop_msgs::FlipperAngle flippers;

  double vz = 0.0;
  if(joy.buttons[B_UP])
    vz = 1.0;
  else if(joy.buttons[B_DOWN])
    vz = -1.0;
  //read select button
  if(joy.buttons[B_T])
    flippers.front_left  = -1.0 * _speedFlipperManual * vz;
  if(joy.buttons[B_C])
    flippers.front_right =  -1.0 * _speedFlipperManual * vz;
  if(joy.buttons[B_S])
    flippers.back_left   = _speedFlipperManual * vz;
  if(joy.buttons[B_X])
    flippers.back_right  = _speedFlipperManual * vz;

//  _pubTwist.publish(twist);
//  _pubSensorHead.publish(sensorHead);
//  _pubFlippers.publish(flippers);
  auto comm = Communication::getInstance();

    comm->publishToDrive(twist);
    comm->publishToSensorHead(sensorHead);
    comm->publishToFlippers(flippers);



  //  auto imageMain = comm->imageMain();
//  if(imageMain)
//  {
//    hud->setImageMain(imageMain);
//    // hud->update();
//  }
 //static bool switched = false;
 static ros::Time timerSwitch = ros::Time::now();
 auto mapCtrl = MapperController::getInstance();
 if(joy.buttons[B_L1] && joy.buttons[B_R1])
 {
   const double timePressed = (ros::Time::now() - timerSwitch).toSec();
   std::cout << __PRETTY_FUNCTION__ << " buttons pressed since " << timePressed << " > " << _threshSwitchDir <<std::endl;
   if((timePressed > _threshSwitchDir) && !_switched)
   {
     std::cout << __PRETTY_FUNCTION__ << "wanna switch" << std::endl;
     // if(mapCtrl->type() == IMapper::RemoteType::DRIVE)
     mapCtrl->switchMapper(IMapper::RemoteType::DRIVE_REV, true);
     return;
     //        else if(_mapper->type() == IMapper::RemoteType::DRIVE_REV)
     //          this->switchMapper(IMapper::RemoteType::DRIVE, false);
     timerSwitch = ros::Time::now();
     _switched = false;
   }
 }
 else
 {
   timerSwitch = ros::Time::now();
   _switched = false;
 }

  _last = joy;
  std::cout << __PRETTY_FUNCTION__ << "exit" << std::endl;
}

void MapperDrive::mapImage(void)
{
  std::cout << __PRETTY_FUNCTION__ << "call" << std::endl;
  auto hud = Hud::getInstance();
  auto comm = Communication::getInstance();
  auto imageMain = comm->imageMain();
  if(imageMain)
  {
    hud->setImageMain(imageMain);
     hud->update();
  }
  else
    std::cout << __PRETTY_FUNCTION__ << "no image received" << std::endl;
  std::cout << __PRETTY_FUNCTION__ << "exit" << std::endl;
}

void MapperDrive::init(void)
{
  std::cout << __PRETTY_FUNCTION__ << "call" << std::endl;
  auto comm = Communication::getInstance();
  comm->setHoming(_homingPitchFor, _homingYawFor);
  _homingIters = 50;
  
  std::cout << __PRETTY_FUNCTION__ << "exit" << std::endl;
}

}
