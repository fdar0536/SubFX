#ifndef ASSLAUNCHER_HPP
#define ASSLAUNCHER_HPP

#include "pybind11/pybind11.h"

#include "../common/subfxassinit.hpp"
#include "launcher.hpp"

namespace py = pybind11;

class AssLauncher : public Launcher
{
public:

    static std::pair<std::shared_ptr<AssLauncher>, const char *>
    create(std::shared_ptr<SubFXAssInit> &assConfig);

    int exec(std::shared_ptr<SubFXAssInit> &assConfig);

protected:

    AssLauncher() :
        Launcher(),
        resString(std::vector<std::string>()),
        assBuf(std::vector<std::string>()),
        totalConfigs(0),
        currentConfig(1)
    {}

private:

    std::vector<std::string> resString;

    std::vector<std::string> assBuf;

    size_t totalConfigs;

    size_t currentConfig;

    void reset();

    int execConfig(std::shared_ptr<SubFXAssInit> &assConfig,
                   std::shared_ptr<ConfigData> &config,
                   py::list &dialogs);

    void pExecConfigWarning(std::string &input);

    template<class T>
    void pExecConfigError(T &error)
    {
        std::string lastError(error.what());

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
