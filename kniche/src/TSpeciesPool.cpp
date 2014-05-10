///////////////////////////////////////////////////////////////////////////
// FILE NAME:TSpeciesPool.cpp
// VERSION: $Revision: 1.2 $
// FUNCTION: Implementation of TEnvGenerator Class
// AUTHOR/S: Dylan Schwilk (pricklysoft.net)
// REMARKS: Class for handling species pool and species characteristics.
//          This allows for optimizations sch as lookup tables.
// Copyright © 2004 - 2005
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or modify it
// under the termLibrary General Public License as published 
// by the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.  This library is distributed in the
// hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU Library General Public License for more details.
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
/////////////////////////////////////////////////////////////////////////// 


#include "TSpeciesPool.h"
#include "dws/dws_random.h"
#include "dws/dws_math.h"

TSpeciesPool::TSpeciesPool()
{}

TSpeciesPool::TSpeciesPool(const std::vector<TPlant>& pv)
    : _pool(pv)
{
    int abund;
    _abundances.reserve(this->Size()*10); //starting point
    // fill abundance distribution according to logseries
    for(int i=0;i<this->Size();i++)
    {
	abund = this->Size() / ( DWS::random_integer(1 , this->Size()) );  // how many to insert of this species
	for(int j=0;j<abund;j++)
	{
	    _abundances.push_back(_pool[i].Species());
	}
    }
}

TSpeciesPool::TSpeciesPool(const TSpeciesPool& copy)
: _pool(copy._pool),
 _fitTable(copy._fitTable),
 _dispTable(copy._dispTable),
 _abundances(copy._abundances)
 {}

TSpeciesPool::~TSpeciesPool()
{}

int TSpeciesPool::RandomPlant()
{
    // need to deal with abundances
    return _abundances[DWS::random_integer(0,this->_abundances.size()-1)];
}


void TSpeciesPool::CreateLookupTables(TEnvironment& env)
{
    int lsize = env.Size();
    _fitTable.resize(this->Size());
    _dispTable.resize(this->Size());
    
    // create fit lookup table
    for(int s=0; s<this->Size();s++)
    {
	_fitTable[s].resize(lsize);
	for(int pos=0;pos<lsize;pos++)
	{
	    _fitTable[s][pos] = _pool[s].CheckFitness(env.EnvironmentAt(pos));
	 //   std::cout << "FITNESS OF SPECIES " << s << " at " << pos << " is " <<  _fitTable[s][pos] << std::endl;
	}
    }
    
    
        // create disp lookup table
    for(int s=0; s<this->Size();s++)
    {
	_dispTable[s].resize(lsize);
	for(int pos=0;pos<lsize;pos++)
	{
	    _dispTable[s][pos] = _pool[s].CheckSeeds(pos);
	}
    }
}

double TSpeciesPool::SpeciesFitness(int id, int pos)
{
    return _fitTable[id][pos];    
}

double TSpeciesPool::SpeciesDispersal(int id, int distance)
{
    distance = DWS::abs(distance);
    return _dispTable[id][distance];    
}
    


std::ostream& operator<<(std::ostream& os, const TSpeciesPool& g)
{
    os << g.Size() << '\t';
    for(int i=0;i<g.Size();i++)
    {
	os << g._pool[i] << '\t';
    }
    os << g._abundances.size() << '\t';
    for(int j=0;j<g._abundances.size();j++)
    {
	os << g._abundances[j] << '\t';
    }
 return os;   
}

/*! Stream input
  * Reads plant in from stream.
  */
std::istream& operator>>(std::istream& is, TSpeciesPool& g)
{
    int s;
    is >> s;
    g._pool.resize(s);
    for(int i=0;i<g.Size();i++)
    {
	is >> g._pool[i];
    }
    is >> s;
    g._abundances.resize(s);
    for(int j=0;j<g._abundances.size();j++)
    {
	is >> g._abundances[j];
    }
    return is;
}
