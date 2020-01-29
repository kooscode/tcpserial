/*
 * Simple application to proxy Serial Port to TCP Server 
 * Copyright (C) 2017 Koos du Preez, Inc.
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
 * MODIFIED BY: Koos du Preez - kdupreez@hotmail.com
*/


#include <unistd.h>
#include "thread_rx.hpp"

namespace kooscode
{
    thread_rx::thread_rx(int socket_fd, basicserial* pttyport)
    {
        _pttyport = pttyport;
        _socket_fd = socket_fd;
    }

    thread_rx::~thread_rx() 
    {
    }
    
    //runloop will be called continiously for so long thread 
    //is running.. call thread_stop to kill thread..
    void thread_rx::thread_runloop()
    {
        this->mutex_lock();
        
            if (_pttyport->isopen)
            {
                //read msg from Serial
                _instring += _pttyport->readstring(128, 250);
                
                size_t nlpos = _instring.find('\n') + 1;
                
                //check if string read is \n terminated.
                //if not, might be partial string, so keep and for next.
                if (nlpos > 0)
                {
                    //cut out only up to \n terminator.
                    std::string ttymsg = _instring.substr(0, nlpos);

                    //keep remainder around or reset.
                    _instring = (_instring.length() > nlpos) ? _instring.substr(nlpos, _instring.length()) : "";
                    
                    //send msg to TCP client
                    write(_socket_fd, ttymsg.c_str(), ttymsg.length());                    

                    std::cout << "TTY > TCP: " << ttymsg ;
                }


            }
        
        this->mutex_unlock();
    }
}

