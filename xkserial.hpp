/*
 * Basic C++ Wrapper for libseriaport 
 * Copyright (C) 2017 Jacobus du Preez / kdupreez@hotmail.com
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
*/

#ifndef XKSERIAL_HPP
#define XKSERIAL_HPP

#include <stdio.h>
#include <iostream>

//using libserialport from sigrok
// https://sigrok.org/wiki/Libserialport
#include <libserialport.h>

using namespace std;

namespace  xk
{

    //Copied most common subset from LibSerial...
    enum XKBaud
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

    class xkserial
    {
        public:
            xkserial();   
            ~xkserial();
            
            void open(string serialPortPath, XKBaud baudRate);
            void close();
            int writeString(string dataString, uint32_t timeout_ms);
            string readstring(uint32_t maxbufflen, uint32_t timeout_ms);
        
            bool isopen = false;
       
        private:
            sp_port* _ttyport;

        
    };
    
}

#endif /* XKSERIAL_HPP */


