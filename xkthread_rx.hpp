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
            int _socket_fd = 0;
            xkserial* _pttyport = nullptr;
            
    };
    
}
#endif /* XKTHREAD_RX_HPP */

