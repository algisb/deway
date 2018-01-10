#include "Light.h"
#include <sstream>
#include "Camera.h"
using namespace kelp;

Light::Light(Shader * _shader, float _strength, kep::Vector3 _colour)
{
    
    m_shader = _shader;
    m_strength = _strength;
    m_colour = _colour;
    m_enabled = 1;
}

Light::~Light()
{
    //some sphaghetti with clearing lights incase deleted at runtime
    std::vector<Light*> lights = m_owner->m_world->m_lights;
    m_owner->m_world->m_lights.clear();
    for(int i = 0; i<lights.size(); i++)
        if(lights[i] == this)
            lights.erase(lights.begin()+i);
    
    for(int i = 0; i<lights.size(); i++)
        lights[i]->init();
    m_owner->m_world->m_lights = lights;
    glUseProgram(m_shader->m_shaderLocation);
    glUniform1i(m_shaderNumLightsLocation, m_owner->m_world->m_lights.size());
    //printf("light dest %d\n", m_owner->m_world->m_lights.size());
}
void Light::init()
{
    m_owner->m_world->m_lights.push_back(this);
    ////////////////////////////////////////////////////
    std::string nameInShader;
    std::stringstream ss;
    
    ss << m_owner->m_world->m_lights.size()-1;//TODO: no mechanism for deleteing lights sources
    std::string loc;
    ss >> loc;

    
    glUseProgram(m_shader->m_shaderLocation);
    nameInShader.clear();
    nameInShader = "light[" + loc + "].enabled";
    m_shaderEnabledLocation = glGetUniformLocation(m_shader->m_shaderLocation, nameInShader.c_str());

    nameInShader.clear();
    nameInShader = "light[" + loc + "].strength";
    m_shaderStrengthLocation = glGetUniformLocation(m_shader->m_shaderLocation, nameInShader.c_str());
    
    nameInShader.clear();
    nameInShader = "light[" + loc + "].colour";
    m_shaderColourLocation = glGetUniformLocation(m_shader->m_shaderLocation, nameInShader.c_str());
    
    nameInShader.clear();
    nameInShader = "light[" + loc + "].type";
    m_shaderTypeLocation = glGetUniformLocation(m_shader->m_shaderLocation, nameInShader.c_str());



    m_shaderNumLightsLocation = glGetUniformLocation(m_shader->m_shaderLocation, "numLights");
}

void Light::update()
{
    glUseProgram(m_shader->m_shaderLocation);
    glUniform1i(m_shaderEnabledLocation, m_enabled);
    glUniform1f(m_shaderStrengthLocation, m_strength);
    glUniform3fv(m_shaderColourLocation, 1, &m_colour.data[0]);
    glUniform1i(m_shaderNumLightsLocation, m_owner->m_world->m_lights.size());
}

void Light::render()
{
    
}


LightDirectional::LightDirectional(Shader * _shader, float _strength, kep::Vector3 _colour, kep::Vector3 _direction) : Light(_shader, _strength, _colour)
{
    m_direction = _direction;
}

LightDirectional::~LightDirectional()
{
}

void LightDirectional::init()
{
    Light::init();
    std::string nameInShader;
    std::stringstream ss;
    
    ss << m_owner->m_world->m_lights.size()-1;
    std::string loc;
    ss >> loc;

    
    //glUseProgram(m_shader->m_shaderLocation);
    nameInShader.clear();
    nameInShader = "light[" + loc + "].direction";
    m_shaderDirectionLocation = glGetUniformLocation(m_shader->m_shaderLocation, nameInShader.c_str());
}

void LightDirectional::update()
{
    Light::update();
    kep::Matrix4 viewMat = m_owner->m_world->m_renderCamera->m_viewMat;
    kep::Vector3 direction = kep::Matrix3(viewMat).transpose() * m_direction.normalized();
    glUniform3fv(m_shaderDirectionLocation, 1, &direction.data[0]);
    glUniform1i(m_shaderTypeLocation, 0);//0 is directional light inside the shader
}

void LightDirectional::render()
{
    
}





LightPoint::LightPoint(Shader * _shader, float _strength, kep::Vector3 _colour) : Light(_shader, _strength, _colour)
{
    
}

LightPoint::~LightPoint()
{
}
void LightPoint::init()
{
    Light::init();
    
    m_transform = m_owner->getComponent<Transform>();
    std::string nameInShader;
    std::stringstream ss;
    
    ss << m_owner->m_world->m_lights.size()-1;
    std::string loc;
    ss >> loc;

    nameInShader.clear();
    nameInShader = "light[" + loc + "].position";
    m_shaderPositionLocation = glGetUniformLocation(m_shader->m_shaderLocation, nameInShader.c_str());
}

void LightPoint::update()
{
    Light::update();
    kep::Matrix4 viewMat = m_owner->m_world->m_renderCamera->m_viewMat;
    kep::Vector3 position = viewMat.transpose() * m_transform->m_position;
    glUniform3fv(m_shaderPositionLocation, 1, &position.data[0]);
    glUniform1i(m_shaderTypeLocation, 1);//1 is point light inside the shader
    
}

void LightPoint::render()
{
    
}
