#include <iostream>
#include <fstream>
#include <string>

#include <cstring>

#include "pybind11/embed.h"
#include "cxxopts.hpp"
#include "asslauncher.hpp"
#include "../common/print_script_template.hpp"
#include "config.h"

namespace py = pybind11;

template<class T> int handleError(T &e);

int main(int argc, char **argv)
{
    std::string name(PROJ_NAME);
    name += "CLI";
    std::string desc("A wrapper for ");
    desc += PROJ_NAME;
    desc += " in cli.";
    cxxopts::Options options(name, desc);

    options.add_options()
            ("h,help", "Print usage.")

            ("v,version", "Print version.")

            ("t,template",
             "Print script template to stdout or a file. Default is stdout.",
             cxxopts::value<std::string>()->default_value(""))

            ("e,example",
             "Print an example config file to stdout or a file. "
             "Default is stdout.",
             cxxopts::value<std::string>()->default_value(""))

            ("c,check",
             "Check config file is valid or not.",
             cxxopts::value<std::string>())

            ("p,parse",
             "Parse config file and output subtitle file.",
             cxxopts::value<std::string>());

    if (argc == 1)
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (argc > 3)
    {
        std::cerr << "Too many arguments!" << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    try
    {
#ifdef _MSC_VER // fix me, this is caused by cxxopts, and just work around
        char **argvs(argv);
#else
        const char **argvs(const_cast<const char **>(argv));
#endif
        auto result = options.parse(argc, argvs);

        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("version"))
        {
            std::cout << PROJ_VERSION << std::endl;
            return 0;
        }

        if (result.count("template"))
        {
            std::string outFile(result["template"].as<std::string>());
            return printAssPyScriptTemplate(outFile);
        }

        if (result.count("example"))
        {
            std::string output(result["template"].as<std::string>());
            if (output.empty())
            {
                ConfigParser::printExample(std::cout);
            }
            else
            {
                std::fstream out;
                out.open(output, std::fstream::trunc | std::fstream::out);
                if (out.fail())
                {
                    std::cerr << "Fail to open output file." << std::endl;
                    return 1;
                }

                ConfigParser::printExample(out);

                out.close();
            }
            return 0;
        }

        if (result.count("check"))
        {
            std::string target(result["check"].as<std::string>());
            try
            {
                ConfigParser::checkConfig(target);
            }
            catch (std::invalid_argument &e)
            {
                return handleError(e);
            }
            catch (std::runtime_error &e)
            {
                return handleError(e);
            }

            std::cout << "All ok." << std::endl;
            return 0;
        }

        if (result.count("parse"))
        {
            py::scoped_interpreter guard{};

            std::string jsonFileName(result["parse"].as<std::string>());
            std::shared_ptr<ConfigParser> assConfig;
            try
            {
                assConfig = ConfigParser::create(jsonFileName);
            }
            catch (std::invalid_argument &e)
            {
                return handleError(e);
            }

            if (assConfig == nullptr)
            {
                std::cout << "Fail to allocate memory." << std::endl;
                return 1;
            }

            std::shared_ptr<AssLauncher> assLauncher(AssLauncher::create());
            int ret(assLauncher->exec(assConfig));
            if (ret)
            {
                std::cerr << "Something error happened. "
                             "Please see log for details.";
                std::cerr << std::endl;
            }

            return ret;
        }
    }
    catch (cxxopts::OptionException &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Fail to parse command line argument(s)." << std::endl;
        return 1;
    }

    return 0;
}

template<class T> int handleError(T &e)
{
    std::cout << e.what() << std::endl;
    return 1;
}
