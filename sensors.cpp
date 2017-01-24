////////////////////////////////////////////////////////////////////////////////
// 
// sensors.cpp - part of the CheapWx project.  
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
//
////////////////////////////////////////////////////////////////////////////////

#include "sensors.h"
#include <Wire.h>
#include <LM75.h>
#include <stdlib.h>
#include <stdio.h>

// esp8266 related ones
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
extern "C" {
#include "user_interface.h"
}


////////////////////////////////////////////////////////////////////////////////
// globals in this file.
LM75 lm75;

////////////////////////////////////////////////////////////////////////////////
// functions

////////////////////////////////////////
//
SensorData_t readSensors(void)
{
   SensorData_t data;
   float temptemp = 0.0;
   data.vcc = ESP.getVcc();

   // wake up the sensor
   lm75.shutdown(false);
   delay(50);
   // give it time to settle
   temptemp = lm75.temp();

   // check to see if it's over the highest temp, if so adjust.
   if( temptemp > 127 )
   {
      data.temp = temptemp - 256;
   }else{
      data.temp = temptemp;
   }
   
   // shut it down now.  This makes the sensor go from 100uA to 3.5uA.  Worth doing for battery operated sensors.
   lm75.shutdown(true);

   data.light = 0;  // change this when we get light sensing working.
   data.door = false;
   data.aircon = false;
   data.heat = false;
   data.fan = false;
   data.humidity = 0;
   data.pressure = 0;
   return data;
}
