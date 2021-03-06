/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "Utils"

#define PI \
    3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00

using namespace PROJ_NAMESPACE::Utils;

SYMBOL_SHOW std::pair<double, double>
Math::rotate2d(double x, double y, double angle) NOTHROW
{
    double ra(rad(angle));
    return std::make_pair(cos(ra) * x - sin(ra) * y, sin(ra) * x + cos(ra) * y);
}

SYMBOL_SHOW double Math::rad(double angle) NOTHROW
{
    return (angle * PI / 180.);
}

SYMBOL_SHOW double Math::deg(double r) NOTHROW
{
    return (r * 180. / PI);
}

SYMBOL_SHOW double Math::random(double min, double max) NOTHROW
{
    return ((max - min) * rand() / (RAND_MAX + 1.0) + min);
}
