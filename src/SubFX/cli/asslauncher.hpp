#ifndef ASSLAUNCHER_HPP
#define ASSLAUNCHER_HPP

#include "pybind11/pybind11.h"

#include "../common/subfxassinit.hpp"
#include "launcher.hpp"

using namespace std;
namespace py = pybind11;

class AssLauncher : public Launcher
{
public:
    
    AssLauncher(SubFXAssInit *assConfig);
    
    int exec(SubFXAssInit *assConfig);
    
private:
    
    vector<string> resString;
    
    vector<string> assBuf;
    
    size_t totalConfigs;
    
    size_t currentConfig;
    
    void reset();
    
    int execConfig(SubFXAssInit *assConfig,
                   shared_ptr<ConfigData> &config,
                   py::list &dialogs);
    
    void pExecConfigWarning(string &input);
    
    template<class T>
    void pExecConfigError(T &error)
    {
        lastError = error.what();
        success = false;
        
        const char *now(getCurrentTime());
        if (now)
        {
            fputs(now, logFile);
        }
        else
        {
            fputs("CANNOT get current time!", logFile);
        }
        
        fputs(lastError.c_str(), logFile);
        fputs("\n", logFile);
    }
    
    const char *getCurrentTime();
    
    void reportProgress(size_t currentLine, size_t totalLines);
    
};

#endif // ASSLAUNCHER_HPP
