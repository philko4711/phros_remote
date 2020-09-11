cmake_minimum_required(VERSION 2.8.3)
project(phros_remote)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  message_generation
  roscpp
  rqt_gui
  rqt_gui_cpp
  image_transport
  ohm_teleop_msgs
  ohm_actors_msgs
  ohm_schroedi_mc   #this dependency should be changed. Move the schroedi mc messages to the actors msgs
  ohm_power_unit    #this dependency should be changed. Move the power unit messages to the actors msgs
)

set(CMAKE_AUTORCC ON)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})
find_package(OpenCV REQUIRED)
find_package(cmake_modules REQUIRED)
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})


################################################
## Declare ROS messages, services and actions ##
################################################

#add_message_files(
#	FILES
#	pu.msg
#)

#generate_messages(
#	DEPENDENCIES
#	std_msgs
#)



###################################
## catkin specific configuration ##
###################################

catkin_package(
  INCLUDE_DIRS ${INC_DIR}
  LIBRARIES ${PROJECT_NAME}
  ${CMAKE_CURRENT_BINARY_DIR}/..
  CATKIN_DEPENDS roscpp rqt_gui rqt_gui_cpp std_msgs message_runtime message_generation
)
#catkin_package(
#  CATKIN_DEPENDS roscpp  
#)


###########
## Build ##
###########


## set variables for clock-node

#set(SRCS_CLOCK
#  src/clock/clock.cpp
#)
#set(HDRS_CLOCK
#  src/clock/clock.h
#)
#set(UIS_CLOCK
#  src/clock/clock.ui
#)

## set variables for horizon-node

#set(SRCS_HORIZON
#  src/horizon/horizon.cpp
#  src/horizon/WidgetHorizon.cpp
#)
#set(HDRS_HORIZON
#  src/horizon/horizon.h
#  src/horizon/WidgetHorizon.h
#)

## set variables for horizon2-node

set(SRCS_HORIZON2
  src/horizon2/horizon2.cpp
  src/horizon2/WidgetHorizon2.cpp
)
set(HDRS_HORIZON2
  src/horizon2/horizon2.h
  src/horizon2/WidgetHorizon2.h
)

## set variables for health-node
set(SRCS_HEALTH
  src/health/HealthPlugin.cpp
)
set(HDRS_HEALTH
  src/health/HealthPlugin.h
)
set(UIS_HEALTH
  src/health/health_plugin.ui
)

## set variables for CO2 node
set(SRCS_CO2
  src/co2/CO2Plugin.cpp
)
set(HDRS_CO2
  src/co2/CO2Plugin.h
)
set(UIS_CO2
  src/co2/co2.ui
)



## set variables for exploration node
set(SRCS_EXPLORATION
  src/exploration/ExplorationPlugin.cpp
)
set(HDRS_EXPLORATION
  src/exploration/ExplorationPlugin.h
)
set(UIS_EXPLORATION
  src/exploration/exploration.ui
)

SET(UIS_REMOTE src/gui/main.ui)

SET(SRCS_REMOTE src/RemotePlugin.cpp
				src/gui/WidgetMain.cpp
				src/Communication.cpp
				src/gui/Hud.cpp
				src/gui/PerspectiveModel.cpp
				src/utils/SuperImageSubscriber.cpp
				src/MapperController.cpp
				src/mappers/MapperBase.cpp
				src/mappers/MapperArm.cpp
				src/mappers/MapperDrive.cpp
				src/mappers/MapperDriveReverse.cpp
				src/mappers/MapperHud.cpp
				src/gui/menu/MenuItemBase.cpp
				src/gui/menu/MenuItemModeArm.cpp
				src/gui/menu/MenuItemModeDrive.cpp
				src/gui/menu/MenuItemModeDriveReverse.cpp
				src/gui/menu/MenuItemFlipperAscent.cpp
				src/gui/menu/MenuItemFlipperFlat.cpp
				src/gui/menu/MenuItemFlipperUp4.cpp
				src/gui/icons/qt_res.qrc)

SET(HDRS_REMOTE src/RemotePlugin.h
				src/gui/WidgetMain.h
				src/gui/Hud.h)


##########################################################################################

set(INC_DIR
  include
   ${PROJECT_SOURCE_DIR}/src/gui
   ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(${INC_DIR} ${catkin_INCLUDE_DIRS})

##########################################################################################
# qt5_wrap_cpp produces moc files for all headers listed
# qt5_wrap_ui produces .h files for all .ui files listed

## Clock:
#qt5_wrap_cpp(MOCS_SRCS_CLOCK ${HDRS_CLOCK})
#qt5_wrap_ui(UI_HEADER_CLOCK ${UIS_CLOCK})
#
### Horizon:
#qt5_wrap_cpp(MOCS_SRCS_HORIZON ${HDRS_HORIZON})
#
### Horizon2:
#qt5_wrap_cpp(MOCS_SRCS_HORIZON2 ${HDRS_HORIZON2})
#
### Health:
#qt5_wrap_cpp(MOCS_SRCS_HEALTH ${HDRS_HEALTH})
#qt5_wrap_ui(UI_HEADER_HEALTH ${UIS_HEALTH})
#
### CO2:
#qt5_wrap_cpp(MOCS_SRCS_CO2 ${HDRS_CO2})
#qt5_wrap_ui(UI_HEADER_CO2 ${UIS_CO2})
#
### Exploration:
#qt5_wrap_cpp(MOCS_SRCS_EXPLORATION ${HDRS_EXPLORATION})
#qt5_wrap_ui(UI_HEADER_EXPLORATION ${UIS_EXPLORATION})

## Remote:
qt5_wrap_cpp(MOCS_SRCS_REMOTE ${HDRS_REMOTE})
qt5_wrap_ui(UI_HEADER_REMOTE ${UIS_REMOTE})


##########################################################################################





## Clock:
#add_library(clock ${SRCS_CLOCK} ${MOCS_SRCS_CLOCK} ${UI_HEADER_CLOCK})
#target_link_libraries(clock ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
#
### Horizon:
#add_library(horizon ${SRCS_HORIZON} ${MOCS_SRCS_HORIZON})
#target_link_libraries(horizon ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
#
### Horizon2:
#add_library(horizon2 ${SRCS_HORIZON2} ${MOCS_SRCS_HORIZON2})
#target_link_libraries(horizon2 ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
#
### Health:
#add_library(health ${SRCS_HEALTH} ${MOCS_SRCS_HEALTH} ${UI_HEADER_HEALTH})
#target_link_libraries(health ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
#
### CO2:
#add_library(co2 ${SRCS_CO2} ${MOCS_SRCS_CO2} ${UI_HEADER_CO2})
#target_link_libraries(co2 ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
#
### Exploration:
#add_library(exploration ${SRCS_EXPLORATION} ${MOCS_SRCS_EXPLORATION} ${UI_HEADER_EXPLORATION})
#target_link_libraries(exploration ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

## Simulator for Health-Plugin:
#add_executable(pupublisher_node src/health/simulator.cpp)
#target_link_libraries(pupublisher_node ${catkin_LIBRARIES})

add_library(remote ${SRCS_REMOTE} ${MOCS_SRCS_REMOTE} ${UI_HEADER_REMOTE})
target_link_libraries(remote ${catkin_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

