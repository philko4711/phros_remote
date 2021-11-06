#include "MapperInspect.h"
#include <ohm_visca_control/viscaControl.h>
#include "Hud.h"
#include "MapperController.h"

namespace phros_remote
{
MapperInspect::MapperInspect():
_subsImgTele(std::make_unique<SuperImageSubscriber>("/tele_cam/image_rect", ros::Duration(1.0))),
_subsImgTotal(std::make_unique<SuperImageSubscriber>("/orbbec/overview", ros::Duration(1.0)))
{

}

void MapperInspect::map(std::shared_ptr<MapperPsPad>& msg)
{
  //toDo: this block is identically for all mappers. This should be in mapper base.
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

  ohm_visca_control::viscaControl control_data;
  if(!msg->button(MapperPsPad::ButtonsPad::A1).state())
  {
    const double axisZoom = msg->axis(MapperPsPad::AxesPad::STICK_LEFT_Y);
    control_data.autofocus     = 0;
    control_data.teleStandard  = msg->button(MapperPsPad::ButtonsPad::CROSS_LEFT).state();//joy->buttons[button_zoom_tele];
    control_data.wideStandard  = msg->button(MapperPsPad::ButtonsPad::CROSS_RIGHT).state();//joy->buttons[button_zoom_wide];
    control_data.variableSpeed = (uint8_t)(round(_zoomSpeedMax * std::abs(axisZoom)));   //joy->axes[axis_zoom]
    //if((joy->axes[axis_zoom]) < -(deadzone_zoom))
    if(axisZoom < -_deadzoneZoom)
    {
      control_data.wideVariable = 1;
    }
    //else if((joy->axes[axis_zoom]) > deadzone_zoom)
    else if((axisZoom) > _deadzoneZoom)
    {
      control_data.teleVariable = 1;
    }
    else
    {
      control_data.teleVariable = 0;
      control_data.wideVariable = 0;
    }
  }
  else if(msg->button(MapperPsPad::ButtonsPad::A1).state())   //else
  {
      const double axisFocus = msg->axis(MapperPsPad::AxesPad::STICK_LEFT_X);
    control_data.autofocus = 1;
    control_data.nearStandard = msg->button(MapperPsPad::ButtonsPad::CROSS_DOWN).state();//joy->buttons[button_focus_near];
    control_data.farStandard  = msg->button(MapperPsPad::ButtonsPad::CROSS_UP).state();//joy->buttons[button_focus_far];
    control_data.variableLevel = (uint8_t)(round(_focusSpeedMax*std::abs(axisFocus)));  //joy->axes[axis_focus]
    //if((joy->axes[axis_focus]) < -(deadzone_focus))
    if(axisFocus < -_deadzoneFocus)
    {
      control_data.farVariable = 1;
    }
    //else if((joy->axes[axis_focus]) > deadzone_focus)
    else if(axisFocus > _deadzoneFocus)
    {
      control_data.nearVariable = 1;
    }
    else
    {
      control_data.farVariable = 0;
      control_data.nearVariable = 0;
    }
  }
  control_data.header.stamp = ros::Time::now();
}
void MapperInspect::mapImage(void) {
   auto hud       = Hud::getInstance();
   auto imageTele = _subsImgTele->data();
   if(imageTele)
    hud->setImageMain(imageTele);

}
//void MapperInspect::init(void) {}

} // namespace phros_remote
