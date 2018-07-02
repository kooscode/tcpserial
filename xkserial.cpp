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

    }
    
    void xkserial::close()
    {
        
    }

    
    //Writing string data to serial port..
    uint32_t xkserial::writeString(string dataString)
    {

        return 0;
    }
    
    string xkserial::readstring()
    {
  
        
        return "";
    }

}
