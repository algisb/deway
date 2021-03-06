#ifndef AABB_DEWAY_H_
#define AABB_DEWAY_H_
#include "Core.h"
#include "Triangle.h"
#include "Span.h"
#include "Voxel.h"
namespace deway
{
#define FINDMINMAX(x0,x1,x2,min,max) \
min = max = x0;   \
if(x1<min) min=x1;\
if(x1>max) max=x1;\
if(x2<min) min=x2;\
if(x2>max) max=x2;

#define AXISTEST(_axis) \
{\
    float p[3];\
    p[0] = kep::dot(_axis, tp[0]);\
    p[1] = kep::dot(_axis, tp[1]);\
    p[2] = kep::dot(_axis, tp[2]);\
    float min,max;\
    FINDMINMAX(p[0], p[1], p[2], min, max);\
    float r = hs.x* std::abs(_axis.x) + hs.y * std::abs(_axis.y) + hs.z * std::abs(_axis.z);\
    if(min>r || max<-r) return 0;\
}
    class Triangle;
    class AABB 
    {
    public:
        kep::Vector3 c;//center
        kep::Vector3 hs;//halfsize
        AABB(kep::Vector3 _center = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _halfSize = kep::Vector3(0.0f, 0.0f, 0.0f));
        ~AABB();
        inline int triTest(Triangle * _t)
        {
            //shift the box to origin
            kep::Vector3 tp[3] = {_t->p[0], _t->p[1], _t->p[2]};
            
            for(int i = 0; i < 3; i++)
                tp[i] = tp[i] - c;
                
            //t.n = kep::cross(e[0], e[1]);
            //t.n.normalize();
            
            kep::Vector3 axis[3];
            axis[0] = kep::Vector3(1.0f, 0.0f, 0.0f);
            AXISTEST(axis[0]);
            axis[1] = kep::Vector3(0.0f, 1.0f, 0.0f);
            AXISTEST(axis[1]);
            axis[2] = kep::Vector3(0.0f, 0.0f, 1.0f);
            AXISTEST(axis[2]);
            
            AXISTEST(_t->n);
            
            kep::Vector3 e[3];
            e[0] = tp[1] - tp[0];
            e[1] = tp[2] - tp[1];
            e[2] = tp[0] - tp[2];
            
            
            AXISTEST(kep::cross(axis[0], e[0]));
            AXISTEST(kep::cross(axis[0], e[1]));
            AXISTEST(kep::cross(axis[0], e[2]));
            
            AXISTEST(kep::cross(axis[1], e[0]));
            AXISTEST(kep::cross(axis[1], e[1]));
            AXISTEST(kep::cross(axis[1], e[2]));
            
            AXISTEST(kep::cross(axis[2], e[0]));
            AXISTEST(kep::cross(axis[2], e[1]));
            AXISTEST(kep::cross(axis[2], e[2]));
            
            return 1;
        }
        inline int aabbTest(AABB * _aabb)
        {
            float max0[3], min0[3], 
                  max1[3], min1[3];
                  
            for(int i = 0; i<3; i++)
            {
                max0[i] = c.data[i] + hs.data[i]; 
                min0[i] = c.data[i] - hs.data[i];
                max1[i] = _aabb->c.data[i] + _aabb->hs.data[i]; 
                min1[i] = _aabb->c.data[i] - _aabb->hs.data[i];
            }
              if((min0[0] <= max1[0] && max0[0] >= min1[0]) && (min0[1] <= max1[1] && max0[1] >= min1[1]) && (min0[2] <= max1[2] && max0[2] >= min1[2]))
                  return 1;
              else
                  return 0;
        }
        inline int spanTest(Span * _span)
        {
            AABB * aabb = _span->m_voxels[0]->aabb;
            float max0[2], min0[2], 
                  max1[2], min1[2];
                  

            max0[0] = c.data[0] + hs.data[0]; 
            min0[0] = c.data[0] - hs.data[0];
            max1[0] = aabb->c.data[0] + aabb->hs.data[0]; 
            min1[0] = aabb->c.data[0] - aabb->hs.data[0];
            
            max0[1] = c.data[2] + hs.data[2]; 
            min0[1] = c.data[2] - hs.data[2];
            max1[1] = aabb->c.data[2] + aabb->hs.data[2]; 
            min1[1] = aabb->c.data[2] - aabb->hs.data[2];
            
            if((min0[0] <= max1[0] && max0[0] >= min1[0]) && (min0[1] <= max1[1] && max0[1] >= min1[1]) )
                return 1;
            else
                return 0;
        }
    };
};

#endif //AABB_DEWAY_H_
