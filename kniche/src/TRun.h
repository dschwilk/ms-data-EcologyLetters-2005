///////////////////////////////////////////////////////////////////////////
// FILE NAME: TRun.h
// VERSION: $Revision: 1.8 $
// REMARKS: Handles a single run of the niche simulation
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// Copyright © 2002, 2003, 2004     
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or modify it
// under theNU
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
 * Header file for TRun class.
 */
 
#if !defined(_TRun_h)
#define _TRun_h

#include <iostream>
#include <string>
#include "TLandscape.h"

                           
/*!
 * @brief Handles a single run of the simulation
 *
 * This class reads in the necessary parameters, creates the landscape
 * and runs the simulation to stability, then outputs the final
 * statistics.  The class is best thought of as a parameter set. A run
 * can be reset, rerun and result in different outputs do to
 * stochasticity, but it represetns an unchangeable set of parameters.
 */
class TRun
{
public:
    TRun();
    TRun(unsigned size, double imRate, std::string envGen, std::string
         MCEnvGen, unsigned nSpecies, unsigned cycles,  double nb, double db);
    ~TRun();

    /*! Initialize the run.  Load landscape and populate
     */
    void Initialize();
    
    /*!
     * Initiate the run.  Run will go until stability and output stats to the
     * streams provided.
     */
    void Run(int runID, std::ostream* os, std::ostream* landOut =NULL,
             std::ostream* popOut=NULL);
 

    // Access to the generators and landscape (all as pointers)
    TEnvGenerator* GetEnvGen(){return _pEnvGen;};
    TEnvGenerator* GetMCEnvGen(){return  _pMCEnvGen;};
    TLandscape* GetLandscape(){return _pLandscape;};

    friend std::istream& operator>>(std::istream& is, TRun& r);
    static void WriteHeader(std::ostream& os);
    friend std::ostream& operator<<(std::ostream& os, const TRun& r);

private:
    //   bool IsStable();
    void UpdateDiversities(const TLandscape& L, int time);
    void MakePlantVector(std::vector<TPlant>& pv, TEnvGenerator* pGen,
                         unsigned nSpecies, double nb,double db);
   // void Makee db);
   // void MakePlantVectorLogseries(std::vector<TPlant>& pv,
                        //TEnvGenerat        //TEnvGenerator* pGen, unsigned nSpecies, double nb,double db);
    
    // params for TLandscape construction
    unsigned _size;
    double _imRate;
    
    // params for landscape initialization
    TLandscape* _pLandscape;
    TEnvGenerator* _pEnvGen;
    TEnvGenerator* _pMCEnvGen;
    unsigned _nSpecies; // number of distinct species in metacommunity
    unsigned _epochs; // num of epochs to run for
    double _nicheB; // niche breadth of every plant
    double _dispB;  // dispersal breadth of every plant
    
        // for keeping track of a run
    std::vector<double> _runningAve;

};

#endif
