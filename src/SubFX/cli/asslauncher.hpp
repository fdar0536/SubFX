#ifndef ASSLAUNCHER_HPP
#define ASSLAUNCHER_HPP

#include "internal/basecommon.h"

#include "pybind11/pybind11.h"

#include "../common/configparser.hpp"
#include "Utils"

namespace py = pybind11;

class AssLauncher
{
public:

    static std::shared_ptr<AssLauncher>
    create() NOTHROW;

    int exec(std::shared_ptr<ConfigParser> &assConfig) NOTHROW;

protected:

    AssLauncher() :
        m_assBuf(std::vector<std::string>()),
        m_resString(std::vector<std::string>()),
        m_totalConfigs(0),
        m_currentConfig(1),
        m_yutils(py::object()),
        m_parser(py::object())
    {}

private:

    std::vector<std::string> m_assBuf;

    std::vector<std::string> m_resString;

    size_t m_totalConfigs;

    size_t m_currentConfig;

    py::object m_yutils;

    py::object m_parser;

    std::shared_ptr<PROJ_NAMESPACE::Utils::Logger> m_logger;

    int getParser(std::shared_ptr<ConfigParser> &assConfig) NOTHROW;

    void reset();

    int execConfig(std::shared_ptr<ConfigData> &config,
                   py::list &dialogs);

    void pExecConfigWarning(std::string &input);

    template<class T>
    void pExecConfigError(T &error)
    {
        const char *now(getCurrentTime());
        if (now)
        {
            m_logger->writeErr(now);
        }
        else
        {
            m_logger->writeErr("CANNOT get current time!\n");
        }

        m_logger->writeErr(error.what());
        m_logger->writeErr("\n");
    }

    const char *getCurrentTime();

    void reportProgress(size_t currentLine, size_t totalLines);

};

#endif // ASSLAUNCHER_HPP
