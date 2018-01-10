#ifndef FORCEGENERATOR_H_
#define FORCEGENERATOR_H_

#include "RigidBody.h"
#include <vector>
namespace kep 
{
    class ForceGenerator
    {
    public:
        ForceGenerator();
        virtual ~ForceGenerator()=0;
        virtual void updateForce(RigidBody * _rigidBody, real _duration) = 0;
    };
    
    class ForceRegistry
    {
    public:
        struct ForceRegistration
        {
            RigidBody * rigidBody;
            ForceGenerator * fg;
        };
        
        typedef std::vector<ForceRegistration> Registry;
        Registry m_registrations;
        
        ForceRegistry();
        
        void updateForces(real _duration);
        
        
        void add(RigidBody * _rigidBody, ForceGenerator * _fg);
        void remove(RigidBody * _rigidBody, ForceGenerator * _fg);
        void clear();
        
    };

    
    
    ////////////////////
    //Derived generators
    ////////////////////
    class Gravity : public ForceGenerator
    {
    public:
        Vector3 m_gravity;
        Gravity(const Vector3 & _gravity);
        virtual void updateForce(RigidBody * _rigidBody, real _duration);
    };
    
    class LinearDrag : public ForceGenerator
    {
    public:
        //velocity drag coefficient
        real m_k1;
        //velocity squared drag coefficient
        real m_k2;

        LinearDrag(real _k1, real _k2);
        virtual void updateForce(RigidBody * _rigidBody, real _duration);
    };
    
    class AngularDrag : public ForceGenerator
    {
    public:
        //drag coefficient
        real m_k1;
        //squared drag coefficient
        real m_k2;
        
        AngularDrag(real _k1, real _k2);
        virtual void updateForce(RigidBody * _rigidBody, real _duration);
    };
    
    class Spring : public ForceGenerator
    {
    public:
        Vector3 m_localConnectionPoint;
        
        Vector3 m_otherConnectionPoint;
        real m_springConstant;
        real m_restLength;
        
        Spring(const Vector3 & _localConnectionPoint, const Vector3 & _otherConnectionPoint, real _springConstant, real _restLength);
        virtual void updateForce(RigidBody * _rigidBody, real _duration);
        
    };

}
#endif //FORCEGENERATOR_H_
