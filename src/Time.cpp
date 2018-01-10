///
///  @file Time.cpp
///  @brief time related utilities

#include "Time.h"

using namespace kelp;

float Time::s_deltaT = 0.0f;
float Time::s_realT = 0.0f;
int Time::s_frameRate = 0;

void Time::calc()
{
    float t = glfwGetTime();
    s_deltaT = t-s_realT;
    s_realT = t;//updated last frame time
    if(s_deltaT != 0.0f)//just incase float spills spahaghetti 
        s_frameRate = int(1.0f/s_deltaT);
}

void Time::calcEnd()
{
    if(Config::s_capFrameRate)
    {
        float t1 = glfwGetTime();
        float tt = t1 - s_realT;
        if(tt < Config::s_targetDeltaT)
            delay(Config::s_targetDeltaT-tt);
    }
}
void Time::delay(const float _t)
{
    float delay = _t;
    float elapsedT = 0.0f;
    float prevT = glfwGetTime();
    while(elapsedT < delay)
    {
        float t = glfwGetTime();
        float diff = t-prevT;
        elapsedT += diff;
        prevT = t;
    }
}


