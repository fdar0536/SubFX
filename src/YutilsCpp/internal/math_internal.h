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

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void bezier2(double pct, double **pts, int is3D, double *dst);

void bezier3(double pct, double **pts, int is3D, double *dst);

void bezier4(double pct, double **pts, int is3D, double *dst);

void bezierN(double pct, double **pts, size_t ptsCount,
             int is3D, double *dst);

double fac(double n);

#ifdef __cplusplus
}
#endif
