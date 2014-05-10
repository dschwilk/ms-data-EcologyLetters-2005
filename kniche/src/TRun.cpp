///////////////////////////////////////////////////////////////////////////
// FILE NAME: TRun.cpp
// VERSION: $Revision: 1.6 $
// REMARKS: Handles a single run of the niche simulation
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// Copyright © 2002, 2003, 2004     
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TRun.cpp $
// Revision 1.6  2005/02/25 19:23:46  schwilkd
// Fixed run output header (replaced 'cycles' with 'epochs').
//
// Revision 1.5  2005/02/16 20:35:56  schwilkd
// Fixed delted lines.
//
// Revision 1.4  2005/02/16 00:34:26  schwilkd
// Fixed another mysterious disappearing text issue in the file.  Is this
// caused by RCS?
//
// Revision 1.3  2005/02/15 00:25:55  schwilkd
// Removed stability testing, changed i/o format slightly (removed
// _cycles, added _epochs).
//
// Revision 1.2  2005/02/11 23:08:56  schwilkd
// Refactored: moved run initialization code into separate function from
// Run().
//
////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// OLD REVISION HISTORY:
//--------+----------+--------------------------------------+-----------------*/
//  0.2.5   020607      Added running average stability criteria
//  0.3.0   021107      Reads parameters from input stream
//  0.4.0   030114      Added _nSpecies member and changed MakePLantVector
//                      now _nTypes controls number of unique phenotypes and 
//                      _nSpecies controls only species identity.
//  0.5.0   031009      Added new input parameter: TEnvGenerator describing
//                      Metacommunity environment (generator used to assign
//                      metacommunity species niche optima).
//	    040706	Changed -p popout format to be more compact.  Now 
//			population sizes of zero are omitted.
//*****************************************************************************/

//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////

/*!

 * \file
 * Implementation file for TRun class.
 */
 
#include "TRun.h"

#include "TLandscape.h"
#include "TEnvironment.h"
#include "TPlant.h"

#include "dws/dws_math.h"
#include "dws/dws_random.h"
#include "dws/dws_stlstats.h"

#include <stdexcept>
#include <fstream>

// Number of cycles to include in running div average this
// is also the frequency at which to check change.
static const int RUNNING_AVE_SIZE(10); 

TRun::TRun()
    : _pMCEnvGen(NULL),
      _pEnvGen(NULL),
      _pLandscape(NULL),
      _runningAve(RUNNING_AVE_SIZE)
{
      // initialize running average ring buffer
    for(unsigned i=0;i<_runningAve.size();i++) _runningAve[i]=0; 
}

// This constructor is less likely to be used than the defaul since we
// generally read paramters directly from a file.  I might remove it
// to avoid confusion.
TRun::TRun(unsigned size, double imRate, std::string envGen, std::string MCEnvGen,
           unsigned nSpecies, unsigned epochs,  double nb, double db)
:   _size(size),
    _imRate(imRate),
    _epochs(epochs),
    _nicheB(nb),
    _dispB(db),
    _runningAve(RUNNING_AVE_SIZE)
{
    for(unsigned i=0;i<_runningAve.size();i++) _runningAve[i]=0;  // initialize
    // The TRun object owns the pointers below and must handle memory
    _pEnvGen = TEnvGenerator::GetEnvGenerator(envGen, _size);
    _pMCEnvGen = TEnvGenerator::GetEnvGenerator(MCEnvGen, _size);
    _pLandscape = new TLandscape(_size, imRate);
}


TRun::~TRun()
{
    delete _pEnvGen;
    delete _pMCEnvGen;
    delete _pLandscape;
}

void TRun::Initialize()
{
    std::vector<TPlant> V(_size);
    MakePlantVector(V, _pMCEnvGen, _nSpecies, _nicheB,_dispB);
   _pLandscape->GenerateEnvironment(*_pEnvGen);
    _pLandscape->Populate(V);
}

// Method: Run
// -----------
// The meat of the matter
void TRun::Run(int runID, std::ostream* stats,
               std::ostream* landOut, std::ostream* popOut)
{
    unsigned t = 0;
    std::map<unsigned, TPopulation> popMap; // to get popsizes
    std::vector<std::map<unsigned, TPopulation> > populationHistories;

    // Run until stability or MAX_GENERATIONS
    for(t=0;t < this->_epochs;t++)
    {
        if (!_pLandscape->RunFor(_size)) break;  // we break out of loop if the whole shebang goes extinct
        UpdateDiversities(*_pLandscape,t);
       // record population histories if option is selected
       if(popOut!=NULL)
       {
           _pLandscape->GetPopulations(popMap);
	   populationHistories.push_back( popMap);
       }
    }
    
    // The run is over, now do stats
    TLongLStats lstats =   _pLandscape->GetLongStats();
    *stats  << runID << '\t' << *this << '\t' << lstats.species  << '\t'
            <<  DWS::average(_runningAve.begin(),_runningAve.end(), 0.0)
            << '\t' << lstats.meanRealizedSimilarity << '\t'
            << lstats.meanDistSD << '\t' << lstats.meanNicheSD << '\t'
            << lstats.distOverlap << '\t' << t << std::endl;
    
    // Population size histories if we are doing long output
    if(popOut!=NULL) 
    {
        *popOut << "RUN\t" << runID << "\tParameters:\n" << *this << '\n';
        *popOut << "TimeStep\tSpecies:N\n";
	for(int pi=0; pi < populationHistories.size();pi++)
        {
            *popOut << pi << '\t';
            for(std::map<unsigned,
                    TPopulation>::iterator mi=populationHistories[pi].begin();
                mi!=populationHistories[pi].end();++mi)
            {
                *popOut << (mi->first);
		*popOut << ':' << (mi->second.N()) << '\t';
            }
            *popOut << '\n';
        }
    }
    
    //  Landscape output, if requested.
    if(landOut !=NULL) {
        *landOut << "RUN\t" << runID << "\tLandscape:\n";
        _pLandscape->Format(*landOut);
        *landOut << std::endl;
    }
}

/*
bool TRun::IsStable()
{
    _d0 = _d1; // update 'old div,' _d0
    _d1 = DWS::average(_runningAve.begin(),_runningAve.end(), 0.0);   // update 'new div'
    // std::cout << _d0 << '\t' << _d1 << '\t' <<  DWS::abs( _d0 - _d1 ) / _size << std::endl; 
    // check change fraction standardized by RUNNING_AVERAGE_SIZE (to make it a per cycle prob)
    if (DWS::abs( _d0 - _d1 ) / _size < CHANGE_FRAC)
    {
        return true;
    } else {
        return false;
    }
}
*/

void TRun::UpdateDiversities(const TLandscape& L, int time)
{
    TLStats landscapeStats;

    landscapeStats = _pLandscape->GetShortStats();
    // add to array, cycling through    
    _runningAve[time % this->_runningAve.size()] = landscapeStats.species;  
}

///////////////////////////////////////////////////////////////////////////
// TRun input/output
///////////////////////////////////////////////////////////////////////////

// static function: WriteHeader
void TRun::WriteHeader(std::ostream& os)
{
    os << "RunId\tLSize\tImmigrationRate\tEnvironmentType\t"
       << "MetaComEnv\tnSpecies\tEpochs\tnicheB\tdispersalB";
}

std::ostream& operator<<(std::ostream& os, const TRun& r)
{
    //os << r._size << '\t' << r._fitThresh << '\t' << r._imRate << '\t';
    os << r._size << '\t' << r._imRate << '\t';
    os << *r._pEnvGen << '\t' << *r._pMCEnvGen << '\t' << r._nSpecies << '\t' 
       << r._epochs << '\t' << r._nicheB << '\t' << r._dispB;
    return os;
}

// The TRun object read from a stream will own and be responsible for
// the three pointers below
std::istream& operator>>(std::istream& is, TRun& r)
{
    std::string tok;
    if(is.eof())  throw std::runtime_error("Error reading run parameters");
    is >> r._size;
    if(is.eof())  throw std::runtime_error("Error reading run parameters");
    is >> r._imRate;
    if(is.eof()) throw std::runtime_error("Error reading run parameters");
    is >> tok;
    r._pEnvGen = TEnvGenerator::GetEnvGenerator(tok, r._size);
    is >> tok;
    r._pMCEnvGen = TEnvGenerator::GetEnvGenerator(tok, r._size);
    is >> r._nSpecies  >> r._epochs  >> r._nicheB >> r._dispB;
    r._pLandscape = new TLandscape(r._size, r._imRate);
    return is;
}

// MakePlantVector
// ---------------
/*! Creates a vector of \a nSpecies plants with distinct phenotypes.
 * The species id of each plant in the array is the same as its
 * position in the array.  The environmental generator, pGen is used
 * to assign niche modes.
 */
void TRun::MakePlantVector(std::vector<TPlant>& pv, TEnvGenerator* pGen,
                           unsigned nSpecies, double nb,double db)
{
    pv.resize(nSpecies);
    TTrait dispersal(0,db,1);
    for(unsigned i=0;i<nSpecies;i++)
    {
        TPlant p(i,2);  //
        TTrait fitness(pGen->GenerateAt( (i * _size) / nSpecies ),nb,1.0);
        p.SetTrait(1,fitness);
        p.SetTrait(0,dispersal);
        pv[i] = p;
    }
 //   for(int j=0;j<nTypes;j++) std::cout << pv[j] << std::endl;
}
