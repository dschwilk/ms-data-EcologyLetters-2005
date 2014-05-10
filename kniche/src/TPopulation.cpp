/******************************************************************************/
// FILE: < TPopulation.cpp >
/******************************************************************************/

/*!
 * \file
 * Implementation file for TPopulation class.
 */

#include "TPopulation.h"


//////////////////////////////////////////////////////////////////////
// Class: TPopulation: implementation
//////////////////////////////////////////////////////////////////////

TPopulation::TPopulation(){}
TPopulation::~TPopulation(){}

void TPopulation::AddLocation(unsigned p){
    _locations.push_back(p);
}

unsigned TPopulation::N() const 
{
    return _locations.size();
}


DWS::SimpleDistributionT TPopulation::Distribution() const
{
    return DWS::simple_distribution(this->_locations.begin(),this->_locations.end());
}

DWS::SimpleDistributionT TPopulation::NicheDistribution(const TEnvironment& env) const
{    
    std::vector<double> nL;

    for(std::vector< double >::const_iterator i = _locations.begin(); i!=_locations.end(); i++)
    {
        nL.push_back( env.EnvironmentAt( TLPos(*i) ) );
    }
    return DWS::simple_distribution(nL.begin(),nL.end());  
}

