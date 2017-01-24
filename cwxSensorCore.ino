////////////////////////////////////////////////////////////////////////////////
// 
// cwxSensorCore - part of the CheapWx project.  
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

////////////////////////////////////////
// includes
////////////////////////////////////////

// standard ones
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LM75.h>

// esp8266 related ones
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
extern "C" {
#include "user_interface.h"
}
ADC_MODE(ADC_VDD);

// specific to this app
#include <SimpleShell.h>
#include "commands.h"
#include "eeshadow.h"

////////////////////////////////////////
// globals
////////////////////////////////////////

ExecStatus help( CommandLine* clp );  // solves chicken and egg of refering to one in the other.

Command cmdlist[] = {
   {"help", help},
   {"name",setSensorName},
   {"server",setServer},
   {"time", setSleepTime},
   {"creds",setCredentials},
   {"list", listNetworks},
   {"bounce",bounceNetwork},
   {"info", info},
   {"reboot",reboot},
   {"sleep",sleep},   
   {"dump", dumpEeprom},
   {"send",sendData},
   {"get",getData},
   {NULL,NULL}
};

// need to make this a built in command for the shell.
ExecStatus help( CommandLine* clp )
{
   Command* ptr = cmdlist;

   Serial.println("Available commands:");
   while( ptr->cmd_name )
   {
      Serial.println(ptr->cmd_name);
      ptr++;
   }
}


SimpleShell shell(cmdlist);

/////////
// if pin14 is pulled down, it will set it to command mode.  Since it re-executes
// setup and loop each time it comes out of sleep, you won't necessarily have to reboot
// after pulling the pin down.
int senseMode = 0;

EEShadow eeshadow;
char msg[128];
bool NetworkUp = false;

////////////////////////////////////////
// setup
////////////////////////////////////////
void setup(void)
{

   Serial.begin(115200);
   Serial.setTimeout(10000);
   Wire.begin();        // defaults still to pin4 (SDA) and pin5 (SCL)
   pinMode(14, INPUT_PULLUP);
   senseMode = digitalRead(14); // read here so we can pop into commandMode if needed

   pinMode(17, OUTPUT);
   
   Serial.println("Starting eeprom");
   eeshadow.begin();

   sprintf(msg, "Starting wifi for %s", eeshadow.getCreds()->ssid );
   Serial.println(msg);
   
   WiFi.begin(eeshadow.getCreds()->ssid, eeshadow.getCreds()->password);
   WiFi.mode(WIFI_STA);

   for (int i = 0; i < 30; i++)
   {
      if (WiFi.status() != WL_CONNECTED)
      {
         NetworkUp = false;
      }
      else
      {
         Serial.println("");
         Serial.println("WiFi connected");
         Serial.println("IP address: ");
         Serial.println(WiFi.localIP());
         WiFi.printDiag(Serial);
         NetworkUp = true;
         break;
      }
      delay(500);
      Serial.print(".");
   }
   if( ! NetworkUp )
   {
      Serial.println("Network didn't start.  Might want to reset or try a different AP");
   }
   else
   {
      Serial.printf("\nReady\n");
      if( !senseMode )
      {
         Serial.printf("\ncommand mode> ");
      }
   }
}
 
////////////////////////////////////////
// loop
////////////////////////////////////////
void loop(void)
{
   char input[320];

   // there's two modes here. if senseMode is pulled high by internal pullup resistors, then
   // we're going to sense, then transmit, then sleep.
   // If it's not in senseMode, then it's set to config mode, which means we read from the
   // serial port and execute shell commands.
   
   if( senseMode )
   {
      if( NetworkUp )
      {
         Serial.println("Awake, attached and executing code");
         senseModeSendData();
      }
      else
      {
         Serial.println("Network is down, can't push.  Please go to command mode (pull pin 14 down) and fix credentials");
      }

      ESP.deepSleep( eeshadow.getSleepTime() * 1000000UL, WAKE_RF_DEFAULT);      
   }
   else
   {
      if( Serial.available() > 0 )
      {
         memset(input, 0, 320);
         Serial.readBytesUntil('\r',input,320);
         // do whatever just came in
         switch( shell.executeCommand( input ) )
         {
            case SUCCESS:
               break;
            case FAILED:
               Serial.println("Command failed!");
               break;
            case NOTFOUND:
               Serial.println("Could not find command"); // command not found.
               break;
         }
         Serial.printf("\ncommand mode> ");
      }
   }
}