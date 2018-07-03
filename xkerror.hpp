#ifndef XKERROR_HPP
#define XKERROR_HPP

#include <iostream>
#include <exception>

using namespace std;

namespace  xk
{

    class xkerror: public exception
    {
        public:
            xkerror(string errmsg);
            virtual const char* what() const throw();
        private:
            string _errmsg;
    };
}

#endif /* XKERROR_HPP */

