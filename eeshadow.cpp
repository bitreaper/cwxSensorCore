////////////////////////////////////////////////////////////////////////////////
// 
// eeshadow.cpp - part of the CheapWx project.  
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

#include "eeshadow.h"

extern "C" {
#include "user_interface.h"
}

EEShadow::EEShadow()
{
}

void EEShadow::setName( char* name )
{
   strcpy( this->shadow.cooked.sensorName, name );
   this->eeCommit();   
}

void EEShadow::setServer( char* server )
{
   strcpy( this->shadow.cooked.server, server );
   this->eeCommit();
}

void EEShadow::setSleepTime( int time )
{
   this->shadow.cooked.sleeptime = time;
   this->eeCommit();   
}

void EEShadow::setCreds( char* ssid, char* password )
{
   strcpy( this->shadow.cooked.creds.ssid, ssid );
   strcpy( this->shadow.cooked.creds.password, password );
   this->eeCommit();   
}

const char* EEShadow::getName() 
{
   return this->shadow.cooked.sensorName;
}
const char* EEShadow::getServer( )
{
   return this->shadow.cooked.server;
}
int EEShadow::getSleepTime( )
{
   return this->shadow.cooked.sleeptime;
}

const credential_t* EEShadow::getCreds()
{
   return &this->shadow.cooked.creds;
}

void EEShadow::eeCommit( )
{
   for (int idx = 0; idx < EESIZE; idx++) {
      EEPROM.write(idx, this->shadow.raw[idx]);
   }
   EEPROM.commit();
}

void EEShadow::eeLoad( )
{
   for (int idx = EEBASE ; idx < EESIZE ; idx++) {
      this->shadow.raw[idx] = EEPROM.read(idx);
   }
}

void EEShadow::eeClear( int location, int length )
{
   for (int idx = location ; idx < length ; idx++) {
      EEPROM.write(idx, 0);
   }
   EEPROM.commit();
}

void EEShadow::eeSet( int location, int length, byte data )
{
   for (int idx = 0; idx < length ; idx++) {
      EEPROM.write(idx+location, data);
   }
   EEPROM.commit();
}

void EEShadow::eeWrite( int location, int length, byte* data )
{
   for (int idx = 0; idx < length; idx++) {
      this->shadow.raw[idx+location] = data[idx];
   }
   eeCommit();
}

void EEShadow::eeRead( int location, int length, byte* buffer )
{
   for( int idx = 0; idx < length; idx++ )
   {
      buffer[idx] = this->shadow.raw[idx+location];
   }
}

void EEShadow::begin()
{
   memset(this->shadow.raw,0,EESIZE);
   EEPROM.begin(EESIZE);
   eeLoad();
}
