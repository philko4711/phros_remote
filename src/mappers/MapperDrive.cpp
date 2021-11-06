/*
 * MapperDrive.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: phil
 */

#include "MapperDrive.h"
#include "Communication.h"
#include "gui/Hud.h"
#include "MapperController.h"
#include "ohm_actors_msgs/SensorHeadJoy.h"
#include "ohm_teleop_msgs/FlipperAngle.h"
#include <std_srvs/Empty.h>

#include <geometry_msgs/Twist.h>

#include "PsProfiles.h"
#include <string>

namespace phros_remote
{

static double _threshSwitchDir = 0.25;

MapperDrive::MapperDrive(ros::NodeHandle& nh)
    : _nh(nh)
    , _homingIters(0)
    , _homingYawFor(0.0)
    , _homingPitchFor(0.0)
{
  std::string topicTwist;
  std::string topicFlippers;
  std::string topicSensorHead;
  double      threshSpeedLinearVar  = 0.0;
  double      threshSpeedAngVar     = 0.0;
  double      threshSpeedSensorHead = 0.0;
  double      speedFlipperManual    = 0.0;

  ros::NodeHandle prvNh("~");
  prvNh.param<double>("thresh_speed_linear", threshSpeedLinearVar, 1.3);
  prvNh.param<double>("thresh_speed_linear", threshSpeedAngVar, 0.8);
  prvNh.param<double>("thresh_speed_sensor_head", threshSpeedSensorHead, 0.6);
  prvNh.param<double>("speed_flipper_manual", speedFlipperManual, 2.4);
  prvNh.param<double>("homing_yaw_for", _homingYawFor, 0.0);
  prvNh.param<double>("homing_pitch_for", _homingPitchFor, -0.4);

  _threshSpeedLinear     = static_cast<float>(threshSpeedLinearVar);
  _threshSpeedAngular    = static_cast<float>(threshSpeedAngVar);
  _threshSpeedSensorHead = static_cast<float>(threshSpeedSensorHead);
  _speedFlipperManual    = static_cast<float>(speedFlipperManual);
}

MapperDrive::~MapperDrive()
{
  // TODO Auto-generated destructor stub
}

void MapperDrive::map(std::shared_ptr<MapperPsPad>& msg)
{
  auto hud = Hud::getInstance();
  hud->setArmActive(false);
  if(_reset)
  {
    msg->reset();
    _reset = false;
    return;
  }
  if(msg->button(MapperPsPad::ButtonsPad::PS).edge() == StateButton::Edge::RISING)
  {
    MapperController::getInstance()->switchMapper(IMapper::RemoteType::HUD);
    return;
  }

  ohm_actors_msgs::SensorHeadJoy sensorHead;
  if(_homingIters) // this is kind of a hack. As long as still some "iters" are
                   // left, the homing mode is still active so
  {                // the head will turn itself in the new homing position
    sensorHead.home = true;
    _homingIters--;
  }
  else
    sensorHead.home = false;
  
  //map twist forward
  geometry_msgs::Twist twist;
  twist.linear.x = -0.5 * (msg->axis(MapperPsPad::AxesPad::R2) - msg->axis(MapperPsPad::AxesPad::L2)) * _threshSpeedLinear;
  twist.angular.z = msg->axis(MapperPsPad::AxesPad::STICK_LEFT_X); 
  
  // map sensor head
  sensorHead.pitch = msg->axis(MapperPsPad::AxesPad::STICK_RIGHT_Y) * _threshSpeedSensorHead; 
  sensorHead.yaw   = msg->axis(MapperPsPad::AxesPad::STICK_RIGHT_X) * _threshSpeedSensorHead; 
  if(msg->button(MapperPsPad::ButtonsPad::A2).state()) 
    sensorHead.home = true;
  
  // map flippers
  ohm_teleop_msgs::FlipperAngle flippers;
  double vz = 0.0;
  if(msg->button(MapperPsPad::ButtonsPad::CROSS_UP).state()) 
    vz = 1.0;
  else if(msg->button(MapperPsPad::ButtonsPad::CROSS_DOWN).state()) // msg->crossDown())
    vz = -1.0;
  if(msg->button(MapperPsPad::ButtonsPad::T).state()) 
    flippers.front_left = -1.0 * _speedFlipperManual * vz;
  if(msg->button(MapperPsPad::ButtonsPad::C).state()) 
    flippers.front_right = -1.0 * _speedFlipperManual * vz;
  if(msg->button(MapperPsPad::ButtonsPad::S).state()) 
    flippers.back_left = _speedFlipperManual * vz;
  if(msg->button(MapperPsPad::ButtonsPad::X).state()) 
    flippers.back_right = _speedFlipperManual * vz;

  //publish data
  auto comm = Communication::getInstance();
  comm->publishToDrive(twist);
  comm->publishToSensorHead(sensorHead);
  comm->publishToFlippers(flippers);

  static ros::Time timerSwitch = ros::Time::now();
  auto             mapCtrl     = MapperController::getInstance();
  if((msg->button(MapperPsPad::ButtonsPad::L1).state()) && (msg->button(MapperPsPad::ButtonsPad::R1).state()))
  {
    const double timePressed = (ros::Time::now() - timerSwitch).toSec();
    if((timePressed > _threshSwitchDir) && !_switched)
    {
      mapCtrl->switchMapper(IMapper::RemoteType::DRIVE_REV, true);
      return;
      timerSwitch = ros::Time::now();
      _switched   = false;
    }
  }
  else
  {
    timerSwitch = ros::Time::now();
    _switched   = false;
  }
}

void MapperDrive::mapImage(void)
{
  auto hud       = Hud::getInstance();
  auto comm      = Communication::getInstance();
  auto imageMain = comm->imageMain();
  if(imageMain)
  {
    hud->setImageMain(imageMain);
    hud->update();
  }
  else
    ROS_ERROR_STREAM_THROTTLE(1.0, __PRETTY_FUNCTION__ << " no image received");
}

void MapperDrive::init(void)
{
  auto comm = Communication::getInstance();
  comm->setHoming(_homingPitchFor, _homingYawFor);
  _homingIters = 50; // TODO: this is so very dumb. Add a homing service to
                     // sensor head and call it from here
}

} // namespace phros_remote
