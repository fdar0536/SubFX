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

using namespace std;

namespace py = pybind11;

AssLauncher::AssLauncher(SubFXAssInit *assConfig) :
    Launcher(assConfig->getLogFileName(), assConfig->getOutputFileName()),
    resString(vector<string>()),
    assBuf(vector<string>()),
    totalConfigs(0),
    currentConfig(1)
{}

int AssLauncher::exec(SubFXAssInit *assConfig)
{
    auto parser(assConfig->getParser());
    auto configs(assConfig->getConfigDatas());
    auto dialogs(parser->dialogs());
    totalConfigs = configs.size();
    for (size_t i = 0; i < totalConfigs; ++i)
    {
        auto config(configs.at(i));
        if (execConfig(assConfig, config, dialogs))
        {
            reset();
            cout << endl;
            return 1;
        }
        
        cout << endl;
        ++currentConfig;
    }
    
    auto meta(parser->getMetaPtr());
    auto styles(parser->getStyleData());
    
    cout << "Writing output..." << endl;
    file->writeAssFile(meta, styles, assBuf);
    reset();
    
    success = true;
    lastError = "";
    return 0;
}

// private member function
void AssLauncher::reset()
{
    resString.clear();
    assBuf.clear();
    totalConfigs = 0;
    currentConfig = 1;
}

int AssLauncher::execConfig(SubFXAssInit *assConfig,
                            shared_ptr<ConfigData> &config,
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
    
    string modeName;
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
        if (!assConfig->isSylAvailable())
        {
            pExecConfigWarning(modeName);
            modeName = "line";
        }
        
        break;
    }
    case SubMode::words:
    {
        modeName = "words";
        if (!assConfig->isWordAvailable())
        {
            pExecConfigWarning(modeName);
            modeName = "line";
        }
        
        break;
    }
    case SubMode::chars:
    {
        modeName = "chars";
        if (!assConfig->isCharAvailable())
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
        success = false;
        lastError = "Error: ";
        lastError += "\"startLine\" or \"endLine\" is greater than or equal to ";
        lastError += "dialogs\' count.";
        fputs(lastError.c_str(), logFile);
        return 1;
    }
    
    py::list dialogsBak(dialogs);
    cout << "Current script: " << config->scriptName << endl;
    size_t totalLines(endLine - startLine + 1);
    for (size_t i = startLine; i <= endLine; ++i)
    {
        py::dict line(dialogsBak[i]);
        py::object resObj;
        if (modeName == "line")
        {
            PyDict_DelItemString(line.ptr(), "syls");
            PyDict_DelItemString(line.ptr(), "words");
            PyDict_DelItemString(line.ptr(), "chars");
            
            try
            {
                resObj = mainObj(line, py::list());
                resString = resObj.cast<vector<string>>();
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
            
            assBuf.insert(assBuf.end(), resString.begin(), resString.end());
        }
        else
        {
            py::list list(line[modeName.c_str()]);
            PyDict_DelItemString(line.ptr(), "syls");
            PyDict_DelItemString(line.ptr(), "words");
            PyDict_DelItemString(line.ptr(), "chars");
            
            for (size_t j = 0; j < py::len(list); ++j)
            {
                try
                {
                    resObj = mainObj(line, list[j]);
                    resString = resObj.cast<vector<string>>();
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
                
                assBuf.insert(assBuf.end(),
                              resString.begin(),
                              resString.end());
            } // end for j
        } // end if (modeName == "line")
        
        reportProgress(i + 1, totalLines);
    } // end for i
    
    return 0;
}

void AssLauncher::pExecConfigWarning(string &input)
{
    const char *now(getCurrentTime());
    string output;
    if (now)
    {
        output = string(now);
    }
    else
    {
        output = "CANNOT get current time!";
    }
    
    output += "\n";
    output += "Warning: \"";
    output += input;
    output += "\" mode is not available. ";
    output += "Fallback to \"line\" mode.";
    fputs(output.c_str(), logFile);
    fputs("\n", logFile);
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
    
    cout<< setiosflags(ios::fixed) << setprecision(3);
    double progress = static_cast<double>(currentLine);
    progress /= (static_cast<double>(totalLines));
    progress *= 100.f;
    cout << "Current progress: " << progress;
    
    progress = static_cast<double>(currentConfig);
    progress /= static_cast<double>(totalConfigs);
    progress *= 100.f;
    cout << "% Total progress: " << progress;
    cout << "%\r" << flush;
}
