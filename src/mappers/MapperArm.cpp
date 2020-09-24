/*
 * MapperArm.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: phil
 */

#include "MapperArm.h"
#include "MapperController.h"
#include "Hud.h"

#include "ohm_teleop_msgs/ArmCommand.h"
#include "PsProfiles.h"

#include <string>

#include "Hud.h"
#include "Communication.h"

namespace phros_remote
{

MapperArm::MapperArm(ros::NodeHandle& nh):
        _nh(nh)
{
  ros::NodeHandle prvNh("~");
  std::string topicArmCommand;
  prvNh.param<std::string>("topic_arm_command", topicArmCommand, "arm_command");
  _pubArmCommand = _nh.advertise<ohm_teleop_msgs::ArmCommand>(topicArmCommand, 1);
}

MapperArm::~MapperArm()
{
  // TODO Auto-generated destructor stub
}

void MapperArm::map(const std::shared_ptr<MapperPsPad>& msg)
{
  auto hud = Hud::getInstance();
   hud->setArmActive(true);
  if(_reset)
  {
    *_last = *msg;
    //this->emptyLast();
    _reset = false;
    return;
  }
  //static sensor_msgs::Joy last = joy;

   if(msg->buttonPressedPS() && !_last->buttonPressedPS())
  {
    MapperController::getInstance()->switchMapper(IMapper::RemoteType::HUD);
    return;
  }

  ohm_teleop_msgs::ArmCommand command;
  //map axes
  command.xe = msg->stickLeftY();//joy.axes[A1_Y];
  command.ye = msg->stickLeftX();//joy.axes[A1_X];
  command.ze = msg->stickRightY();//joy.axes[A2_Y];
  //map "buttons"
  //std::cout << __PRETTY_FUNCTION__ << " cur " << joy.buttons[B_SELECT] <<  " last " << _last.buttons[B_SELECT] << std::endl;
  //if(joy.buttons[B_SELECT] && !_last.buttons[B_SELECT])
  if(msg->buttonPressedSL() && !_last->buttonPressedSL())
  {
    //command.change_coordinate_system = joy.buttons[B_SELECT];
    command.change_coordinate_system = true;
   // abort();
  }
  //  if(joy.buttons[B_C] && !last.buttons[B_C])
  command.grzu = msg->buttonPressedC();//joy.buttons[B_C];  //TODO maybe put edge detection info MapperPsPadclass
  //if(joy.buttons[B_S] && !last.buttons[B_S])
  command.grauf = msg->buttonPressedS();//joy.buttons[B_S];
  command.pminus = msg->buttonPressedL1();//joy.buttons[B_L1];
  command.pplus = msg->buttonPressedR1();//joy.buttons[B_R1];
  command.rminus = msg->buttonPressedR2();//joy.buttons[B_R2];
  command.rplus  = msg->buttonPressedL2();//joy.buttons[B_L2];
  command.initial_pose = msg->buttonPressedST();//joy.buttons[B_START];
  command.yminus = msg->crossLeft();//joy.buttons[B_LEFT];
  command.yplus = msg->crossRight();//joy.buttons[B_RIGHT];
  command.initial_up = msg->crossUp();//joy.buttons[B_UP];
  command.initial_look_down = msg->crossDown();//joy.buttons[B_DOWN];
  command.initial_tunnel_side = msg->buttonPressedX();//joy.buttons[B_X];
  command.initial_tunnel_up = msg->buttonPressedT();//joy.buttons[B_T];
  _pubArmCommand.publish(command);
  *_last = *msg;

//  auto comm = Communication::getInstance();
//  auto imageMain = comm->imageMain();
//  if(imageMain)
//    {
//      hud->setImageMain(imageMain);
//     // hud->update();
//    }
}

void MapperArm::mapImage(void)
{
  auto hud = Hud::getInstance();
  auto comm = Communication::getInstance();
   auto imageGripper = comm->imageGripper();
   if(imageGripper)
     {
       hud->setImageMain(imageGripper);
      // hud->update();
     }
   else
     ROS_ERROR_STREAM_THROTTLE(1.0, __PRETTY_FUNCTION__ << " no image received");
}

}
