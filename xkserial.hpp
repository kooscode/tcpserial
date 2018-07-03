#ifndef XKSERIAL_HPP
#define XKSERIAL_HPP

#include <stdio.h>
#include <iostream>

//using libserialport from sigrok
// https://sigrok.org/wiki/Libserialport
#include <libserialport.h>

using namespace std;

namespace  xk
{

    //Copied most common subset from LibSerial...
    enum XKBaud
    {
        BAUD_2400 = 2400,
        BAUD_4800 = 4800,
        BAUD_9600 = 9600,
        BAUD_19200 = 19200,
        BAUD_38400 = 38400,
        BAUD_57600 = 57600,
        BAUD_115200 = 115200,
        BAUD_DEFAULT = 9600,
        BAUD_INVALID = -1
    };

    class xkserial
    {
        public:
            xkserial();   
            ~xkserial();
            
            void open(string serialPortPath, XKBaud baudRate);
            void close();
            int writeString(string dataString, uint32_t timeout_ms);
            string readstring(uint32_t maxbufflen, uint32_t timeout_ms);
        
            bool isopen = false;
       
        private:
            sp_port* _ttyport;

        
    };
    
}

#endif /* XKSERIAL_HPP */


