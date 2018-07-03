#include "xkerror.hpp"

namespace xk
{
    xkerror::xkerror(string errmsg)
    {
        _errmsg = errmsg;
    }

    const char* xkerror::what() const throw()
    {
        return _errmsg.c_str();
    }
}