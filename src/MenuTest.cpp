#include "MenuTest.h"
#include "utils/MapperPsPadSing.h"
#include <QPainter>
#include <QtCore/QDebug>
#include <thread>

MenuTest::MenuTest(QWidget* parent)
    : _threadRos(*this)
{
  // connect(&_timer, SIGNAL(timeout()), this, SLOT(callBackTimer()));
  connect(this, SIGNAL(newJoyCommand()), this, SLOT(joyCommandReceived()));
  //_timer.start(1000);
  this->resize(1600, 1600);
}

void MenuTest::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  QRect    rectCenter = this->rect();
  rectCenter.setWidth(this->rect().width() / 8);
  rectCenter.setHeight(this->rect().height() / 8);
  rectCenter.moveCenter(this->rect().center());
  painter.drawImage(rectCenter, _menu.iconcentral()->menuIcon(), _menu.iconcentral()->menuIcon().rect());

  QRect menuRectSMall = rectCenter;
  menuRectSMall.setWidth(this->rect().width() / 16);
  menuRectSMall.setHeight(this->rect().height() / 16);

  if(_menu.iconleft())
  {
    menuRectSMall.moveCenter(this->rect().center() - QPoint(menuRectSMall.width() + 10, 0));
    painter.drawImage(menuRectSMall, _menu.iconleft()->menuIcon(), _menu.iconleft()->menuIcon().rect());
  }
  if(_menu.iconright())
  {
    menuRectSMall.moveCenter(this->rect().center() + QPoint(menuRectSMall.width() + 10, 0));
    painter.drawImage(menuRectSMall, _menu.iconright()->menuIcon(), _menu.iconright()->menuIcon().rect());
  }
  if(_menu.icontop())
  {
  menuRectSMall.moveCenter(this->rect().center() + QPoint(0, menuRectSMall.height() + 10));
    painter.drawImage(menuRectSMall, _menu.icontop()->menuIcon(), _menu.icontop()->menuIcon().rect());  
  }
  if(_menu.iconbottom())
  {
      menuRectSMall.moveCenter(this->rect().center() - QPoint(0, menuRectSMall.height() + 10));
    painter.drawImage(menuRectSMall, _menu.iconbottom()->menuIcon(), _menu.iconbottom()->menuIcon().rect());
  }

}

void MenuTest::callBackTimer() { std::cout << __PRETTY_FUNCTION__ << " thread id " << std::this_thread::get_id() << std::endl; }

void MenuTest::joyCommandReceived(void)
{
  phros_remote::MapperPsPad& gamepad = *phros_remote::MapperPsPadSing::instance().psPad();
  _mutexJoy.lock();
  // std::cout << __PRETTY_FUNCTION__ << " thread id " << std::this_thread::get_id() << std::endl;

  phros_remote::MapperPsPadSing::instance().psPad()->map(_joy);
  _mutexJoy.unlock();
  if(gamepad.button(phros_remote::MapperPsPad::ButtonsPad::CROSS_LEFT).edge() == phros_remote::StateButton::Edge::RISING)
  {
    qDebug() << __PRETTY_FUNCTION__ << "LEFT";
    _menu.left();
    this->update();
  }
  else if(gamepad.button(phros_remote::MapperPsPad::ButtonsPad::CROSS_RIGHT).edge() == phros_remote::StateButton::Edge::RISING)
  {
    qDebug() << __PRETTY_FUNCTION__ << "RIGHT";
    _menu.right();
    this->update();
  }
  else if(gamepad.button(phros_remote::MapperPsPad::ButtonsPad::CROSS_UP).edge() == phros_remote::StateButton::Edge::RISING)
  {
    qDebug() << __PRETTY_FUNCTION__ << "UP";
    _menu.top();
    this->update();
  }
  else if(gamepad.button(phros_remote::MapperPsPad::ButtonsPad::CROSS_DOWN).edge() == phros_remote::StateButton::Edge::RISING)
  {
    qDebug() << __PRETTY_FUNCTION__ << "DOWN";
    _menu.bottom();
    this->update();
  }
}

void MenuTest::callBackJoy(const sensor_msgs::Joy& joy)
{
  std::thread::id this_id = std::this_thread::get_id();
  // std::cout << __PRETTY_FUNCTION__ << " thread id " << std::this_thread::get_id() << std::endl;
  _mutexJoy.lock();
  _joy = joy;
  _mutexJoy.unlock();
  emit this->newJoyCommand();
}