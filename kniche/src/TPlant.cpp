///////////////////////////////////////////////////////////////////////////
// FILE NAME: TPlant.cpp
// VERSION: $Revision: 1.1 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: 
// Copyright © 2002, 2003, 2004
//  Implementation of TPlant class (organism for gradniche simulation )
///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TPlant.cpp $
// Revision 1.1  2005/02/10 00:33:39  schwilkd
// .
//
// Revision 1.1  2005/02/10 00:25:02  schwilkd
// Initial revision
//
//
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or modify it
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

/*!
 * \file
 * Implementation of TTrait and TPLant classes.
 */

#include "TPlant.h"
#include "dws/dws_random.h"
#include "dws/dws_numerical.h"
#include <cassert>

// TTrait
TTrait::TTrait() 
: _optimum(0),_sd(0),_height(0)
{}

TTrait::TTrait(double opti, double sd, double height)
{
    _optimum=opti;
    _sd=sd;
    _height=height;
}

double TTrait::Test(double val) const 
{ 
//    assert(_sd >= 0 && _height >=0);
// is the abs() call below necessary? // no, removed
    return  (_height * DWS::normal_probability(_optimum - val, _sd ));
}

// TTrait i/o 
std::ostream& operator<<(std::ostream& os, const TTrait& t)
{ 
    os  << t._optimum << '\t' << t._sd << '\t' << t._height << '\t';
    return os;
}

std::istream& operator>>(std::istream& is, TTrait& t)
{
    is >> t._optimum >> t._sd >> t._height;
    return is;
}



// TPlant

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TPlant::TPlant()
:_species(0), _nTraits(0),_traitV(NULL)
{}

TPlant::TPlant(const TPlant& copy)
:   _species(copy._species), _nTraits(copy._nTraits)
{
    if(copy._traitV!=NULL) {
        this->_traitV = new TTrait[_nTraits];
        for(int i=0;i<this->_nTraits;++i) this->_traitV[i] = copy._traitV[i];
    } else {
        _traitV=NULL;
    }
}


TPlant::TPlant(int species, int nTraits)
: _species(species), _nTraits(nTraits)
{
    _traitV = new TTrait[nTraits];
}

TPlant::~TPlant()
{
    if(_traitV!=NULL) delete[] _traitV;
}

/// Equality operator not yet provided 
/*bool TPlant::operator==(const TPlant& other) const
{
    if (this==&other) return true;
    return this->_species == other._species;
}
*/

TPlant& TPlant::operator=(const TPlant& copy)
{
    if(this==&copy) return *this;
    _species=copy._species;
    if(this->_nTraits!=copy._nTraits) {
        delete[] this->_traitV;
        this->_traitV = new TTrait[copy._nTraits];
        _nTraits=copy._nTraits;
    }
    for(int i=0;i<this->_nTraits;++i) this->_traitV[i] = copy._traitV[i];
    return *this;
}

/// Method: CheckFitness(val)
/// This method tests a plant's fitness 
double TPlant::CheckFitness( double val) const
{
    return (_traitV[1].Test(val)); // returns probability density function
}

/// Method: CheckFitness(trait, val)
/// This method tests a plant's fitness for particular trait (env
double TPlant::CheckFitness(int trait, double val) const
{
    return (_traitV[trait].Test(val)); // returns probability density function
}

// Method: CheckSeeds
double TPlant::CheckSeeds(double distance) const 
{
 //   std::cout << "CheckSeeds() called. distance: " << distance << " result: " << _traitV[0].Test(distance) << std::endl;
    return (_traitV[0].Test(distance) ); 
}


std::ostream& operator<<(std::ostream& os, const TPlant& g)
{
    os << g._species << '\t' << g._nTraits << '\t';
    for(int i=0;i<g._nTraits;++i) os << g._traitV[i];
    return os;
}

std::istream& operator>>(std::istream& is, TPlant& g)
{
    int nt;
    is >> g._species >> nt;
    if(nt != g._nTraits) {
        delete[] g._traitV;
        g._nTraits=nt;
        g._traitV = new TTrait[g._nTraits];
    }
        
    for(int i=0;i<g._nTraits;i++) is >> g._traitV[i];
    return is;
}

////////////////////////////////////////////////////////////////////////////////
// static functions
double TPlant::GetMutation(double in, double mRate)
{
    return (DWS::random_bit() ? 
             std::min(1.0, in + DWS::exp_deviate(mRate)) : std::max(0.0, in  - DWS::exp_deviate(mRate)) );
}
