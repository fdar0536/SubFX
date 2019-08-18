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

#include "file.h"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

file::file(const char *ass_file, const char *ass_header) :
    pAssFile(new TCFX_AssFile),
    append(false)
{
    common();
    process_file(ass_file, ass_header);
}

file::~file()
{
    fclose(pAssFile->fp);
    delete pAssFile;
    pAssFile = nullptr;
}

//WriteAssFile(pyAssFile, ASS_BUF)
bool file::write_ass_file(py::list &ASS_BUF)
{
    if (!success)
    {
        return false;
    }

    char *assString;
    int size;

    if (py::len(ASS_BUF) == 0)
    {
        PyErr_SetString(PyExc_RuntimeWarning, "WriteAssFile warning, empty list 'ASS_BUF'\n");
        return false;
    }

    convert_ass_list_to_string(ASS_BUF, &assString, &size);
    fwrite(assString, sizeof(char), size, pAssFile->fp);
    delete[] assString;
    return true;
}

void file::reset(const char *ass_file, const char *ass_header)
{
    fclose(pAssFile->fp);
    delete pAssFile;
    pAssFile = nullptr;

    pAssFile = new TCFX_AssFile;
    success = false;
    append = false;

    process_file(ass_file, ass_header);
}

bool file::is_append() const
{
    if (!success)
    {
        return false;
    }

    return append;
}

/* protected */
bool file::check_if_file_exists(const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp)
    {
        fclose(fp);
        return true;
    }

    return false;
}

void file::convert_ass_list_to_string(py::list &assList, char **pAssString, int *pCount)
{
    int i, count, size, offset;
    char *assString;
    char **pAssLine;
    int *assLineSize;
    count = static_cast<int>(py::len(assList));
    assLineSize = (int *)malloc(count * sizeof(int));
    pAssLine = (char **)malloc(count * sizeof(char *));
    size = 0;
    for (i = 0; i < count; ++i)
    {
        pAssLine[i] = py::extract<char *>(assList[i]);
        assLineSize[i] = static_cast<int>(strlen(pAssLine[i]));
        size += assLineSize[i];
    }
    
    assString = (char *)malloc((size + 1) * sizeof(char));
    offset = 0;
    for (i = 0; i < count; ++i)
    {
        memcpy(assString + offset, pAssLine[i], assLineSize[i] * sizeof(char));
        offset += assLineSize[i];
    }

    assString[size] = '\0';
    free(pAssLine);
    free(assLineSize);
    *pAssString = assString;
    *pCount = size;
}

void file::process_file(const char *ass_file, const char *ass_header)
{
    if (ass_header != nullptr)
    {
        pAssFile->fp = fopen(ass_file, "wb");
        if (!pAssFile->fp)
        {
            delete pAssFile;
            pAssFile = nullptr;
            success = false;
            PyErr_SetString(PyExc_RuntimeError, "CreateAssFile error, failed to create the file!\n");
        }
        else
        {
            fwrite(ass_header, sizeof(char), strlen(ass_header), pAssFile->fp);
            success = true;
        }
    }
    else //append
    {
        if (!check_if_file_exists(ass_file))
        {
            PyErr_SetString(PyExc_RuntimeError, "AppendAssFile error, failed to open the file!\n");
            delete pAssFile;
            pAssFile = nullptr;
            success = false;
        }
        else
        {
            pAssFile->fp = fopen(ass_file, "ab");
            if (!pAssFile->fp)
            {
                delete pAssFile;
                pAssFile = nullptr;
                success = false;
                PyErr_SetString(PyExc_RuntimeError, "AppendAssFile error, failed to open the file!\n");
            }

            success = true;
            append = true;
        }
    }
}
