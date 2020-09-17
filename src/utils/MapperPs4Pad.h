#ifndef MAPPERPS4PAD_H_
#define MAPPERPS4PAD_H_

#include "MapperPsPad.h"
#include <memory>

namespace phros_remote 
{

namespace
{
  const float AXE_MAX = 1.0f;
}

class MapperPs4Pad : public MapperPsPad {
public:
  MapperPs4Pad() {}
  virtual ~MapperPs4Pad() {}
  virtual void map(const sensor_msgs::Joy &joy);
private:
  
};

} // namespace phros_remote

#endif