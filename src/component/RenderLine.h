
#ifndef RENDERLINE_H_
#define RENDERLINE_H_
#include "Component.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Config.h"
#include "kep/Core.h"
namespace kelp
{
    class Camera;
    class RenderLine : public Component
    {
    public:
        kep::Vector3 m_p0;
        kep::Vector3 m_p1;
        kep::Vector3 m_colour;
        static std::vector<kep::Vector3> m_verticies;
        static std::vector<kep::Vector3> m_colours; //1 colour for 2 verts
        RenderLine(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _colour = kep::Vector3(0.0f, 1.0f, 0.0f));
        ~RenderLine();
        static void renderLines(Shader * _shader, Camera * _rc);
        
        void init();
        void update();
        void render();
    };
    
    class RenderLine2 : public Component
    {
    public:
        kep::Vector3 m_p0;
        kep::Vector3 m_p1;
        kep::Vector3 m_colour;
        static std::vector<kep::Vector3> m_verticies;
        static std::vector<kep::Vector3> m_colours; //1 colour for 2 verts
        RenderLine2(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _colour = kep::Vector3(0.0f, 1.0f, 0.0f));
        ~RenderLine2();
        static void renderLines(Shader * _shader, Camera * _rc);
        
        void init();
        void update();
        void render();
    };
};

#endif // RENDERLINE_H_
