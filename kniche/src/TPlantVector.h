// TPLantVector.h


#ifndef _TPLANTVECTOR_H
#define _TPLANTVECTOR_H

#include <vector>
#include "TPlant.h"


typedef std::vector<TPlant> TPlantVector;

std::ostream& operator<<(std::ostream& os, const TPlantVector& v);
std::istream& operator>>(std::istream& is, TPlantVector& v);

#endif
