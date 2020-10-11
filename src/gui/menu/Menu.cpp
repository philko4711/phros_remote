#include "Menu.h"
#include "MenuItemModeDrive.h"
#include "MenuItemModeDriveReverse.h"
#include "MenuItemModeArm.h"
#include "MenuItemFlipperFlat.h"
#include "MenuItemFlipperAscent.h"
#include "MenuItemFlipperDescent.h"
#include "MenuItemFlipperUp4.h"
#include <ros/ros.h>


namespace phros_remote
{
Menu::Menu()
{
  try
  {
    std::shared_ptr<IMenuItem> object = std::make_shared<MenuItemModeDrive>(":/menu_icons/drive.png");
  _ringBuffer = new ItemRingBuffer<IMenuItem>(object);
  // std::cout << __PRETTY_FUNCTION__ << " initial element " << std::endl;
  // this->printConsole();
  _ringBuffer->pushBack(new ItemRingBuffer<IMenuItem>(std::make_shared<MenuItemModeDriveReverse>(":/menu_icons/drive_rev.png")));
  // std::cout << __PRETTY_FUNCTION__ << " first push" << std::endl;
  // this->printConsole();
  _ringBuffer->pushBack(new ItemRingBuffer<IMenuItem>(std::make_shared<MenuItemModeArm>(":/menu_icons/arm.png")));

_ringBuffer->pushBack(new ItemRingBuffer<IMenuItem>(std::make_shared<MenuItemFlipperFlat>(":/menu_icons/flat.png")));

  }
  catch(std::string& e)
  {
    ROS_ERROR_STREAM(__PRETTY_FUNCTION__ << " " << e << std::endl);
    std::exit(1);
  }
  // std::cout << __PRETTY_FUNCTION__ << " initialized" << std::endl;
  // this->printConsole();
  // _menu = new MenuItemModeDrive(":/menu_icons/drive.png");//   new MenuItemModeDrive(":/menu_icons/drive.png"));
  // _menu->init(new MenuItemModeDriveReverse(":/menu_icons/drive_rev.png"));
  // _menu->pushBack(new MenuItemModeArm(":/menu_icons/arm.png"), _menu);  
}

Menu::~Menu()
{
  
}
IMenuItem& Menu::iconcentral() const {return _ringBuffer->content();}
IMenuItem& Menu::iconleft() const {return _ringBuffer->previous()->content();}
IMenuItem& Menu::iconright() const {return _ringBuffer->next()->content();}
 // namespace phros_remote

void Menu::left()
{
 _ringBuffer = _ringBuffer->previous();
}

void Menu::right()
{
  _ringBuffer = _ringBuffer->next();
}

void Menu::printConsole()
{
  ItemRingBuffer<phros_remote::IMenuItem>* item = _ringBuffer;
  do
  {
    std::cout << " " << item->content().type() << std::endl << "\t-> " << item->next()->content().type() << std::endl
                << "\t<- " << item->previous()->content().type() << std::endl;
    item = item->next();
  }while(item != _ringBuffer);
}

}