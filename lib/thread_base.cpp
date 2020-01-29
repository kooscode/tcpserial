/*
 * Base C++ cross platform threading class 
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

#include "thread_base.hpp"

namespace  kooscode
{  
    thread_base::thread_base() 
    {

    }

    thread_base::~thread_base() 
    {   
        //kill thread if still running..
        if (_threadRunning)
        {
            _threadRunning = false;
           _pthread_main->join();
        }
    }
    
    
    void thread_base::thread_start(std::string threadName)
    {
        //wait for old threads to end and kill them off
        if (_pthread_main != nullptr)
        {
            thread_resume(); //make sure its not paused..
            thread_stopwait(); //stop it.. 
            delete _pthread_main;
            _pthread_main = nullptr;
        }
        
        //set thread name and running state.
        _internal_mutex.lock();
            _threadName = threadName;
            _threadRunning = true;
        _internal_mutex.unlock();
        
        _pthread_main = new std::thread(thread_run, this);

    }

    void thread_base::thread_run(thread_base* ctxt)
    {
        ctxt->_internal_mutex.lock();
            bool threadRunning = ctxt->_threadRunning;
            bool threadPaused = ctxt->_threadPaused;
        ctxt->_internal_mutex.unlock();
        
        while (threadRunning)
        {

            if (!threadPaused)
            {
                try
                {
                    ctxt->thread_runloop();
                }
                catch (std::exception& e)
                {
                    std::cout << std::endl << " ****** Thread " << ctxt->get_name() << ": Exception - " << e.what() << std::endl;
                    ctxt->thread_stop();
                }

            }
            else
            {
                //create lock vineer over existing mutex..
                std::unique_lock<std::mutex> pause_lock(ctxt->_internal_mutex);

                // 1. Wait for Notification, 
                // 2. Then block while acquiring lock.. 
                // 3. Then check func return value
                // 4. If return val = false, release lock and wait again (go to step 1)
                // 5. If return val = true, retain lock and move on..    
                ctxt->_internal_wait_lock.wait(pause_lock, [ctxt]{return !ctxt->_threadPaused;});
                
                //lock auto releases at end of scope..
            }

            
            ctxt->_internal_mutex.lock();
                threadRunning = ctxt->_threadRunning;
                threadPaused = ctxt->_threadPaused;
            ctxt->_internal_mutex.unlock();
            
            
        } 
    }

    void thread_base::thread_stop() 
    {
        _internal_mutex.lock();
            _threadRunning = false;
            _threadPaused = false;
        _internal_mutex.unlock();        

        _internal_wait_lock.notify_all();
    }

    void thread_base::thread_stopwait()
    {
        thread_stop(); 
        if (_pthread_main != nullptr)
        {
            _pthread_main->join();
            delete _pthread_main;
            _pthread_main = nullptr;
        }
    }
    
    bool thread_base::isrunning()
    {
        _internal_mutex.lock();
            bool retval = _threadRunning;
        _internal_mutex.unlock();   

        return retval;
    }

    void thread_base::thread_pause()
    {
        _internal_mutex.lock();
            _threadPaused = true;
        _internal_mutex.unlock();   
    }
    
    void thread_base::thread_resume()
    {
        _internal_mutex.lock();
            _threadPaused = false;
        _internal_mutex.unlock();   

        _internal_wait_lock.notify_all();
    }
    
    bool thread_base::ispaused()
    {
        _internal_mutex.lock();
            bool retval = _threadPaused;
        _internal_mutex.unlock();   

        return retval;
    }
            
    std::string thread_base::get_name()
    {
        return _threadName;
    }

    void thread_base::mutex_lock()
    {
        _mutex.lock();
    }
    void thread_base::mutex_unlock()
    {
        _mutex.unlock();;
    }

}

