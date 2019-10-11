#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <memory>
#include <cstdio>

#include "../../CoreLib/CoreLib.hpp"

using namespace std;

class Launcher
{
public:
    
    virtual ~Launcher();
    
    bool isSuccess() const;
    
    string getLastError() const;
    
protected:
    
    Launcher(const string &logFile, const string &outputFile);
    
    bool success;
    
    string lastError;
    
    FILE *logFile;
    
    shared_ptr<CoreFile> file;
    
private:
    
    void LauncherInit(const string &logFile, const string &outputFile);
};

#endif // LAUNCHER_HPP
