/*
 * Basic C++ Wrapper for libseriaport 
 * Copyright (C) 2019 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CREATED BY: Koos du Preez
 * MODIFIED BY: Koos du Preez - kdupreez@hotmail.com
*/

#ifndef BASICSERIAL_HPP
#define BASICSERIAL_HPP

#include <stdio.h>
#include <iostream>

// # using libserialport from sigrok : https://sigrok.org/wiki/Libserialport
// # -----------------------------------------------------------------------
// # STEP 1 - CLONE SRC
// git clone git://sigrok.org/libserialport
// #  STEP 2 - INSTALL Autoconf dependencies
// sudo apt install autoconf libtool
// #  STEP 3 - make and install
// cd libserialport
// ./autogen.sh
// ./configure
// make
// sudo make install

#include <libserialport.h>

namespace  kooscode
{

    //Copied most common subset from LibSerial...
    enum Baud
    {
        BAUD_2400 = 2400,
        BAUD_4800 = 4800,
        BAUD_9600 = 9600,
        BAUD_19200 = 19200,
        BAUD_38400 = 38400,
        BAUD_57600 = 57600,
        BAUD_115200 = 115200,
        BAUD_DEFAULT = 9600,
        BAUD_INVALID = -1
    };

    class basicserial
    {
        public:
            basicserial();   
            ~basicserial();
            
            void open(std::string serialPortPath, Baud baudRate);
            void close();
            int writeString(std::string dataString, uint32_t timeout_ms = 250);
            std::string readstring(uint32_t maxbufflen, uint32_t timeout_ms);
        
            bool isopen = false;
       
        private:
            sp_port* _ttyport;

        
    };
    
}

#endif /* BASICSERIAL_HPP */
