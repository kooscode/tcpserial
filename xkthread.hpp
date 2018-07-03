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

