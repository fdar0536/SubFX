#include <cstdio>

#include <string>
#include <iostream>
#include <stdexcept>

#include "YutilsCpp"

int main()
{
    std::shared_ptr<PROJ_NAMESPACE::Yutils::AssParser> parser(nullptr);
    std::string fileName("in.ass");
    try
    {
        parser = PROJ_NAMESPACE::Yutils::AssParser::create(fileName);
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (parser == nullptr)
    {
        std::cerr << "Fail to create parser." << std::endl;
        return 1;
    }

    try
    {
        parser->extendDialogs();
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
