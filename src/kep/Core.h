#ifndef CORE_KEP_H_
#define CORE_KEP_H_

#include <stdio.h>
#include <stdlib.h>
//#include <cmath>
#include "precision.h"
namespace kep
{
    const real PI = 3.14159265359f;
    class Vector3
    {
    public:
        union
        {
            real data[3];
            struct
            {
                real x;
                real y;
                real z;
            };

        };
    private:
        //real pad; //fucks with opengl when passing stuff to buffers
    public:
        Vector3() : x(0) , y(0), z(0){}
        Vector3(real _x, real _y, real _z) : x(_x), y(_y), z(_z){}
        ~Vector3(){}
        
        void operator *=(const real _value);
        Vector3 operator *(const real _value);
        void operator +=(const Vector3 & _v);
        
        Vector3 operator +(const Vector3 & _v);
        
        void operator -=(const Vector3 & _v);
        
        Vector3 operator -(const Vector3 & _v);
        
        void invert();
        
        real magnitude();
        
        real squareMagnitude();
        void normalize();
        Vector3 normalized();
        
        void addScaledVector(const Vector3 & _vector, const real _scale);
        
        Vector3 componentProduct(const Vector3 & _vector);
        
        real scalarProduct(const Vector3 & _vector);
        
        real operator * (const Vector3 & _vector);

        Vector3 vectorProduct(const Vector3 & _vector);

        void operator %=(const Vector3 & _vector);
        
        Vector3 operator %(const Vector3 & _vector);
        
        bool operator ==(const Vector3 & _vector);
        
        void dump();
    };
    
    class Quaternion
    {
    public:
        union
        {
            struct
            {
                real r;
                
                real i;
                real j;
                real k;
            };
            real data[4];
        };
        
        Quaternion(real _r = 0.0f, real _i = 0.0f, real _j = 0.0f, real _k = 0.0f);
        Quaternion(Vector3 _axis, real _angle);
        ~Quaternion();
        void normalize();
        void operator *= (const Quaternion &_multiplier);
        Quaternion operator * (const Quaternion &_multiplier);
        void rotateByVector(const Vector3 &_vector);
        void addScaledVector(const Vector3 &_vector, real _scale);
        void setEuler(Vector3 _axis, real _angle);
        static Quaternion toQuaternion(real _pitch, real _roll, real _yaw);
    };
    
    class Matrix4;
    class Matrix3
    {
    public:
        union
        {
            real d[3][3];
            real data[9];
        };
        
        Matrix3(    
        real _d0 = 1.0f, real _d1 = 0.0f, real _d2 = 0.0f,
        real _d3 = 0.0f, real _d4 = 1.0f, real _d5 = 0.0f,
        real _d6 = 0.0f, real _d7 = 0.0f, real _d8 = 1.0f
        );
        Matrix3(Matrix4 _m);
        ~Matrix3();
        
        Matrix3 operator*(const Matrix3 &_o) const;
        
        Vector3 operator*(const Vector3 &_vector) const;
        
        real determinant() const;
        Matrix3 transpose() const;
        Matrix3 cofactor() const;
        void setInverse(const Matrix3 & _m);
        Matrix3 inverse() const;
        void invert();
        void setOrientation(const Quaternion &_q);
        
        void setComponents(const Vector3 &_comp0, const Vector3 &_comp1, const Vector3 &_comp2);
        Vector3 getAxisVector(int i) const;
        void setSkewSymmetric(const Vector3& _vector);
        void operator*=(const Matrix3 &_o);
        void operator+=(const Matrix3 &_o);
        void dump();
    };
    
    class Matrix4
    {
    public:
        union
        {
            real d[4][4];
            real data[16];
        };
        Matrix4(    
        real _d0 = 1.0f, real _d1 = 0.0f, real _d2 = 0.0f, real _d3 = 0.0f,
        real _d4 = 0.0f, real _d5 = 1.0f, real _d6 = 0.0f, real _d7 = 0.0f,
        real _d8 = 0.0f, real _d9 = 0.0f, real _d10 = 1.0f, real _d11 = 0.0f,
        real _d12 = 0.0f, real _d13 = 0.0f, real _d14 = 0.0f, real _d15 = 1.0f
        );
        
        ~Matrix4();
        Vector3 operator*(const Vector3 &_vector) const;
        Matrix4 operator*(const Matrix4 &_o) const;
        real determinant() const;
        Matrix4 transpose() const;
        Matrix4 cofactor() const;
        void setInverse(const Matrix4 &_m);
        Matrix4 inverse() const;
        void invert();
        
        void setOrientationAndPos(const Quaternion &_q, const Vector3 &_pos);
        
        Vector3 transformInverseDirection(const Vector3 &_vector) const;
        Vector3 transformDirection(const Vector3 &_vector)const;
        
        Vector3 getAxisVector(int i) const;
        
        void dump();
        
    };
    
    inline Vector3 localToWorld(const Vector3 &_local, const Matrix4 &_transform)
    {
        return _transform * _local;
    }
    inline Vector3 worldToLocal(const Vector3 &_world, const Matrix4 &_transform)
    {
        Matrix4 inverseTransform;
        inverseTransform.setInverse(_transform);
        
        return inverseTransform * _world;
    }
    inline Matrix4 perspectiveProjection(real _fov, real _width, real _height, real _near, real _far)
    {
        real aspect = _width/_height;
        real top = tan((_fov/2)*(kep::PI/180.0f));
        real bottom = -top;
        real right = top * aspect;
        real left = -top * aspect;
        return Matrix4((2*_near)/(right-left), 0, (right + left)/(right - left), 0,
                       0, (2*_near)/(top-bottom),(top + bottom)/(top - bottom), 0,
                       0, 0, -(_far + _near)/(_far - _near) ,-(2*_far*_near)/(_far - _near),
                       0, 0, -1.0f, 0);
    }
    inline real dot(Vector3 _v0, Vector3 _v1)
    {
        return (_v0.x*_v1.x + _v0.y*_v1.y + _v0.z*_v1.z);
    }
    inline Vector3 cross(Vector3 _v0, Vector3 _v1)
    {
        return Vector3(
            _v0.y*_v1.z - _v0.z*_v1.y,
            _v0.z*_v1.x - _v0.x*_v1.z,
            _v0.x*_v1.y - _v0.y*_v1.x
        );
    }
};
#endif // CORE_KEP_H_
