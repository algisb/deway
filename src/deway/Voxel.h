#ifndef VOXEL_DEWAY_H_
#define VOXEL_DEWAY_H_

namespace deway 
{
    class Edge;
    class AABB;
    class Region;
    class Voxel
    {
    public:
        AABB * aabb;
        bool overlaps; // flaged if overlaps any mesh, does mean its traversible
        bool traversable; //traversible
        bool blacklisted; //cannot be traversible
        
        bool edge;//does this voxel have a null neighbour
        
        union
        {
            Voxel * nghbr[8];//neighbouring voxels
            struct
            {
                Voxel * up;
                Voxel * up_left;
                Voxel * left;
                Voxel * down_left;
                Voxel * down;
                Voxel * down_right;
                Voxel * right;
                Voxel * up_right;
            };
        };
        
        
        float dist; //distance to the cosest edge voxel
        
        //Region generation
        Region * reg;//region voxel belongs to currently
        int regCounter;
        
        union
        {
            Edge * edg[4];//edges of the top surface span of the voxel
            struct
            {
                Edge * up_e;
                Edge * left_e;
                Edge * down_e;
                Edge * right_e;
            };
        };
        
        
        
        Voxel();
        //Voxel(AABB _aabb = AABB());
        ~Voxel();
        void genAABB(AABB _aabb);
        
    };
};

#endif //VOXEL_DEWAY_H_
