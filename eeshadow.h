////////////////////////////////////////////////////////////////////////////////
// 
// eeshadow.h - part of the CheapWx project.  
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


#ifndef eeshadow_h
#define eeshadow_h

#include <Arduino.h>
#include <EEPROM.h>

#define EESIZE 1024

////////////////////////////////////////
//// now for the eeprom locations
#define MAX_CRED 64

typedef struct {
   char ssid[MAX_CRED];
   char password[MAX_CRED];
} credential_t;

#define SERVER_LENGTH 256
#define NAME_LENGTH   64
#define CREDENTIALS_LENGTH sizeof(credential_t)

#define EESIZE 1024
#define EEBASE 0
#define NAME_LOCATION      EEBASE
#define SERVER_LOCATION    NAME_LOCATION+NAME_LENGTH
#define CREDENTIALS_LOCATION SERVER_LOCATION+SERVER_LENGTH

typedef struct eedata_tag {
   char sensorName[NAME_LENGTH];
   char server[SERVER_LENGTH];
   credential_t creds;
} eedata_t;

class EEShadow
{
  private:
   union shadow_tag
   {
      byte raw[1024];
      //eedata_t cooked;
      struct eedata_tag {
         char sensorName[NAME_LENGTH];
         char server[SERVER_LENGTH];
         credential_t creds;
         int sleeptime;
      } cooked;
   } shadow;
   
   void eeClear( int, int );
   void eeSet( int, int, byte );
   void eeWrite( int, int, byte* );
   void eeRead( int, int, byte* );
   void eeCommit();
   void eeLoad();

  public:
   EEShadow();
   void begin();
   void setName( char* name );
   void setServer( char* server );
   void setSleepTime( int time );
   void setCreds( char* ssid, char* password );

   const char* getName();
   const char* getServer();
   int getSleepTime();
   const credential_t* getCreds();
};

#endif

