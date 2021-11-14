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

#include "defines.h"
#include "ass/data.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_assParser
{
    subfx_ass_meta meta;

    fdsa_ptrMap *styles;

    fdsa_ptrVector *dialogs;
} subfx_assParser;

typedef struct subfx_assParser_api
{
    subfx_assParser *(*create)(const char *fileName,
                               const char *warningOut,
                               char *errMsg);

    subfx_exitstate (*destory)(subfx_assParser *parser);

    subfx_exitstate (*extendDialogs)(subfx_assParser *parser,
                                     char *errMsg);

    subfx_exitstate (*dialogIsExtended)(subfx_assParser *parser, bool *out);

} subfx_assParser_api;

#ifdef __cplusplus
}
#endif
