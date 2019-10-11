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
    
    void exec(SubFXAssInit *assConfig);
    
private:
    
    vector<string> resString;
    
    vector<string> assBuf;
    
    int execConfig(SubFXAssInit *assConfig,
                   shared_ptr<ConfigData> &config,
                   py::list &dialogs);
    
    void pExecConfigWarning(string &input);
    
    template<class T>
    void pExecConfigError(T &error)
    {
        lastError = error.what();
        success = false;
        fputs(lastError.c_str(), logFile);
    }
    
};

#endif // ASSLAUNCHER_HPP
