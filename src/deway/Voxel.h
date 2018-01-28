#ifndef VOXEL_DEWAY_H_
#define VOXEL_DEWAY_H_

namespace deway 
{
    class AABB;
    class Voxel
    {
    public:
        AABB * aabb;
        bool overlaps; // flaged if overlaps any mesh, does mean its traversible
        bool traversable; //traversible
        bool blacklisted; //cannot be traversible
        
        bool edge;
        
        Voxel * nghbr[8];//neighbouring voxels
        
        float dist; //distance to the cosest edge voxel
        
        Voxel();
        //Voxel(AABB _aabb = AABB());
        ~Voxel();
        void genAABB(AABB _aabb);
        
    };
};

#endif //VOXEL_DEWAY_H_
