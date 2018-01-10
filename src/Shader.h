#ifndef SHADER_H_
#define SHADER_H_

#include "GL/glew.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


namespace kelp
{
#define MAX_LIGHTS 50 //Get also defined in all shaders
    class Shader
    {
    public:
        static std::vector<Shader*> s_shaders;//mainly needed for lighting
        int m_shaderLocation;
        int m_shaderModelMatLocation;
        int m_shaderViewMatLocation;
        int m_shaderProjMatLocation;
        int m_shaderColourLocation;
        int m_shaderText1SamplerLocation;
        int m_shaderTexturedFlagLocation;
        Shader(const char * _vertexShaderPath,const char * _fragmentShaderPath);
        virtual ~Shader()=0;
        
        std::string LoadShader(const char *_shaderPath);
        bool CheckShaderCompiled(int _shaderLocation);
        void BuildShader(int &_shaderLocation, const char * _vertexShaderPath, const char * _fragmentShaderPath);

    };
    
    class ShaderMin : public Shader
     {
    public:
        
        ShaderMin(const char* _vertexShaderPath = "./shaders/minimal_v.glsl", const char* _fragmentShaderPath = "./shaders/minimal_f.glsl");
        ~ShaderMin();
        
    };
    class ShaderDefault : public Shader 
    {
    public:
        ShaderDefault(const char* _vertexShaderPath = "./shaders/default_v.glsl", const char* _fragmentShaderPath = "./shaders/default_f.glsl");
        ~ShaderDefault();
        
    };
    
};

#endif // SHADER_H_
