/******************************************************************************/
// FILE NAME: TPopulation.h
// VERSION: 1.0
// FUNCTION: Population view
// REMARKS:
//----------------------------------------------------------------------------*/
// Copyright © 2002 */
/*----------------------------------------------------------------------------*/
// AUTHOR/S:  Dylan W. Schwilk                                                              
//----------------------------------------------------------------------------*/
// CREATION DATE: 020830
//---------+--------+---------------------------------------------------------*/
// VERSION |  DATE  | CHANGES
//---------+--------+---------------------------------------------------------*/
//  2.0.0   021109
//  2.1.0   030416
/******************************************************************************/
/*!
 * \file
 * Header file for TPopulation class.
 */
 
#if !defined(_TPOPULATION_H)
#define _TPOPULATION_H

#include <vector>

#include "dws/dws_stlstats.h"
#include "TEnvironment.h"



//////////////////////////////////////////////////////////////////////
// Class: TPopulation
//////////////////////////////////////////////////////////////////////
/*! Provides a view of a single species/population
 *
 * This class is used for statistics and summaries, the simulation works on
 * a community of individuals, but sometimes we need to view the community as a 
 * set of populations.
 */
class TPopulation
{
public:
    TPopulation();
    virtual ~TPopulation();
    
    void AddLocation(unsigned p);
    unsigned N() const;
    DWS::SimpleDistributionT Distribution() const;
    DWS::SimpleDistributionT NicheDistribution(const TEnvironment& env) const;

private:
    std::vector<double> _locations; // location of each member of the population
                                // stored as floating point numbers.
};


#endif
