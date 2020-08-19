#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <iostream>
#include <iomanip>

#include <ctime>

#include "pybind11/cast.h"
#include "pybind11/attr.h"
#include "pybind11/stl.h"

#include "asslauncher.hpp"

namespace py = pybind11;

std::shared_ptr<AssLauncher>
AssLauncher::create() NOTHROW
{
    AssLauncher *ret(new (std::nothrow) AssLauncher());
    if (!ret)
    {
        return nullptr;
    }

    try
    {
#if (defined(_WIN32) && \
    PY_MAJOR_VERSION == 3 && \
    PY_MINOR_VERSION == 8 && \
    PY_MICRO_VERSION >= 0) // python >= 3.8.0 on windows
        /* import os
         * os.add_dll_directory(os.path.join(os.environ["SUBFX_HOME"], "bin"))
         */
        py::object os = py::module::import("os");
        os.attr("add_dll_directory")(os.attr("path").attr("join")(
                                     os.attr("environ")["SUBFX_HOME"],
                                     "bin"));
#endif
        ret->m_yutils = py::module::import("SubFX_YutilsPy");
    }
    catch (py::error_already_set &e)
    {
        std::cerr << e.what() << std::endl;
        delete ret;
        return nullptr;
    }

    return std::shared_ptr<AssLauncher>(ret);
}

int AssLauncher::exec(std::shared_ptr<ConfigParser> &assConfig) NOTHROW
{
    if (assConfig->getLogFileName() == "stdout")
    {
        m_logger = PROJ_NAMESPACE::Utils::Logger::create();
    }
    else
    {
        m_logger = PROJ_NAMESPACE::Utils::Logger::create(
                    assConfig->getLogFileName(),
                    assConfig->getLogFileName());
    }

    if (m_logger == nullptr)
    {
        std::cerr << "Fail to create logger" << std::endl;
        return 1;
    }

    if (getParser(assConfig))
    {
        return 1;
    }

    auto configs(assConfig->getConfigDatas());
    py::list dialogs(m_parser.attr("dialogs")());
    m_totalConfigs = configs.size();
    for (size_t i = 0; i < m_totalConfigs; ++i)
    {
        auto config(configs.at(i));
        if (execConfig(config, dialogs))
        {
            reset();
            std::cout << std::endl;
            return 1;
        }

        std::cout << std::endl;
        ++m_currentConfig;
    }

    auto meta(m_parser.attr("meta")());
    auto styles(m_parser.attr("styles")());

    // todo: find better way to solve this problem
    py::list assBuf(py::cast(m_assBuf));

    std::cout << "Writing output..." << std::endl;
    try
    {
        m_yutils.attr("AssWriter").attr("write")(
                    assConfig->getOutputFileName().c_str(),
                    meta,
                    styles,
                    assBuf);
    }
    catch (py::error_already_set &e)
    {
        m_logger->writeErr(e.what());
        return 1;
    }

    reset();

    return 0;
}

// private member functions

int AssLauncher::getParser(std::shared_ptr<ConfigParser> &assConfig) NOTHROW
{
    try
    {
        m_parser = m_yutils.attr("AssParser").attr("create")(
                    assConfig->getSubName());
        m_parser.attr("extendDialogs")();
    }
    catch (py::error_already_set &e)
    {
        pExecConfigError(e);
        return 1;
    }

    return 0;
}

void AssLauncher::reset()
{
    m_resString.clear();
    m_assBuf.clear();
    m_totalConfigs = 0;
    m_currentConfig = 1;
}

int AssLauncher::execConfig(std::shared_ptr<ConfigData> &config,
                            py::list &dialogs)
{
    py::object mainObj;
    try
    {
        py::object imp = py::module::import("importlib.util");
        py::object spec = imp.attr("spec_from_file_location")("main",
                                                    config->scriptName);
        mainObj = imp.attr("module_from_spec")(spec);
        spec.attr("loader").attr("exec_module")(mainObj);
        mainObj = mainObj.attr("SubFXMain");
    }
    catch (py::error_already_set &e)
    {
        pExecConfigError(e);
        return 1;
    }

    std::string modeName;
    switch (config->mode)
    {
    case SubMode::lines:
    {
        modeName = "line";
        break;
    }
    case SubMode::syls:
    {
        modeName = "syls";
        if (!m_parser.attr("isSylAvailable")())
        {
            pExecConfigWarning(modeName);
            modeName = "line";
        }

        break;
    }
    case SubMode::words:
    {
        modeName = "words";
        if (!m_parser.attr("isWordAvailable")())
        {
            pExecConfigWarning(modeName);
            modeName = "line";
        }

        break;
    }
    case SubMode::chars:
    {
        modeName = "chars";
        if (!m_parser.attr("isCharAvailable")())
        {
            pExecConfigWarning(modeName);
            modeName = "line";
        }

        break;
    }
    } // end switch (config->mode)

    size_t startLine(static_cast<size_t>(config->startLine));
    size_t endLine(config->endLine < 0 ? (dialogs.size() - 1)
                                       : static_cast<size_t>(config->endLine));

    if (startLine >= py::len(dialogs) ||
        endLine >= py::len(dialogs))
    {
        std::string lastError("Error: ");
        lastError += "\"startLine\" or \"endLine\" is greater than or equal to ";
        lastError += "dialogs\' count.\n";
        m_logger->writeErr(lastError);
        return 1;
    }

    py::list dialogsBak(dialogs);
    std::cout << "Current script: " << config->scriptName << std::endl;
    size_t totalLines(endLine - startLine + 1);
    for (size_t i = startLine; i <= endLine; ++i)
    {
        py::dict line(dialogsBak[i]);
        py::object resObj;
        if (modeName == "line")
        {
            try
            {
                resObj = mainObj(line, py::list());
                m_resString = resObj.cast<std::vector<std::string>>();
            }
            catch (py::error_already_set &e)
            {
                pExecConfigError(e);
                return 1;
            }
            catch (py::cast_error &e)
            {
                pExecConfigError(e);
                return 1;
            }

            m_assBuf.insert(m_assBuf.end(),
                            m_resString.begin(),
                            m_resString.end());
        }
        else
        {
            py::list list(line[modeName.c_str()]);
            for (size_t j = 0; j < py::len(list); ++j)
            {
                try
                {
                    resObj = mainObj(line, list[j]);
                    m_resString = resObj.cast<std::vector<std::string>>();
                }
                catch (py::error_already_set &e)
                {
                    pExecConfigError(e);
                    return 1;
                }
                catch (py::cast_error &e)
                {
                    pExecConfigError(e);
                    return 1;
                }

                m_assBuf.insert(m_assBuf.end(),
                                m_resString.begin(),
                                m_resString.end());
            } // end for j
        } // end if (modeName == "line")

        reportProgress(i + 1, totalLines);
    } // end for i

    return 0;
}

void AssLauncher::pExecConfigWarning(std::string &input)
{
    const char *now(getCurrentTime());
    std::string output;
    if (now)
    {
        output = std::string(now);
    }
    else
    {
        output = "CANNOT get current time!";
    }

    output += "\n";
    output += "Warning: \"";
    output += input;
    output += "\" mode is not available. ";
    output += "Fallback to \"line\" mode.\n";
    m_logger->writeErr(output);
    m_logger->writeErr("\n");
}

const char *AssLauncher::getCurrentTime()
{
    time_t rawtime;
    time(&rawtime);
    struct tm *now(localtime(&rawtime));
    if (!now)
    {
        return nullptr;
    }

    return asctime(now);
}

void AssLauncher::reportProgress(size_t currentLine, size_t totalLines)
{

    std::cout<< std::setiosflags(std::ios::fixed) << std::setprecision(3);
    double progress = static_cast<double>(currentLine);
    progress /= (static_cast<double>(totalLines));
    progress *= 100.;
    std::cout << "Current progress: " << progress;

    progress = static_cast<double>(m_currentConfig);
    progress /= static_cast<double>(m_totalConfigs);
    progress *= 100.;
    std::cout << "% Total progress: " << progress;
    std::cout << "%\r" << std::flush;
}
