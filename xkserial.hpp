#ifndef XKSERIAL_HPP
#define XKSERIAL_HPP

#include <stdio.h>
#include <iostream>

using namespace std;

namespace  xk
{

    //Copied most common subset from LibSerial...
    enum XKBaud
    {
        BAUD_2400,
        BAUD_4800,
        BAUD_9600 ,
        BAUD_19200,
        BAUD_38400,
        BAUD_57600,
        BAUD_115200,
        BAUD_DEFAULT,
        BAUD_INVALID
    };

    class xkserial
    {
        public:
            void open(string serialPortPath, XKBaud baudRate);
            void close();
            uint32_t writeString(string dataString);
            string readstring();
        
        private:

        
    };
    
}

#endif /* XKSERIAL_HPP */


