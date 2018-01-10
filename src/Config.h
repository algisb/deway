#ifndef CONFIG_H_
#define CONFIG_H_

namespace kelp
{
    class Config
    {
    public:
        static int s_windowWidth;
        static int s_windowHeight;
        static bool s_capFrameRate;
        static int s_targetFrameRate;
        static float s_targetDeltaT;
        
    };
    
};
#endif // CONFIG_H_
