/*
 * Simple application to proxy Serial Port to TCP Server 
 * Copyright (C) 2017 TerraClear, Inc.
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

#ifndef XKTHREAD_RX_HPP
#define XKTHREAD_RX_HPP

#include <vector>

#include "xkthread.hpp"
#include "xkserial.hpp"

namespace xk
{
    class xkthread_rx : public xkthread
    {
        public:
            xkthread_rx(int socket_fd, xkserial* pttyport);
            virtual ~xkthread_rx();
            
            std::vector<std::string> _msglist;

        private:
            //implementation of base class pure virtual functions..
            void thread_runloop();
            
            int         _socket_fd = 0;
            xkserial*   _pttyport = nullptr;            
            std::string _instring = ""; 
            
    };
    
}
#endif /* XKTHREAD_RX_HPP */

