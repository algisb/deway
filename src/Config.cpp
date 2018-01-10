///
///  @file Config.cpp
///  @brief holds engine configuration settings

#include "Config.h"
using namespace kelp;

int Config::s_windowWidth = 1920;
int Config::s_windowHeight = 1080;
bool Config::s_capFrameRate = true;
int Config::s_targetFrameRate = 60;
float Config::s_targetDeltaT = (float)(1.0f/(float)s_targetFrameRate);
