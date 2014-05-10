///////////////////////////////////////////////////////////////////////////
// FILE NAME: TEnvironment.cpp
// VERSION: $Revision: 1.5 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: 
// Copyright © 2002, 2003, 2004     
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library Generalre; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published 
// by the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.  This library is distributed in the
// hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU Library General Public License for more details.
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
////////////////////////////////////////////////////////////////////////////


#include "TEnvironment.h"

#include <algorithm>
//constructors

TEnvironment::TEnvironment()
{}  // do nothing ctor


TEnvironment::TEnvironment(int size)
: _vals(size)
{
    std::fill(_vals.begin(),_vals.end(), size);
}

TEnvironment::TEnvironment(int size, TEnvGenerator& gen)
: _vals(size)
{
 for(TLPos i=0;i<size;i++) this->_vals[i] = gen.GenerateAt(i);
}

TEnvironment::TEnvironment(const TEnvironment& cpy)
: _vals(cpy._vals.size())
{
    std::copy(cpy._vals.begin(), cpy._vals.end(), this->_vals.begin());
}

//dtor
TEnvironment::~TEnvironment()
{}


//assignment
TEnvironment& TEnvironment::operator=(const TEnvironment& other)
{
    if(this==&other) return *this;
    
    this->_vals=other._vals;
    return *this;
}


// public functions

double TEnvironment::EnvironmentAt(TLPos po) const
{
   return _vals[po];
}


void TEnvironment::Generate(TEnvGenerator& gen)
{
    for(TLPos i=0;i<_vals.size();i++) _vals[i] = gen.GenerateAt(i);
}


void TEnvironment::MakeMonotonic()
{
    std::sort(_vals.begin(),_vals.end());
}

// friend functions

std::ostream& operator<<( std::ostream& os, const TEnvironment& E)
{
    os << E._vals.size() << '\t';
    for(TLPos i=0;i<E._vals.size();i++) os << E._vals[i] << '\t';
    return os;
}

std::istream& operator>>(std::istream& is, TEnvironment& E)
{
    int v;
    is >> v;
    E._vals.resize(v);
    for(TLPos i=0;i<v;i++) is >> E._vals[i];
    return is;
}
