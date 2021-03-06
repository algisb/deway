#ifndef SPAN_DEWAY_H_
#define SPAN_DEWAY_H_
#include "typedefs.h"

namespace deway 
{
    class Voxel;
    class Span //A coloumn of voxels in Y direction
    {
    public:
        uint m_size;
        Voxel ** m_voxels;
        Span * nghbr[8];
        
        Span();
        ~Span();
        void gen(uint _size);
    };
};
#endif //SPAN_DEWAY_H_
