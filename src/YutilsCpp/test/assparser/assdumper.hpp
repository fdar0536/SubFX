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
