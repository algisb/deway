#include "PolyGen.h"
using namespace deway;
PolyGen::PolyGen(std::vector<Contour*> * _contours)
{
    m_contours_ref = _contours;
}
PolyGen::~PolyGen()
{
}

