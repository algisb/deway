#ifndef VOXEL_DEWAY_H_
#define VOXEL_DEWAY_H_

namespace deway 
{
    class AABB;
    class Region;
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
        
        //Region generation
        Region * reg;//region voxel belongs to currently
        Region * regLast;//region the voxel belonged to before expansion
        
        Voxel();
        //Voxel(AABB _aabb = AABB());
        ~Voxel();
        void genAABB(AABB _aabb);
        
    };
};

#endif //VOXEL_DEWAY_H_
