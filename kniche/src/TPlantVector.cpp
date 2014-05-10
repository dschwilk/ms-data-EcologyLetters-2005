// TPlantVector.cpp


#include "TPlantVector.h"


std::ostream& operator<<(std::ostream& os, const TPlantVector& v)
{
    os << v.size() << '\t';
    for(unsigned i=0; i< v.size();i++) os << v[i] << '\t';
    return os;
}

std::istream& operator>>(std::istream& is, TPlantVector& v)
{
    unsigned n;
    is >> n;
    v.resize(n);
    for(unsigned i=0;i<n;i++) is >> v[i];
    return is;
}
    
//
