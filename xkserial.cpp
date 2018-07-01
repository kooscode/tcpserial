#include <cstring>

#include "xkserial.hpp"
#include "xkerror.hpp"

#include "iostream"

using namespace std;

namespace xk
{
    //Constructor with init for serial comms.
    void xkserial::open(string serialPortPath, XKBaud baudRate)
    {
        //Open Serial Port..
        _serial_stream.Open( serialPortPath ) ;
        if ( !_serial_stream.good() )
        {
            string msg = "Error: Could not create serial port " + serialPortPath;
            throw xkerror(msg);
        }
        
        // Set the baud rate of the serial port.
        _serial_stream.SetBaudRate((LibSerial::SerialStreamBuf::BaudRateEnum)baudRate);
        if (!_serial_stream.good())
            throw xkerror("Error: Could not set the baud rate.");
        
        // Set the number of data bits.
        _serial_stream.SetCharSize( LibSerial::SerialStreamBuf::CHAR_SIZE_8 ) ;
        if ( !_serial_stream.good() )
            throw xkerror("Error: Could not set the character size.");
        
        // Disable parity.
        _serial_stream.SetParity( LibSerial::SerialStreamBuf::PARITY_NONE ) ;
        
        if (!_serial_stream.good())
            throw xkerror("Error: Could not disable the parity.");
        
        // Set the number of stop bits.
        _serial_stream.SetNumOfStopBits(1) ;
        if ( !_serial_stream.good() )
            throw xkerror("Error: Could not set the number of stop bits.");
        
        // Turn on hardware flow control.
        _serial_stream.SetFlowControl( LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE ) ;
        if ( !_serial_stream.good() )
            throw xkerror("Error: Could not use hardware flow control.");
    }
    
    void xkserial::close()
    {
        _serial_stream.Close();
    }

    
    //Writing string data to serial port..
    uint32_t xkserial::write(string dataString)
    {
        uint32_t retval = 0;
        if (_serial_stream.good())
        {
            retval = (uint32_t) dataString.length();
            _serial_stream.write(dataString.c_str() , retval);
        }
        
        return retval;
    }
    
    string xkserial::readstring()
    {
        string retval = "[NODATA]\n";
                
        char inchars[1024];
        memset(&inchars, 0, 1024);
        
        if (_serial_stream.good());   
        {
            _serial_stream.getline(inchars, 1024);
            retval = string(inchars);
        }
            
        
        return retval;
    }
    
    void xkserial::sendMsg(pickMsg msgtosend)
    {
        //TODO: build "ack" into msg protocol..

        bool dosend = false;
        
        //decide if msg should be sent..
        if (_lstMsg == msgtosend) //previous msg was the same and max count not reached..
        {
            //dont send more than X amount of duplicate messages..
            if (_msgCount < _maxDuplicateMessages)
            {
                dosend = true;
                _maxDuplicateMessages ++;
            }
        }
        else
        {
            dosend = true;
        }
        
        //if msg must sent, send it..
        if (dosend)
        {
            //default idle msg.
            string msgstr = "i\n";
            
            //decide on msg type
            switch (msgtosend)
            {
                case MSG_PICK:
                    msgstr = "p\n";
                    break;
                case MSG_SCOOP:
                    msgstr = "s\n";
                    break;
                default:
                    break;
            }
            
            //send msg..
            write(msgstr);
        }
        
    }

}
