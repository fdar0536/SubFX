/*
 * This file is part of SubFX,
 * Copyright (c) 2020 fdar0536
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "YutilsCpp"
#include "Utils"

class AssDumper
{
public:

    static std::shared_ptr<AssDumper> create();

    void dumpMeta();

    void dumpStyles();

    void dumpDialogs();

    std::shared_ptr<PROJ_NAMESPACE::Yutils::AssParser> getParser() const;

private:

    AssDumper() :
        m_logger(nullptr),
        m_parser(nullptr)
    {}

    std::shared_ptr<PROJ_NAMESPACE::Utils::Logger> m_logger;

    std::shared_ptr<PROJ_NAMESPACE::Yutils::AssParser> m_parser;

    void writeStyle(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssStyle> &);

    void writeDialog(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssDialog> &);

    void writeTextChunked(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssTextChunked> &);

    void writeSyls(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssSyl> &);

    void writeWords(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssWord> &);

    void writeChars(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssChar> &);

    void writeString(const std::string &prefix, std::string &data);

    template <class T>
    void writeData(const std::string &prefix, T data)
    {
        std::string out(prefix);
        out += std::to_string(data);
        out += "\n";
        m_logger->writeOut(out);
    }
};
