////////////////////////////////////////////////////////////////////////////////
// 
// commands.h - part of the CheapWx project.  
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

#ifndef commands_h
#define commands_h

#include <SimpleShell.h>

ExecStatus scribble( CommandLine* clp );
ExecStatus setSensorName( CommandLine* clp );
ExecStatus setServer( CommandLine* clp );
ExecStatus setSleepTime( CommandLine* clp );
ExecStatus testConnection( CommandLine* clp );
ExecStatus senseModeSendData( void );
ExecStatus sendData( CommandLine* clp );
ExecStatus getData( CommandLine* clp );
ExecStatus setCredentials( CommandLine* clp );
ExecStatus dumpEeprom( CommandLine* clp );
ExecStatus listNetworks( CommandLine* clp );
ExecStatus bounceNetwork( CommandLine* clp );
ExecStatus info( CommandLine* clp );
ExecStatus reboot( CommandLine* clp );
ExecStatus sleep( CommandLine* clp );
#endif
