#ifndef MAPPERPS3PAD_H_
#define MAPPERPS3PAD_H_

#include "MapperPsPad.h"
#include <memory>

namespace phros_remote 
{
  class MapperPs3Pad : public MapperPsPad
  {
    public:
    MapperPs3Pad(){}
    virtual ~MapperPs3Pad(){}
    virtual void map(const sensor_msgs::Joy &joy);
    private:
  };
}

#endif