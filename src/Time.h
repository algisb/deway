#ifndef KTIME_H_
#define KTIME_H_

#include <GLFW/glfw3.h>
#include "Config.h"

namespace kelp
{
    class Time
    {
    public:
        static float s_deltaT;
        static float s_realT;
        static int s_frameRate;
        
        static void calc();
        static void calcEnd();
        static void delay(const float _t);
        
    };
};


#endif // KTIME_H_
