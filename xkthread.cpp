#include "xkthread.hpp"

namespace  xk
{  
    xkthread::xkthread() 
    {

    }

    xkthread::~xkthread() 
    {   
        pthread_mutex_destroy(&_mutex);
    }
    
    
    void xkthread::thread_start(std::string threadName)
    {
        _threadName = threadName;
        _threadRunning = true;

        pthread_create(&_thread_main, NULL, thread_run, this);
        std::cout << "Thread Started: " << _threadName << std::endl;
    }

    void* xkthread::thread_run(void* thread_obj)
    {
        xkthread* ctxt = (xkthread*) thread_obj;        
        
        while (ctxt->_threadRunning)
        {
            ctxt->thread_runloop();
        }
        
        return nullptr;     
    }

    void xkthread::thread_stop() 
    {
        std::cout << "Thread Stopping: " << _threadName << std::endl;        
        _threadRunning = false;
    }

    void xkthread::thread_stopwait()
    {
        thread_stop(); 
        pthread_join(_thread_main, NULL);
    }
    
    void xkthread::mutex_lock()
    {
        pthread_mutex_lock(&_mutex);
        
    }
    
    void xkthread::mutex_unlock()
    {
       pthread_mutex_unlock(&_mutex);       
    }

}

