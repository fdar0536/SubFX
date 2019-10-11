#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include "pybind11/cast.h"
#include "pybind11/attr.h"
#include "pybind11/stl.h"

#include "asslauncher.hpp"

using namespace std;

namespace py = pybind11;

AssLauncher::AssLauncher(SubFXAssInit *assConfig) :
    Launcher(assConfig->getLogFileName(), assConfig->getOutputFileName()),
    resString(vector<string>()),
    assBuf(vector<string>())
{}

void AssLauncher::exec(SubFXAssInit *assConfig)
{
    auto parser(assConfig->getParser());
    auto configs(assConfig->getConfigDatas());
    auto dialogs(parser->dialogs());
    for (size_t i = 0; i < configs.size(); ++i)
    {
        auto config(configs.at(i));
        if (execConfig(assConfig, config, dialogs))
        {
            resString.clear();
            assBuf.clear();
            return;
        }
    }
    
    auto meta(parser->getMetaPtr());
    auto styles(parser->getStyleData());
    file->writeAssFile(meta, styles, assBuf);
    
    resString.clear();
    assBuf.clear();
    
    success = true;
    lastError = "";
}

// private member function
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
        mainObj = mainObj.attr("TCFXMain");
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
                resObj = mainObj(line, py::list(), i);
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
                    resObj = mainObj(line, list[j], i);
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
            } // end for j
        } // end if (modeName == "line")
    } // end for i
    
    return 0;
}

void AssLauncher::pExecConfigWarning(string &input)
{
    string output("Warning: \"");
    output += input;
    output += "\" mode is not available. ";
    output += "Fallback to \"line\" mode.";
    fputs(output.c_str(), logFile);
}
