///////////////////////////////////////////////////////////////////////////
// FILE NAME: TEnvironment.h
// VERSION: $Revision: 1.4 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: 
// Copyright © 2002, 2003, 2004, 2005
// Structure for storing continuous environment values
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or
// modifyis library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.  This library
// is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General
// Public License for more details.  You should have received a copy
// of the GNU Library General Public License along with this library;
// if not, write to the Free Software Foundation, 59 Temple Place -
// Suite 330, Boston, MA 02111-1307, USA.
////////////////////////////////////////////////////////////////////////////

#ifndef _TENVIRONMENT_H
#define _TENVIRONMENT_H

#include "nichedefs.h"
#include "TEnvGenerator.h"

#include <vector>

class TEnvironment
{
public:
    TEnvironment();
    TEnvironment(int size);
    TEnvironment(int size, TEnvGenerator& gen);
    TEnvironment(const TEnvironment& cpy);
    ~TEnvironment();
    
    TEnvironment& operator=(const TEnvironment& other);
    int Size(){return _vals.size();}

    /*! Environment map.
     *
     * Mapping function. Default behavior is monotonic gradient.
     */
    double EnvironmentAt(TLPos pos) const;
    
    /*! Recreate environment
     *
     * Hand a generator function to the environment
     */
    void Generate(TEnvGenerator& gen);

    /*! Function: MakeMonotonic()
     *
     * Re-order environment so it is a monotonic rising gradient.
     */
    void MakeMonotonic();
    
    friend std::ostream& operator<<( std::ostream& os, const TEnvironment& E);
    friend std::istream& operator>>(std::istream& is, TEnvironment& E);


private:

std::vector<double> _vals;

}; 
#endif
