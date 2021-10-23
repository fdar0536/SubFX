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

#include "include/internal/fonthandle.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_exitstate subfx_fontHandle_init(subfx_fontHandle_api *);

subfx_fontHandle *subfx_fontHandle_create(const char *family,
                                          bool bold,
                                          bool italic,
                                          bool underline,
                                          bool strikeout,
                                          int32_t size,
                                          double xscale, // 1.
                                          double yscale, // 1.
                                          double hspace, // 0.
                                          char *errMsg);

subfx_exitstate subfx_fontHandle_destroy(subfx_fontHandle *);

double *subfx_fontHandle_metrics(subfx_fontHandle *);

double *subfx_fontHandle_text_extents(subfx_fontHandle *fontHandle,
                                      const char *text);

char *subfx_fontHandle_text_to_shape(subfx_fontHandle *fontHandle,
                                     const char *text, char *errMsg);

#ifdef __cplusplus
}
#endif
