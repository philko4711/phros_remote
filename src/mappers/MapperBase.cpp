/*
 * MapperBase.cpp
 *
 *  Created on: May 17, 2019
 *      Author: phil
 */

#include "MapperBase.h"
#include "utils/MapperPs4Pad.h"

namespace phros_remote
{

MapperBase::MapperBase():
    _reset(true)
{
  // TODO add config from launch for gamepad type
  _mapperPsPad = std::make_unique<MapperPs4Pad>();

}

MapperBase::~MapperBase()
{
  // TODO Auto-generated destructor stub
}


} /* namespace ohm_remote */
