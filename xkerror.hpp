/*
 * Base Error Class for C++ Exceptions 
 * Copyright (C) 2017 Jacobus du Preez / kdupreez@hotmail.com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
            
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

