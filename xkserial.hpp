#ifndef XKSERIAL_HPP
#define XKSERIAL_HPP

#include <stdio.h>
#include <SerialStream.h>


using namespace std;

namespace  xk
{
    enum pickMsg
    {
        MSG_IDLE,
        MSG_PICK,
        MSG_SCOOP
    };
    
    //Copied most common subset from LibSerial...
    enum XKBaud
    {
        BAUD_2400  = SerialPort::BAUD_2400,
        BAUD_4800  = SerialPort::BAUD_4800,
        BAUD_9600  = SerialPort::BAUD_9600,
        BAUD_19200 = SerialPort::BAUD_19200,
        BAUD_38400 = SerialPort::BAUD_38400,
        BAUD_57600 = SerialPort::BAUD_57600,
        BAUD_115200 = SerialPort::BAUD_115200,
        BAUD_DEFAULT = SerialPort::BAUD_DEFAULT,
        BAUD_INVALID = -1
    } ;

    class xkserial
    {
        public:
            uint32_t _maxDuplicateMessages = 8;
        
            void open(string serialPortPath, XKBaud baudRate);
            void close();
            uint32_t write(string dataString);
            string readstring();
        
            void sendMsg(pickMsg msgtosend);
            //string read();
        
        private:
            LibSerial::SerialStream _serial_stream;
        
            pickMsg _lstMsg = pickMsg::MSG_IDLE;
            uint32_t _msgCount = 0;
        
    };
    
}

#endif /* XKSERIAL_HPP */


