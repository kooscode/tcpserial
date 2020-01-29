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
 * CREATED BY: Koos du Preez
 * MODIFIED BY: Koos du Preez - kdupreez@hotmail.com
*/

#ifndef XKTHREAD_RX_HPP
#define XKTHREAD_RX_HPP

#include <vector>

#include "thread_base.hpp"
#include "basicserial.hpp"

namespace kooscode
{
    class thread_rx : public thread_base
    {
        public:
            thread_rx(int socket_fd, basicserial* pttyport);
            virtual ~thread_rx();
            
            std::vector<std::string> _msglist;

        private:
            //implementation of base class pure virtual functions..
            void thread_runloop();
            
            int         _socket_fd = 0;
            basicserial*   _pttyport = nullptr;            
            std::string _instring = ""; 
            
    };
    
}
#endif /* XKTHREAD_RX_HPP */

