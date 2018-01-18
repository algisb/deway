#include "Triangle.h"
#include "AABB.h"
using namespace deway;

Triangle::Triangle(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _p2, kep::Vector3 _n)
{
    p[0] = _p0;
    p[1] = _p1;
    p[2] = _p2;
    n = _n;
    aabb = NULL;
}

Triangle::~Triangle()
{
    delete aabb;
}

void Triangle::genAABB()
{
    //Triangle bounding box
    float d3 = 1.0f/3.0f;
    kep::Vector3 c = (p[0] + p[1] + p[2]) * d3;
    float max[3]={0.0f};
    
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j<3; j++)
        {
            float data = std::abs(p[i].data[j] - c.data[j]);
            if(data > max[j])
                max[j] = data;
        }
    }
    aabb = new AABB(c, kep::Vector3(max[0], max[1], max[2]));
    //aabb->c.dump();
    //aabb->hs.dump();
}

