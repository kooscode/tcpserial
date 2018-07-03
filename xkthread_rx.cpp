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

