///
///  @file Shader.cpp
///  @brief GLSL shader utility class, dealing with loading and compiling shaders
#include "Shader.h"

using namespace kelp;

std::vector<Shader*> Shader::s_shaders = std::vector<Shader*>();
Shader::Shader(const char * _vertexShaderPath, const char * _fragmentShaderPath)
{
    s_shaders.push_back(this);
    BuildShader(m_shaderLocation, _vertexShaderPath, _fragmentShaderPath);
    m_shaderModelMatLocation = glGetUniformLocation(m_shaderLocation, "modelMat");
    m_shaderViewMatLocation = glGetUniformLocation(m_shaderLocation, "viewMat");
    m_shaderProjMatLocation = glGetUniformLocation(m_shaderLocation, "projMat");
    m_shaderColourLocation = glGetUniformLocation(m_shaderLocation, "colour");
    m_shaderText1SamplerLocation = glGetUniformLocation(m_shaderLocation, "tex1");
    m_shaderTexturedFlagLocation = glGetUniformLocation(m_shaderLocation, "textured");
    
}
Shader::~Shader()
{

}

std::string Shader::LoadShader(const char * _shaderPath)
{
    std::string content;
    std::ifstream fileStream(_shaderPath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << _shaderPath << ". File does not exist." << std::endl;
        return "";
    }
    char tmpChar[256];
    sprintf(tmpChar, "#define MAX_LIGHTS %d \n", MAX_LIGHTS);
    bool appendDefines = false;
    
    std::string line;
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n"); //NOTE: assumed that the first line is the version number (cuz it has to be othervise shader compilation errors)
        if(!appendDefines)//defines go after the version number
        {
            appendDefines = true;
            content.append(tmpChar);
        }
    }

    fileStream.close();
    return content;
}

bool Shader::CheckShaderCompiled(int _shaderLocation)
{
    GLint compiled;
    glGetShaderiv(_shaderLocation, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLsizei len;
        glGetShaderiv(_shaderLocation, GL_INFO_LOG_LENGTH, &len);
        GLchar* log = new GLchar[len + 1];
        glGetShaderInfoLog(_shaderLocation, len, &len, log);
        std::cout << "ERROR: Shader compilation failed: " << log << std::endl;
        delete[] log;

        return false;
    }
    return true;
}

void Shader::BuildShader(int &_shaderLocation, const char * _vertexShaderPath, const char * _fragmentShaderPath)
{
    std::string vs = LoadShader(_vertexShaderPath);
    const GLchar *vShaderText = vs.c_str();

    std::string fs = LoadShader(_fragmentShaderPath);
    const GLchar *fShaderText = fs.c_str();

    _shaderLocation = glCreateProgram();

    // Create the vertex shader
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    // Give GL the source for it
    glShaderSource(vShader, 1, &vShaderText, NULL);
    // Compile the shader
    glCompileShader(vShader);
    // Check it compiled and give useful output if it didn't work!
    if (!CheckShaderCompiled(vShader))
    {
        std::cout << "ERROR: failed to compile vertex shader" << std::endl;
        return;
    }
    // This links the shader to the program
    glAttachShader(_shaderLocation, vShader);



    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderText, NULL);
    glCompileShader(fShader);
    if (!CheckShaderCompiled(fShader))
    {
        std::cout << "ERROR: failed to compile fragment shader" << std::endl;
        return;
    }
    glAttachShader(_shaderLocation, fShader);
    glLinkProgram(_shaderLocation);
    GLint linked;
    glGetProgramiv(_shaderLocation, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLsizei len;
        glGetProgramiv(_shaderLocation, GL_INFO_LOG_LENGTH, &len);

        GLchar* log = new GLchar[len + 1];
        glGetProgramInfoLog(_shaderLocation, len, &len, log);
        std::cout << "ERROR: Shader linking failed: " << log << std::endl;
        delete[] log;

        return;
    }
}


ShaderMin::ShaderMin(const char* _vertexShaderPath, const char* _fragmentShaderPath) : Shader(_vertexShaderPath, _fragmentShaderPath)
{
}

ShaderMin::~ShaderMin()
{
    
}


ShaderDefault::ShaderDefault(const char* _vertexShaderPath, const char* _fragmentShaderPath) : Shader(_vertexShaderPath, _fragmentShaderPath)
{
}

ShaderDefault::~ShaderDefault()
{
    
}
