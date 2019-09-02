#include "file.hpp"

CoreFile::CoreFile(const string &fileName, const string &assHeader)
{
    if (assHeader.size() == 0)
    {
        try
        {
            file.open(fileName, fstream::app | fstream::out);
        }
        catch (...)
        {
            throw invalid_argument("CANNOT open file.");
        }
        
        append = true;
        this->assHeader = "";
    }
    else
    {
        try
        {
            file.open(fileName, fstream::trunc | fstream::out);
        }
        catch (...)
        {
            throw invalid_argument("CANNOT open file.");
        }
        
        append = false;
        this->assHeader = assHeader;
    }
}

CoreFile::~CoreFile()
{
    file.close();
}

void CoreFile::writeAssFile(vector<string> &assBuf)
{
    if (assHeader.size() != 0)
    {
        file << assHeader;
    }

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << endl;
    }
}

void CoreFile::reset(const string &fileName, const string &assHeader)
{
    fstream newFile;
    if (assHeader.size() == 0)
    {
        try
        {
            newFile.open(fileName, fstream::app | fstream::out);
        }
        catch (...)
        {
            throw invalid_argument("CANNOT reset file!");
        }

        append = true;
        this->assHeader = "";
    }
    else
    {
        try
        {
            newFile.open(fileName, fstream::trunc | fstream::out);
        }
        catch (...)
        {
            throw invalid_argument("CANNOT reset file!");
        }

        append = false;
        this->assHeader = assHeader;
    }

    file.close();
    file.swap(newFile);
}

bool CoreFile::isAppend()
{
    return append;
}