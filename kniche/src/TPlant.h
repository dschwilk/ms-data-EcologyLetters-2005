///////////////////////////////////////////////////////////////////////////
// FILE NAME: TPlant.h
// VERSION: $Revision: 1.1 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: 
// Copyright © 2002, 2003, 2004
//  Implementation of TPlant class (organism for gradniche simulation )
///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TPlant.h $
// Revision 1.1  2005/02/10 00:36:15  schwilkd
// .
//
// Revision 1.1  2005/02/10 00:25:51  schwilkd
// Initial revision
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
 * Header file for TTrait and TPLant classes.
 */
 
#if !defined(_plant_h)
    #define _plant_h

    #include "dws/dws_random.h"
    
    #include <iostream>
    #include <vector>
    
/*!
 * @brief A continuous phenotype trait or character.
 *
 * This class defines a single organismal trait which describes a phenotype with
 * a gaussian fitness response to an environmental variable. a TTrait consiists of three 
 * parameters: an optimum (mode), a standard deviation and a height (the height
 * is a weighting that allows traits to contribute unequally to fitness.
 */  
class TTrait
{
public:
    TTrait();
    TTrait(double opti, double sd, double height);
    
    double Test(double val) const;
   
    // friend functionss
    friend std::ostream& operator<<(std::ostream& os, const TTrait& g);
    friend std::istream& operator>>(std::istream& is, TTrait& t); 
//private:
    // three values describe a weighted normal prob distribution
    double  _optimum;
    double _sd;
    double _height;
};
    
  
/*!
 * \brief The class which manages a single organism in the simulation.
 *
 * A TPlant is a single organism, the basic unit of selection in this model.  A 
 * plant is made up of a vector of TTraits.  The first trait trait, obtained by 
 * GetTrait(0) is always dispersal (which is treated differently than all other 
 * traits because dispersal does not depend upon the environment, merely location.
 * 
 * Each plant has an identifier, Species(), which returns the id of the species to
 * which the plant belongs.
 */    
    class TPlant
    {
        public:
        TPlant(); //!< Produces empty cell (dead plant).
        TPlant(int species, int nTraits);
        TPlant(const TPlant& copy);
        virtual ~TPlant();
    
//        void AddTrait(TTrait t) {_traitV.push_back(t);}//!< Add to trait vector 
        void SetTrait(int n, TTrait t){_traitV[n]=t;} //!< Change trait in trait vector 
        TTrait GetTrait(int n) const {return _traitV[n];} //!< Return trait at position \a n
        int Species() const {return _species;} //!< Returns plant's integer ID
        void SetSpecies(int set) {_species=set;} //!< Sets plant's integer ID
        void Kill(){_species=-1;} //!< Make the plant dead
        bool IsDead() const {return _species==-1;} //!< Test if plant is dead
    
//        void MutateCopy(TPlant& child, double mRate) const;

        /*!
         * Tests the fitness of trait \a trait against an environmental value of
         * \a val.
         */
        double CheckFitness(int trait, double val) const;
	double CheckFitness(double val) const;  // with default trait
        /*!
         * Test probability kernel for seed probablity distribution at \a distance.
         * Returns a floating point number which is the relative weighting of seeds from a given 
         * parent at a particular location.  Is equivalent to CheckFitness(0, distance).
         * Distance should be in landscape units, not normalized.
          */
        double CheckSeeds(double distance) const;
    
      //  bool operator==(const TPlant& other) const;
        TPlant& operator=(const TPlant& other);
        
        /*! Stream output
         * Writes plant data to stream in order: species id, number of traits, 
         * every trait in vector
         */
        friend std::ostream& operator<<(std::ostream& os, const TPlant& g);
         /*! Stream input
         * Reads plant in from stream.
         */
        friend std::istream& operator>>(std::istream& is, TPlant& g);
      
        private:
        
        static double GetMutation(double in, double mRate); 

protected:
    int _species;
    int _nTraits;
    
    TTrait* _traitV;                /*< trait vector. First trait is always dispersal. Environment fitness traits
                                        then number 1-N.  THis may slow things down, so I am going back to named
                                        named traits until this flexibility is needed. */
}; // class TPlant
    
#endif // _plant_h
