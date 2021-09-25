/*
 * MapperDriveReverse.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: phil
 */

#include "MapperDriveReverse.h"
#include "MapperController.h"

#include "ohm_actors_msgs/SensorHeadJoy.h"
#include "ohm_teleop_msgs/FlipperAngle.h"

#include <geometry_msgs/Twist.h>

#include "Hud.h"
#include "PsProfiles.h"
#include <string>

#include "Communication.h"
#include "Hud.h"

namespace phros_remote
{

static double _threshSwitchDir = 0.25;

MapperDriveReverse::MapperDriveReverse(ros::NodeHandle& nh)
    : _nh(nh)
    , _homingIters(0)
{
  std::string topicTwist;
  std::string topicFlippers;
  std::string topicSensorHead;
  double      threshSpeedLinearVar  = 0.0;
  double      threshSpeedAngVar     = 0.0;
  double      threshSpeedSensorHead = 0.0;
  double      speedFlipperManual    = 0.0;

  ros::NodeHandle prvNh("heini");
  prvNh.param<double>("thresh_speed_linear", threshSpeedLinearVar, 1.3);
  prvNh.param<double>("thresh_speed_linear", threshSpeedAngVar, 0.8);
  prvNh.param<double>("thresh_speed_sensor_head", threshSpeedSensorHead, 0.6);
  prvNh.param<double>("speed_flipper_manual", speedFlipperManual, 2.4);
  prvNh.param<double>("homing_yaw_rev", _homingYawRev, 2.7);
  prvNh.param<double>("homing_pitch_rev", _homingPitchRev, -0.4);

  _threshSpeedLinear     = static_cast<float>(threshSpeedLinearVar);
  _threshSpeedAngular    = static_cast<float>(threshSpeedAngVar);
  _threshSpeedSensorHead = static_cast<float>(threshSpeedSensorHead);
  _speedFlipperManual    = static_cast<float>(speedFlipperManual);

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
  if(_homingIters)
  {
    sensorHead.home = true; // this is kind of a hack. As long as still some "iters" are left, the homing mode is still active so
    _homingIters--;         // the head will turn itself in the new homing position
  }
  else
    sensorHead.home = false;
  geometry_msgs::Twist twist;

  twist.linear.x = 0.5 * (msg->axis(MapperPsPad::AxesPad::R2) - msg->axis(MapperPsPad::AxesPad::L2)) * _threshSpeedLinear;
  twist.angular.z = msg->axis(MapperPsPad::AxesPad::STICK_LEFT_X);
  sensorHead.pitch = msg->axis(MapperPsPad::AxesPad::STICK_RIGHT_Y) * _threshSpeedSensorHead;  
  sensorHead.yaw   = -msg->axis(MapperPsPad::AxesPad::STICK_RIGHT_X) * _threshSpeedSensorHead; 
  if(msg->button(MapperPsPad::ButtonsPad::A1).state())
    sensorHead.home = true;

  // map flippers
  ohm_teleop_msgs::FlipperAngle flippers;
  double vz = 0.0;
  if(msg->button(MapperPsPad::ButtonsPad::CROSS_UP).state()) 
    vz = -1.0;
  else if(msg->button(MapperPsPad::ButtonsPad::CROSS_DOWN).state()) 
    vz = 1.0;
  if(msg->button(MapperPsPad::ButtonsPad::T).state()) 
    flippers.back_left = _speedFlipperManual * vz;
  if(msg->button(MapperPsPad::ButtonsPad::C).state()) 
    flippers.back_right = _speedFlipperManual * vz;
  if(msg->button(MapperPsPad::ButtonsPad::S).state()) 
    flippers.front_left = -1.0 * _speedFlipperManual * vz;
  if(msg->button(MapperPsPad::ButtonsPad::X).state()) 
    flippers.front_right = -1.0 * _speedFlipperManual * vz;

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
      std::cout << __PRETTY_FUNCTION__ << "wanna switch" << std::endl;
      mapCtrl->switchMapper(IMapper::RemoteType::DRIVE, true);
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

void MapperDriveReverse::mapImage(void)
{
  auto hud       = Hud::getInstance();
  auto comm      = Communication::getInstance();
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

} // namespace phros_remote
