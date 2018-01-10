#ifndef PRECISION_H_
#define PRECISION_H_
#include <cmath>
#include <stdint.h>
namespace kep
{
    typedef float real;
#define real_pow powf
#define real_abs fabsf

#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#define real_sqrt sqrtf

};

#endif // PRECISION_H_
