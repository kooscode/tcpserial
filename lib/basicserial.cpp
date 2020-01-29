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

#include <cstring>
#include <iostream>
#include <libserialport.h>

#include "basicserial.hpp"
#include "error_base.hpp"

namespace kooscode
{

    basicserial::basicserial() 
    {
        _ttyport = nullptr;
    }
    
    basicserial::~basicserial()
    {
        //force close any open port.
        this->close();
    }

    //Constructor with init for serial comms.
    void basicserial::open(std::string serialPortPath, Baud baudRate)
    {
        //force close on already open ports..
        if (_ttyport != nullptr)
            this->close();

        isopen = false;
        
        int callret = SP_OK;
        
        //Get native device port for serial port name/path
        callret = sp_get_port_by_name(serialPortPath.c_str(), &_ttyport);
        if  (callret != SP_OK)
            throw error_base( "OPEN: Error getting serial device.");

        //Open Port..
        callret = sp_open(_ttyport, SP_MODE_READ_WRITE);
        if (callret != SP_OK)
            throw error_base( "OPEN: Error opening port.");
        
        //set baud rate
        callret = sp_set_baudrate(_ttyport, baudRate);
        if (callret != SP_OK)
            throw error_base( "OPEN: Error getting setting baud.");
        
        isopen = true;
    }
    
    void basicserial::close()
    {
        if (_ttyport != nullptr)
        {
            //close and free port..
            sp_close(_ttyport);                    
            sp_free_port(_ttyport);
        }
        
        _ttyport = nullptr;
        isopen = false;
    }

    
    //Writing string data to serial port..
    int basicserial::writeString(std::string dataString, uint32_t timeout_ms)
    {
         return sp_blocking_write(_ttyport, dataString.c_str(), dataString.length(), timeout_ms);
    }
    
    std::string basicserial::readstring(uint32_t maxbufflen, uint32_t timeout_ms)
    {
        std::string retstr = "";
        
        if (maxbufflen < 1)
            throw error_base("READ: Max Length must be non zero.");
        
        //setup read buffer & clear
        char ttybuff[maxbufflen];
        memset(&ttybuff, 0x00, maxbufflen);

        //get serial return & send back to client
        int readsize = sp_blocking_read_next(_ttyport, &ttybuff, maxbufflen, timeout_ms);
        
        if (readsize > 0)
          retstr =  std::string(ttybuff);
        
        return retstr;

    }

}
