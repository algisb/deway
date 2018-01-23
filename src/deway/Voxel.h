#ifndef VOXEL_DEWAY_H_
#define VOXEL_DEWAY_H_

namespace deway 
{
    class AABB;
    class Voxel
    {
    public:
        AABB * aabb;
        bool overlaps; // flaged if overlaps anything
        bool traversable;
        bool blacklisted; //
        
//         union
//         {
//             Voxel * nghbr[26];
//         };
        Voxel();
        //Voxel(AABB _aabb = AABB());
        ~Voxel();
        void genAABB(AABB _aabb);
        
    };
};

#endif //VOXEL_DEWAY_H_
