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

#include "error_base.hpp"

namespace kooscode
{
    error_base::error_base(std::string errmsg)
    {
        _errmsg = errmsg;
    }

    error_base::error_base(std::string errmsg, int error_number)
    {
        std::stringstream strstrm;
        strstrm << errmsg << " : " <<  std::strerror(error_number);
        _errmsg = strstrm.str();
    }

    const char* error_base::what() const throw()
    {
        return _errmsg.c_str();
    }
    
    void error_base::stderr_redirect(std::string err_filename)
    {
        freopen (err_filename.c_str(), "w", stderr);
    }
}