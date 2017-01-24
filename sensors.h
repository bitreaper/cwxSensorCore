////////////////////////////////////////////////////////////////////////////////
// 
// sensors.h - part of the CheapWx project.  
// Author: bitreaper
// Created: 05/20/2016
//
//    Copyright (C) 2016  bitreaper   bitreaper@n357.com
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SENSORS_H__
#define __SENSORS_H__

extern "C" {
#include "user_interface.h"
}

typedef struct sensors_tag {
   int vcc;
   float light;
   float temp;
   float humidity;
   float pressure;
   bool fan;       // fan on or off
   bool aircon;    // aircon on or off
   bool heat;      // heat on or off
   bool door;      // door sensor triggered
} SensorData_t;

SensorData_t readSensors(void);

#endif
