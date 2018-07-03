#include <cstring>
#include <libserialport.h>

#include "xkserial.hpp"

#include "xkerror.hpp"

#include "iostream"

using namespace std;

namespace xk
{

    xkserial::xkserial() 
    {
        _ttyport = nullptr;
    }
    
    xkserial::~xkserial()
    {
        //force close any open port.
        this->close();
    }

    //Constructor with init for serial comms.
    void xkserial::open(string serialPortPath, XKBaud baudRate)
    {
        //force close on already open ports..
        if (_ttyport != nullptr)
            this->close();

        isopen = false;
        
        int callret = SP_OK;
        
        //Get native device port for serial port name/path
        callret = sp_get_port_by_name(serialPortPath.c_str(), &_ttyport);
        if  (callret != SP_OK)
            throw xkerror( "OPEN: Error getting serial device.");

        //Open Port..
        callret = sp_open(_ttyport, SP_MODE_READ_WRITE);
        if (callret != SP_OK)
            throw xkerror( "OPEN: Error opening port.");
        
        //set baud rate
        callret = sp_set_baudrate(_ttyport, baudRate);
        if (callret != SP_OK)
            throw xkerror( "OPEN: Error getting setting baud.");
        
        isopen = true;
    }
    
    void xkserial::close()
    {
        if (_ttyport != nullptr)
        {
            //close and free port..
            sp_close(_ttyport);                    
            sp_free_port(_ttyport);
        }
        
        _ttyport = nullptr;
        isopen = false;
    }

    
    //Writing string data to serial port..
    int xkserial::writeString(string dataString, uint32_t timeout_ms)
    {
         return sp_blocking_write(_ttyport, dataString.c_str(), dataString.length(), timeout_ms);
    }
    
    string xkserial::readstring(uint32_t maxbufflen, uint32_t timeout_ms)
    {
        string retstr = "";
        
        if (maxbufflen < 1)
            throw xkerror("READ: Max Length must be non zero.");
        
        //setup read buffer & clear
        char ttybuff[maxbufflen];
        memset(&ttybuff, 0x00, maxbufflen);

        //get serial return & send back to client
        int readsize = sp_blocking_read_next(_ttyport, &ttybuff, maxbufflen, timeout_ms);
        
        if (readsize > 0)
          retstr =  std::string(ttybuff);
        
        return retstr;

    }

}
