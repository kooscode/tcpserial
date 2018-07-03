/*
 * Simple application to proxy Serial Port to TCP Server 
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

#include <unistd.h>
#include "xkthread_rx.hpp"

namespace xk
{
    xkthread_rx::xkthread_rx(int socket_fd, xkserial* pttyport)
    {
        _pttyport = pttyport;
        _socket_fd = socket_fd;
    }

    xkthread_rx::~xkthread_rx() 
    {
    }
    
    //runloop will be called continiously for so long thread 
    //is running.. call thread_stop to kill thread..
    void xkthread_rx::thread_runloop()
    {
        this->mutex_lock();
        
            if (_pttyport->isopen)
            {
                //read msg from Serial
                std::string ttymsg = _pttyport->readstring(128, 250);

                //send msg to TCP client
                write(_socket_fd, ttymsg.c_str(), ttymsg.length());

            }
        
        this->mutex_unlock();
    }
}

