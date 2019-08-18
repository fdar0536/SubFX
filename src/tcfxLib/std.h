/* 
 *  Copyright (C) 2009-2011 milkyjing <milkyjing@gmail.com>
 *                2019 fdar0536
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef TCFXLIB_STD_H
#define TCFXLIB_STD_H
#pragma once

#include "boost/python.hpp"

#define TCFXLIB_VER_STR "0.1"

namespace py = boost::python;

/**
 * TCFX_Py_Error_Code structure that is in fact a Python long object.
 */
typedef py::long_ TCFX_Py_Error_Code;

class common
{
public:

    common() : success(false)
    {}

    ~common(){}

    bool is_success() const
    {
        return success;
    }

protected:

    bool success;
};

#endif    /* TCFXLIB_STD_H */

