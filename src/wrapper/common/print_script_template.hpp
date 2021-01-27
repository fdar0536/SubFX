#pragma once

#include <string>

#include <cstdio>

#include "internal/basecommon.h"
#include "Utils"

int printAssPyScriptTemplate(std::string &fileName) NOTHROW
{
    std::shared_ptr<PROJ_NAMESPACE::Utils::Logger> logger;
    if (fileName.empty())
    {
        logger = PROJ_NAMESPACE::Utils::Logger::create();
    }
    else
    {
        logger = PROJ_NAMESPACE::Utils::Logger::create(fileName, fileName);
    }

    if (logger == nullptr)
    {
        return 1;
    }

    logger->writeOut("from SubFX_AssPy import *\n");
    logger->writeOut("\n");
    logger->writeOut("Yutils = SubFX_AssPy.Yutils");
    logger->writeOut("\n");
    logger->writeOut("def SubFXMain(line, currentSyllable):\n");
    logger->writeOut("    assBuf = []\n");
    logger->writeOut("\n");
    logger->writeOut("    # Todo: create your own awesome script here.\n");
    logger->writeOut("    # for example:\n");
    logger->writeOut("    timeStart = line[\"start_time\"]\n");
    logger->writeOut("    timeEnd = line[\"end_time\"]\n");
    logger->writeOut("    effect = fad(300, 300)\n");
    logger->writeOut("\n");
    logger->writeOut("    ass_main(assBuf, SubL(timeStart, "
                     "timeEnd, 0, 'default'), effect, line[\"text\"])\n");
    logger->writeOut("\n");
    logger->writeOut("    return assBuf");
    logger->writeOut("\n");

    return 0;
}
