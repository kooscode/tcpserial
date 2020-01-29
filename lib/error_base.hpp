/*
 * Base Error Class for C++ Exceptions 
 * Copyright (C) 2019 TerraClear, Inc.
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
 * 
 * CREATED BY: Koos du Preez
 * MODIFIED BY: Koos du Preez - kdupreez@hotmail.com
*/
            
#ifndef ERROR_BASE_HPP
#define ERROR_BASE_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <exception>

namespace  kooscode
{

    class error_base: public std::exception
    {
        public:
            error_base(std::string errmsg);
            error_base(std::string errmsg, int error_number);
            virtual const char* what() const throw();
            
            static void stderr_redirect(std::string err_filename);
            
        private:
            std::string _errmsg;
    };
}

#endif /* ERROR_BASE_HPP */

