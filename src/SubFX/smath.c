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

#include <stdlib.h>
#include <math.h>

#include "smath.h"

#define PI \
    3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00

subfx_math *subfx_math_init()
{
    subfx_math *math = calloc(1, sizeof(subfx_math));
    if (!math)
    {
        return NULL;
    }

    math->rotate2d = subfx_math_rotate2d;
    math->rad = subfx_math_rad;
    math->deg = subfx_math_deg;
    math->random = subfx_math_random;

    return math;
}

double *subfx_math_rotate2d(double x, double y, double angle)
{
    double *ret = calloc(2, sizeof(double));
    if (!ret)
    {
        return NULL;
    }

    double ra = subfx_math_rad(angle);
    ret[0] = cos(ra) * x - sin(ra) * y;
    ret[1] = sin(ra) * x + cos(ra) * y;
    return ret;
}

// math.rad
double subfx_math_rad(double angle)
{
    return (angle * PI / 180.);
}

// math.deg
double subfx_math_deg(double r)
{
    return (r * 180. / PI);
}

// math.random
double subfx_math_random(double min, double max)
{
    return ((max - min) * rand() / (RAND_MAX + 1.0) + min);
}
