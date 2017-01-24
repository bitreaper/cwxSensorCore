////////////////////////////////////////////////////////////////////////////////
// 
// commands.cpp - part of the CheapWx project.  
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

#include "commands.h"
#include "sensors.h"
#include "eeshadow.h"
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

#define HTTP_CODE_OK 200

////////////////////////////////////////////////////////////////////////////////
// file scoped globals
extern EEShadow eeshadow;   // this is the one declared and instantiated in main

////////////////////////////////////////////////////////////////////////////////
// Setters and getters

////////////////////////////////////////
ExecStatus setSensorName( CommandLine* clp )
{
   Serial.printf( "Setting name to %s\n", clp->argv[0] );
   eeshadow.setName( clp->argv[0] );
   return SUCCESS;
}

////////////////////////////////////////
ExecStatus setServer( CommandLine* clp )
{
   Serial.printf( "Setting server url base to %s\n", clp->argv[0] );
   eeshadow.setServer( clp->argv[0] );
   return SUCCESS;
}

////////////////////////////////////////
ExecStatus setSleepTime( CommandLine* clp )
{
   Serial.printf( "Setting sleep to %s\n", clp->argv[0] );
   eeshadow.setSleepTime( atoi(clp->argv[0]) );
   return SUCCESS;
}

////////////////////////////////////////
ExecStatus setCredentials( CommandLine* clp )
{
   Serial.printf("Setting ssid to %s and password to %s\n", clp->argv[0], clp->argv[1] );
   eeshadow.setCreds( clp->argv[0], clp->argv[1] );
   return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
ExecStatus dumpEeprom( CommandLine* clp )
{

   Serial.println();
   Serial.printf("Dump of eeprom\n");
   Serial.printf("Sensor Name: %s\n", eeshadow.getName());
   Serial.printf("Server String: %s\n", eeshadow.getServer());
   Serial.printf("Sleep time: %d\n", eeshadow.getSleepTime());
   Serial.printf("First set of creds, ssid/passwd: %s / %s \n", eeshadow.getCreds()->ssid, eeshadow.getCreds()->password);
   Serial.println(eeshadow.getCreds()->ssid);
   return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// now for the action commands
ExecStatus pushData(char* data, char* type)
{
   // go to http://<server>/testlink and add your data just like you would with sendData
   HTTPClient http;
   char url[384];
   sprintf(url, "%s/keep?sensor=%s&data=%s&sensorType=%s", eeshadow.getServer(),
           eeshadow.getName(),
           data,
           type);
   
   http.begin(url); //HTTP

   // start connection and send HTTP header
   int httpCode = http.GET();

   if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
         String payload = http.getString();
         Serial.println(payload);
      }
   } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      return FAILED;
   }
   
   http.end();

   return SUCCESS;
}

////////////////////////////////////////
// a forced read of the sensor and a push of that data to server.
ExecStatus sendData( CommandLine* clp )
{
   return senseModeSendData();
}

////////////////////////////////////////
// a forced read of the sensor and a push of that data to server.
ExecStatus senseModeSendData( void )
{
   SensorData_t data = readSensors();
   char buffer[32];

   sprintf(buffer, "%d", (int)(data.temp*100));
   if( pushData( buffer, "temp" ) == FAILED )
   { return FAILED; }
   Serial.printf("temp is %s\n", buffer);
   Serial.printf("temp send successful\n");

   sprintf(buffer, "%d", (int)(data.light));
   if( pushData( buffer, "light" ) == FAILED )
   { return FAILED; }
   Serial.printf("light is %s\n", buffer);
   Serial.printf("light send successful\n");
   
   sprintf(buffer, "%d", data.vcc);
   if( pushData( buffer, "vcc" ) == FAILED )
   { return FAILED; }
   Serial.printf("vcc is %s\n", buffer);
   Serial.printf("vcc send successful\n");

   return SUCCESS;
}

////////////////////////////////////////
// a forced read of the sensor, and print to the console
ExecStatus getData( CommandLine* clp )
{
   SensorData_t data = readSensors();
   Serial.print("Current temp is: ");
   Serial.println(data.temp);
   Serial.print("Current light: ");
   Serial.println(data.light);
   Serial.print("Current vcc: ");
   Serial.println(data.vcc);
   return SUCCESS;
}

ExecStatus listNetworks( CommandLine* clp )
{
   Serial.println("Scanning local networks...");
   WiFi.disconnect();
   WiFi.scanNetworks();
   WiFi.begin(eeshadow.getCreds()->ssid, eeshadow.getCreds()->password);
   return SUCCESS;   
}

ExecStatus bounceNetwork( CommandLine* clp )
{
   Serial.println("Disconnecting network...");
   WiFi.disconnect();
   Serial.printf("Reconnecting network to %s\n", eeshadow.getCreds()->ssid);   
   WiFi.begin(eeshadow.getCreds()->ssid, eeshadow.getCreds()->password);
   return SUCCESS;   
}

ExecStatus info( CommandLine* clp )
{
   Serial.printf("\n\ncwxSensorCore compiled on %s %s", __DATE__, __TIME__);
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   WiFi.printDiag(Serial);
   Serial.println("Flash Chip info:");
   Serial.print( "Flash size: " ); Serial.println( ESP.getFlashChipSize() );
   Serial.print( "Flash speed: " ); Serial.println( ESP.getFlashChipSpeed() );
   return SUCCESS;   
}

ExecStatus reboot( CommandLine* clp )
{
   ESP.restart();
}

ExecStatus sleep( CommandLine* clp )
{
   Serial.printf("Sleeping for %d seconds", eeshadow.getSleepTime());
   delay(1000);
   ESP.deepSleep( eeshadow.getSleepTime() * 1000000UL, WAKE_RF_DEFAULT);
   return SUCCESS;
}
