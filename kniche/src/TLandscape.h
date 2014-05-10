///////////////////////////////////////////////////////////////////////////
// FILE NAME: TLandscape.h
// VERSION: $Revision: 1.4 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: 
// Copyright © 2002, 2003, 2004
// Landscape array for gradniche simulation
///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TLandscape.h $
// Revision 1.4  2005/02/22 20:51:36  schwilkd
// Added Repopulate() method to re-assign species to locations using the existing
// mc.
//
// Revision 1.3  2005/02/16 00:32:01  schwilkd
// Added MakeEnvMonotonic method.
//
// Revision 1.2  2005/02/11 19:01:06  schwilkd
// Cleaned comments.
//
// Revision 1.1  2005/02/10 00:36:15  schwilkd
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
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
//  OLD REVISION HISTORY:
//  2.0.0   021109
//  2.1.0   030416
//  2.3.0   030415      Added population statistics -- distribution stats, 
//                      overlap stats.
//  3.0.0   030421      Speeded up recruitment by eliminating ability
//                      of simulation to use alternative parent selection and
//                      competition algorithms.
//  4.0.0   030531      Eliminated dependency on boost library, split TEnvironment
//                      into a second class, TEnvGenerator, which produces the
//                      environmental function.  TEnvironment is now a component
//                      of TLandscape and is a simple array.  TLandscapes can now
//                      be read to and from files.  Serialization is through
//                      operator>> and <<.  The Format() function now outputs to
//                      a stream for easy human-readable/spreadsheet pastable
//                      representation.
//  4.4.0   040701      Removed fitness threshold -- required changing code in
//                      TRun.cpp, TRun.h and, main.cpp.  Wasn;t being used so I 
//                      tried to speed up the code.
/******************************************************************************/

/*!
 * \file
 * Header file for TLandscape class.
 */
 
#if !defined(_landscape_h)
#define _landscape_h
        
#include <vector>  
#include <map>                                                 
    

#include "TEnvironment.h"
#include "TChooseWeighted.h"
#include "TPopulation.h"
#include "TSpeciesPool.h"  

                        
struct TLStats 
{
    unsigned species;
    unsigned empty_space;
}; // TLStats
    
struct TLongLStats
{
    unsigned species;
    unsigned emptySpace;
    double meanDistSD;
    double meanNicheSD;
    double meanRealizedSimilarity;
    double meanDistDistance;
    double distOverlap;

    TLongLStats()
        {
        // Zero all members
            species = emptySpace = 0;
            meanDistSD=meanNicheSD=0.0;
            distOverlap=meanDistDistance=meanRealizedSimilarity=0.0;
        }
};
 

/*! @brief The clanages the 1-dimensional landscape filled with plants,
 * takes care of dispersing, competition and recruitment.  The main
 * functions for use by clients of this class are the Populate and
 * RunFor methods.
 */  
class TLandscape
{
public:
    //TLandscape(){};  //  no default ctor
    TLandscape(unsigned size, /*double fitThresh,*/ double imRate);
    virtual ~TLandscape();
               
    TLPos Size() const;

    /*! Create the metacommunity and fill the landscape.
     */
    void Populate(const std::vector<TPlant>& pv);

    /*! Refill the landscape choosing randomly from the current
     *  metacommunity.
     */
    void Repopulate();

    /*! Fill the environment array using the supplied generator
     * object.
     */
    void GenerateEnvironment(TEnvGenerator& gen);

    /*! Function: MakeEnvMonotonic()
     *
     * Re-order environment so it is a monotonic rising gradient.
     */
    void MakeEnvMonotonic();

    /*! Function: RunFor()
     *
     * Populate() and GenerateEnvironment() must be called before
     * RunFor().
     */
    bool RunFor(int time);
    
    int PlantAt(TLPos pos) const;  // should really be SpeciesAt()
    // const TPlant& GetPlantBySpecies(unsigned id) const;
   
    unsigned GetPopulations(std::map<unsigned, TPopulation> & pops) const;        
    TLStats GetShortStats() const;  // returns a stats struct, 
    TLongLStats GetLongStats() const;
 //       double NormalPos(TLPos pos) const;



    // io
    void Format(std::ostream& os) const;
        
    friend std::ostream& operator<<( std::ostream& os, const TLandscape& L);
    friend std::istream& operator>>(std::istream& is, TLandscape& L);
    
protected:      
    // Private methods
    void Recruit(TLPos pos);
    void MakeSeeds(TLPos pos, DWS::TChooseWeighted& weights );
    //  double CheckFitnessAt(TPlant& plant, TLPos pos) const;  

    // landscape statistics
    double DistributionOverlap(unsigned binSize) const;
    double LevinsOverlap(unsigned binSize) const;
 
    // Landscape data
    double _imRate;     // immigration rate
    TEnvironment _env;  // array of environments
    std::vector<int> _larray;   // array of plant ids on landscape     
    TSpeciesPool _metacommunity; // array of metacommunity plants
        
    // implementation data not for serialization
    std::vector<int> _seedbank;  // reuseable seedbank array (plant ids)
    DWS::TChooseWeighted _chooseWeighted; // for use by the recruitment algorithm
};
    

std::ostream& operator<<(std::ostream& os, const std::vector<int> v);
std::istream& operator>>(std::istream& is, std::vector<int> v);
#endif // _landscape_h
