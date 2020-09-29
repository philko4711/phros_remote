/*
 * MapperDriveReverse.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: phil
 */

#include "MapperDriveReverse.h"
#include "MapperController.h"

#include "ohm_teleop_msgs/FlipperAngle.h"
#include "ohm_actors_msgs/SensorHeadJoy.h"

#include <geometry_msgs/Twist.h>

#include <string>
#include "PsProfiles.h"
#include "Hud.h"

#include "Hud.h"
#include "Communication.h"

namespace phros_remote
{

static double _threshSwitchDir = 0.25;

MapperDriveReverse::MapperDriveReverse(ros::NodeHandle& nh):
    _nh(nh),
    _homingIters(0)
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
  prvNh.param<double>("homing_yaw_rev"   , _homingYawRev  , 2.7);
  prvNh.param<double>("homing_pitch_rev" , _homingPitchRev, -0.4);

  _threshSpeedLinear     = static_cast<float>(threshSpeedLinearVar );
  _threshSpeedAngular    = static_cast<float>(threshSpeedAngVar    );
  _threshSpeedSensorHead = static_cast<float>(threshSpeedSensorHead);
  _speedFlipperManual    = static_cast<float>(speedFlipperManual);

//  _pubTwist      = _nh.advertise<geometry_msgs::Twist>(topicTwist, 1);
//  _pubFlippers   = _nh.advertise<ohm_teleop_msgs::FlipperAngle>(topicFlippers, 1);
//  _pubSensorHead = _nh.advertise<ohm_actors_msgs::SensorHeadJoy>(topicSensorHead, 1);

}

MapperDriveReverse::~MapperDriveReverse()
{
  // TODO Auto-generated destructor stub
}

void MapperDriveReverse::map(std::shared_ptr<MapperPsPad>& msg)
{
  auto hud = Hud::getInstance();
  hud->setArmActive(false);
  if(_reset)
  {
    // this->emptyLast();
    msg->reset();
    _reset = false;
    return;
  }

  //static sensor_msgs::Joy last = joy;
  //if(joy.buttons[B_PS] && !_last.buttons[B_PS])
  if(msg->button(MapperPsPad::ButtonsPad::PS).edge() == StateButton::Edge::RISING)
  //if(msg->buttonPressedPS() && !_last->buttonPressedPS())
  {
    MapperController::getInstance()->switchMapper(IMapper::RemoteType::HUD);
    return;
  }

  ohm_actors_msgs::SensorHeadJoy sensorHead;
  if(_homingIters)
  {
    //std::cout << __PRETTY_FUNCTION__ << "iters " << _homingIters << std::endl;
    sensorHead.home = true;   //this is kind of a hack. As long as still some "iters" are left, the homing mode is still active so
    _homingIters--;           //the head will turn itself in the new homing position
  }
  else
    sensorHead.home = false;
  geometry_msgs::Twist twist;

  twist.linear.x =  -0.5 * (msg->axis(MapperPsPad::AxesPad::R2) - msg->axis(MapperPsPad::AxesPad::L2)) * _threshSpeedLinear;
  //twist.linear.x =  -0.5 * (msg->L2() - msg->R2()) * _threshSpeedLinear;
  //twist.linear.x =  -0.5 * (joy.axes[L2] - joy.axes[R2]) * _threshSpeedLinear;   //axes switched for reverse

twist.angular.z =  msg->axis(MapperPsPad::AxesPad::STICK_LEFT_X);
  //twist.angular.z =  msg->stickLeftX();
  //twist.angular.z =  joy.axes[A1_X];
  //map sensor head
  sensorHead.pitch = msg->axis(MapperPsPad::AxesPad::STICK_RIGHT_Y) * _threshSpeedSensorHead;//msg->stickRightY() 
  sensorHead.yaw   = -msg->axis(MapperPsPad::AxesPad::STICK_RIGHT_X) * _threshSpeedSensorHead;// msg->stickRightX() ;
  // sensorHead.pitch = msg->stickRightY() * _threshSpeedSensorHead;
  // sensorHead.yaw   = -msg->stickRightX() * _threshSpeedSensorHead;
  // sensorHead.pitch = joy.axes[A2_Y] * _threshSpeedSensorHead;
  // sensorHead.yaw   = -joy.axes[A2_X] * _threshSpeedSensorHead;   //axis sign switched for reverse mode
  if(msg->button(MapperPsPad::ButtonsPad::A1).state())//PressedA1())
  //if(msg->buttonPressedA1())
  //if(joy.buttons[B_A2])
    sensorHead.home = true;


  //map flippers
  ohm_teleop_msgs::FlipperAngle flippers;

  double vz = 0.0;
  //if(joy.buttons[B_UP])
  //if(msg->crossUp())
  if(msg->button(MapperPsPad::ButtonsPad::CROSS_UP).state())//crossUp())
    vz = -1.0;
  //else if(joy.buttons[B_DOWN])
  else if(msg->button(MapperPsPad::ButtonsPad::CROSS_DOWN).state())//msg->crossDown())//else if(msg->crossDown())
    vz = 1.0;
  //read select button
  //if(joy.buttons[B_T])
  //if(msg->buttonPressedT())
  if(msg->button(MapperPsPad::ButtonsPad::T).state())//  msg->buttonPressedT())
    flippers.back_left  = _speedFlipperManual * vz;
  //if(joy.buttons[B_C])
  if(msg->button(MapperPsPad::ButtonsPad::C).state())//(msg->buttonPressedC())
  //if(msg->buttonPressedC())
    flippers.back_right = _speedFlipperManual * vz;
  //if(joy.buttons[B_S])
  //if(msg->buttonPressedS())
  if(msg->button(MapperPsPad::ButtonsPad::S).state())//(msg->buttonPressedS())
    flippers.front_left   = -1.0 * _speedFlipperManual * vz;
  //if(joy.buttons[B_X])
  //if(msg->buttonPressedX())
  if(msg->button(MapperPsPad::ButtonsPad::X).state())//(msg->buttonPressedX())
    flippers.front_right  = -1.0 * _speedFlipperManual * vz;

  auto comm = Communication::getInstance();

  comm->publishToDrive(twist);
  comm->publishToSensorHead(sensorHead);
  comm->publishToFlippers(flippers);

 //static bool switched = false;
 static ros::Time timerSwitch = ros::Time::now();
 auto mapCtrl = MapperController::getInstance();
 if((msg->button(MapperPsPad::ButtonsPad::L1).state()) && (msg->button(MapperPsPad::ButtonsPad::R1).state()))
 //if(msg->buttonPressedL1() && msg->buttonPressedR1())
 //if(joy.buttons[B_L1] && joy.buttons[B_R1])
 {
   const double timePressed = (ros::Time::now() - timerSwitch).toSec();
   std::cout << __PRETTY_FUNCTION__ << " buttons pressed since " << timePressed << " > " << _threshSwitchDir << std::endl;
   if((timePressed > _threshSwitchDir) && !_switched)
   {
     std::cout << __PRETTY_FUNCTION__ << "wanna switch" << std::endl;
     // if(mapCtrl->type() == IMapper::RemoteType::DRIVE)
     mapCtrl->switchMapper(IMapper::RemoteType::DRIVE, true);
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

  //*_last = *msg;

}

void MapperDriveReverse::mapImage(void)
{
  auto hud = Hud::getInstance();
  auto comm = Communication::getInstance();
  auto imageMain = comm->imageMain();
  if(imageMain)
  {
    hud->setImageMain(imageMain);
    // hud->update();
  }
}

void MapperDriveReverse::init(void)
{
  auto comm = Communication::getInstance();
  comm->setHoming(_homingPitchRev, _homingYawRev);
  _homingIters = 50;
}

}
