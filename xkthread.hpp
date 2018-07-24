/*
 * Base C++ cross platform threading class 
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

#ifndef XKTHREAD_HPP
#define XKTHREAD_HPP

#include <stdio.h>
#include <iostream>

#include <pthread.h>
#include <signal.h>

namespace  xk
{  
    class xkthread 
    {
        public:
            xkthread();
            virtual ~xkthread();

            void thread_start(std::string threadName);
            void thread_stopwait();
            
            //lock/unlock resources
            void mutex_lock();
            void mutex_unlock();

        protected:
            //pure virtual function for thread loop..
            virtual void thread_runloop() = 0;

            void thread_stop();

        private:
            std::string _threadName = "thread_base";
            pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
            bool _threadRunning = false;
            pthread_t _thread_main;
            
            static void* thread_run(void* thread_obj);
    };
   
}

#endif /* XKTHREAD_HPP */

