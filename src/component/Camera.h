#ifndef CAMERA_H_
#define CAMERA_H_

#include "Component.h"
#include "kep/Core.h"
#include "Config.h"
#include "Transform.h"
namespace kelp
{
    class Camera : public Component
    {
    public:
        Transform * m_transform;
        kep::Matrix4 m_projectionMat;
        kep::Matrix4 m_viewMat;
        
        kep::Vector3 m_up;  
        kep::Vector3 m_front;
        kep::Vector3 m_left;
        float m_pitchAngle;
        float m_yawAngle;
        bool m_possessed;
        
        Camera(kep::Vector3 _up, kep::Matrix4 _projectionMat, bool _possessed);
        ~Camera();
        
        void init();
        void update();
        void render();
        
        void possessedControls();
        
        void setAsRenderCamera();
    };
};


#endif // CAMERA_H_
