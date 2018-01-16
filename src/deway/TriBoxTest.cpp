#include "TriBoxTest.h"


//might need to remove abs
#define AXISTEST(_axis) \
{\
    float p[3];\
    p[0] = kep::dot(_axis, t.p[0]);\
    p[1] = kep::dot(_axis, t.p[1]);\
    p[2] = kep::dot(_axis, t.p[2]);\
    float min,max;\
    FINDMINMAX(p[0], p[1], p[2], min, max);\
    float r = _v.hs.x* abs(_axis.x) + _v.hs.y * abs(_axis.y) + _v.hs.z * abs(_axis.z);\
    if(min>r || max<-r) return 0;\
}



int deway::triBoxTest(Triangle _t, Voxel _v)
{
    //shift the box to origin
    Triangle t = _t;
    for(int i = 0; i < 3; i++)
        t.p[i] = t.p[i] - _v.c;
    
    kep::Vector3 e[3];
    e[0] = t.p[1] - t.p[0];
    e[1] = t.p[2] - t.p[1];
    e[2] = t.p[0] - t.p[2];
    
    t.n = kep::cross(e[0], e[1]);
    t.n.normalize();
    
    kep::Vector3 axis[13];
    axis[0] = kep::Vector3(1.0f, 0.0f, 0.0f); 
    axis[1] = kep::Vector3(0.0f, 1.0f, 0.0f);
    axis[2] = kep::Vector3(0.0f, 0.0f, 1.0f);
    
    axis[3] = t.n;
    
    axis[4] = kep::cross(axis[0], e[0]);
    axis[5] = kep::cross(axis[0], e[1]);
    axis[6] = kep::cross(axis[0], e[2]);
    
    axis[7] = kep::cross(axis[1], e[0]);
    axis[8] = kep::cross(axis[1], e[1]);
    axis[9] = kep::cross(axis[1], e[2]);
    
    axis[10] = kep::cross(axis[2], e[0]);
    axis[11] = kep::cross(axis[2], e[1]);
    axis[12] = kep::cross(axis[2], e[2]);
    
    for(int i = 0; i<13; i++)
    {
        AXISTEST(axis[i]);
    }

    

    return 1;
}

