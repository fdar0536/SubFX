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

#ifndef TCFXLIB_FILE_H
#define TCFXLIB_FILE_H
#pragma once

#include <cstring>

#include "std.h"

/**
 * TCFX_AssFile structure
 */
typedef struct _tcfx_ass_file {
    FILE *fp;
} TCFX_AssFile, *TCFX_pAssFile;

class file : public common
{
public:

    /**
     * Create an ASS file and returns the handler to the file.
     * @param ass_file
     * @param ass_header
     */
    //CreateAssFile(ass_file, ass_header)
    file(const char *ass_file, const char *ass_header = nullptr);

    //Finalize the ASS file (close the handler to the file and destroy contents assigned to it).
    //FinAssFile(pyAssFile)
    ~file();

    /**
     * Write ASS strings to ASS file.
     * @param ASS_BUF
     * @return TCFX_Py_Error_Code;
     */
    //WriteAssFile(pyAssFile, ASS_BUF)
    bool write_ass_file(py::list &ASS_BUF);

    void reset(const char *ass_file, const char *ass_header = nullptr);

    bool is_append() const;

protected:

    bool check_if_file_exists(const char *filename);

    void convert_ass_list_to_string(py::list &assList, char **pAssString, int *pCount);

private:

    TCFX_pAssFile pAssFile;

    bool append;

    void process_file(const char *ass_file, const char *ass_header = nullptr);
};

#endif    /* TCFXLIB_FILE_H */
