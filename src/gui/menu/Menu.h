#ifndef MENU_H_ 
#define MENU_H_ 

#include "ringbuffer/ItemRingBuffer.h"
#include "gui/menu/IMenuItem.h"

namespace phros_remote
{

class Menu
{
  public:
    Menu(void);
    virtual ~Menu();
    void left();
    void right();
    IMenuItem& iconcentral()const;
    IMenuItem& iconleft()const;
    IMenuItem& iconright()const;
    void printConsole();
  private:
    ItemRingBuffer<IMenuItem>* _ringBuffer;  
};

}

#endif