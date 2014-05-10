///////////////////////////////////////////////////////////////////////////
// FILE NAME: TLandscape.cpp
// VERSION: $Revision: 1.5 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: 
// Copyright © 2002, 2003, 2004     
///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TLandscape.cpp $
// Revision 1.5  2005/02/22 20:51:44  schwilkd
// Added Repopulate() method to re-assign species to locations using the existing
// mc.
//
// Revision 1.4  2005/02/16 20:37:08  schwilkd
// Fixed delted lines.
//
// Revision 1.3  2005/02/16 00:31:42  schwilkd
// Added MakeEnvMonotonic method.  Eventually the TEnvironment class can
// probably be removed and I can have TLandscape manage the environmental
// array directly.
//
// Revision 1.2  2005/02/11 22:25:59  schwilkd
// Minor formatting.
//
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// GNU
// is free software; you can redistribute it and/or modify it
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
 * Implementation file for TLandscape class.
 */

#include "TLandscape.h"

#include "TChooseWeighted.h"
#include "dws/dws_random.h"
#include "dws/dws_math.h"
#include "dws/dws_stlstats.h"

#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace DWS;

//////////////////////////////////////////////////////////////////////
// Class TLandscape implementation
//////////////////////////////////////////////////////////////////////

// Public methods

// Constructor
// Does not initialize landscape, merely creates and empty landscape
TLandscape::TLandscape(unsigned size,  double imRate)
    :_imRate(imRate),
    _env(size),
    _larray(size),
    _seedbank(size+1),
    _chooseWeighted(size)
{}

TLandscape::~TLandscape()
{}

void TLandscape::Populate(const std::vector<TPlant>& pv)
{
  _metacommunity = TSpeciesPool(pv);
  _metacommunity.CreateLookupTables(this->_env);
  this->Repopulate();
}

void TLandscape::Repopulate()
{
    for (unsigned i=0;i<_larray.size();i++)
        {
            _larray[i] = _metacommunity.RandomPlant();
        }
}

void TLandscape::GenerateEnvironment(TEnvGenerator& gen)
{
    _env.Generate(gen);
}

void TLandscape::MakeEnvMonotonic()
{
    this->_env.MakeMonotonic();
    _metacommunity.CreateLookupTables(this->_env);
}

bool TLandscape::RunFor(int time)
{
  TLPos p;
//  std::cout << "runfor()" <<std::endl;
  for(int t=0;t<time;t++)
  {
    p=DWS::random_integer(0,this->Size()-1);
    Recruit(p);
  }
  return true;
}

TLPos TLandscape::Size() const
{
  return _larray.size();
}

// returns plant id
int TLandscape::PlantAt(TLPos pos) const
{
  return _larray[pos];
}

//const TPlant& TLandscape::GetPlantBySpecies(unsigned id) const
//{
//  return _metacommunity[id];
//}


unsigned TLandscape::GetPopulations(std::map<unsigned, TPopulation> & pops) const
{
  unsigned emptySpace=0;
  pops.clear();

  // Fill a vector of locations for each population, add to map
  for(unsigned p=0;p<this->Size();p++)
  {
    if( _larray[p] >= 0 )
    {  // if plant isn't dead
      pops[this->PlantAt(p)].AddLocation(p);
    }
    else
    {
      ++emptySpace;
    }
  }
  return  emptySpace;
}

TLStats TLandscape::GetShortStats() const
{
  TLStats stats;
  std::set<unsigned> nSpecies;
  stats.empty_space = 0;
  for(TLPos i=0;i<this->Size();i++) 
  {
    // count species
    if(PlantAt(i) >= 0) nSpecies.insert(PlantAt(i));
    else ++stats.empty_space;  // count empty space
  }
  stats.species=nSpecies.size();
  return stats;
}

TLongLStats TLandscape::GetLongStats() const
{
  TLongLStats stats;  //zeroed

  std::map<unsigned, TPopulation> popMap;
  std::vector<double> nicheModes;
  std::vector<double> distModes;
  DWS::SimpleDistributionT A;
  stats.emptySpace = this->GetPopulations(popMap);
  stats.species = popMap.size();

  // now extract pop stats from population map
  for(std::map<unsigned, TPopulation>::iterator i=popMap.begin();i!=popMap.end();i++)
  {
    A = (*i).second.Distribution();
    stats.meanDistSD += A.sd;
    distModes.push_back(A.mean);
    A = (*i).second.NicheDistribution(this->_env);
    stats.meanNicheSD += A.sd;
    nicheModes.push_back(A.mean);
  }
  stats.meanDistSD /= double(stats.species);
  stats.meanNicheSD /= double(stats.species);

  
  // get realized similarity
  for(std::vector<double>::iterator s=nicheModes.begin()+1;s!=nicheModes.end();s++)
  {
      stats.meanRealizedSimilarity+= *s - *(s-1);
  }
  stats.meanRealizedSimilarity /= double(stats.species);
      
  
  // get overlap
  stats.distOverlap = this->DistributionOverlap(10); // bin size=10 for now
  //  stats.distOverlap = this->LevinsOverlap(10);
  
  return stats;
}



////////////////////////////////////////////////////////////////////////////////
// Class TLandscape private methods
////////////////////////////////////////////////////////////////////////////////

// Fill the seedbank in preparation for the selection function
void TLandscape::MakeSeeds(TLPos pos, DWS::TChooseWeighted& weights)
{
  TLPos parentpos;
  double weight;
  
  
  // cycle through potential parents -- use two loops to skip current position
  for(parentpos=0;parentpos<pos;parentpos++)
  {
    _seedbank[parentpos]=this->PlantAt(parentpos); // make seed from parent
    weight = _metacommunity.SpeciesDispersal(this->PlantAt(parentpos),pos-parentpos);
    weight = weight * _metacommunity.SpeciesFitness(_seedbank[parentpos],pos);
    weights.AddChoice(parentpos,weight); // add current weight (pdpr) to weight vector
  }
  
  for(parentpos=pos+1;parentpos<this->_larray.size();parentpos++)
  {
    _seedbank[parentpos]=this->PlantAt(parentpos); // make seed from parent
    weight = _metacommunity.SpeciesDispersal(this->PlantAt(parentpos),pos-parentpos);
    weight = weight * _metacommunity.SpeciesFitness(_seedbank[parentpos],pos);
    weights.AddChoice(parentpos,weight); // add current weight (pdpr) to weight vector
  }
  
  // check for immigration event
  if(DWS::random_chance(this->_imRate))
  {
      _seedbank[this->Size()] = _metacommunity.RandomPlant();
      // don't need to check dispersal,
      weights.AddChoice(this->Size(),
                        this->_metacommunity.SpeciesFitness(_seedbank[parentpos],pos));
  }
}


//  Recruit(pos)
// -------------
/// Procedure to attempt replacement of plant at pos.
void TLandscape::Recruit(TLPos pos)
{
  int winningSeed;

  _chooseWeighted.Reset(this->Size());
  MakeSeeds(pos, _chooseWeighted);
  winningSeed = _seedbank[ _chooseWeighted.Choose()];

  _larray[pos] = winningSeed;
}

double TLandscape::DistributionOverlap(unsigned binSize) const
{
  std::set<unsigned> species;
  double mean=0;
  unsigned n = 0;
  for(; n < this->_larray.size() - binSize; n++)
  {
    species.clear();
    for(unsigned p = n; p < n + binSize; p++)
    {
      if( PlantAt(p) >= 0 ) species.insert( this->PlantAt(p) );
    }
    mean+=species.size();
  }
  return (mean / (double)n);
}

double TLandscape::LevinsOverlap(unsigned binSize) const
{
  //    std::set<unsigned> species;
  std::map<unsigned,double> species;
  double overlap=0;

  //double mean=0;
  unsigned n = 0;
  for(; n < this->_larray.size() - binSize; n++)
  {
    species.clear();
    for(unsigned p = n; p < n + binSize; p++)
    {
      if( PlantAt(p) >= 0 ) species[this->PlantAt(p)] += 1.0/binSize; //increment proportions
    }
    double sum=1.0;
    double sq=1.0;
    //   double div=species.size();
    for(std::map<unsigned,double>::const_iterator i=species.begin();i!=species.end();++i)
    {
      sum *= i->second;
      sq *= (i->second)*(i->second);
    }
    overlap+= sum / (std::sqrt(sq));
  }
  return (overlap / (double)n);
}


////////////////////////////////////////////////////////////////////////////////
// Stream I/O Implementation
////////////////////////////////////////////////////////////////////////////////

// formatted output

void TLandscape::Format(std::ostream& os) const
{
  // os << "LSize\tMComSize\tFitThresh\tImRate\n";
  os << "LSize\tMComSize\tImRate\n";
  os << Size() << '\t' << _metacommunity.Size() << '\t' << _imRate << '\n';
  os << "Pos\tEnv\tPlant\n";
  for(TLPos i=0;i< this->Size();i++)
  {
    os << i << '\t' << this->_env.EnvironmentAt(i) << '\t' << _metacommunity.GetSpecies(_larray[i]) << '\n';
  }
}



// serialization
std::ostream& operator<<(std::ostream& os, const TLandscape& L)
{
  os << L._imRate << '\n';
  os << L._env << '\n' << L._larray << '\n' << L._metacommunity;
  return os;
}

std::istream& operator>>(std::istream& is, TLandscape& L)
{
  is >> L._imRate >> L._env >> L._larray >> L._metacommunity;
  return is;
}



//general vector serialization

std::ostream& operator<<(std::ostream& os, const std::vector<int> v)
{
    os << v.size() << '\t';
    for(unsigned i=0; i< v.size();i++) os << v[i] << '\t';
    return os;
}

std::istream& operator>>(std::istream& is, std::vector<int> v)
{
    unsigned n;
    is >> n;
    v.resize(n);
    for(unsigned i=0;i<n;i++) is >> v[i];
    return is;
}

