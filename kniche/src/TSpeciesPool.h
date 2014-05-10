/******************************************************************************/
// CLASS: < TSpeciesPool >
/******************************************************************************/
// FILE NAME: TSpeciesPool.h
// VERSION: 0.1
// FUNCTION:
// REMARKS:
//----------------------------------------------------------------------------*/
// Copyright © 2002 */
/*----------------------------------------------------------------------------*/
// AUTHOR/S:  Dylan W. Schwilk */
//----------------------------------------------------------------------------*/
// CREATION DATE: <date>
//----+------+---------------------------------------------+------------------*/
// VER| DATE | CHANGES | AUTHOR */
//----+------+---------------------------------------------+------------------*/
//
/******************************************************************************/

#if !defined(_species_pool_h)
#define _species_pool_h

#include <vector>
#include "TPlantVector.h"
#include "TEnvironment.h"

/*!
 * \brief The class which manages a the simulation species pool (metacommunity).
 *
 * This class is added to improve the speed of the simulation. This adds a wrapper around 
 * TPlant class so now Tlandscape will only access TSpeciesPool rather than TPLants
 * directly.  THis allows the species pool to maintain a cache of fitness and dispersal
 * test values and eliminate extraneous calculations of guassian kernels.
 */

class TSpeciesPool
{
public:
 TSpeciesPool();
  TSpeciesPool(const std::vector<TPlant>& pv);
  //TSpeciesPool(int species, int nTraits);
  TSpeciesPool(const TSpeciesPool& copy);
  virtual ~TSpeciesPool();

  int Size() const {return _pool.size();}
  int RandomPlant();
  const TPlant& GetSpecies(int id) const {return _pool[id];}
  void CreateLookupTables(TEnvironment& env);  //! Fills the lookup tables for fitness and dispersal kernel values
  double SpeciesFitness(int id, int pos);  //! Returns fitness 
  double SpeciesDispersal(int id, int pos); // and dispersal of species (id) at landscape position pos
  
  friend std::ostream& operator<<(std::ostream& os, const TSpeciesPool& g);
  /*! Stream input
  * Reads plant in from stream.
  */
  friend std::istream& operator>>(std::istream& is, TSpeciesPool& g);

protected:
  TPlantVector _pool;   // array of plants in species pool
  std::vector<int> _abundances; // array of plant IDs repeated according to abundance distribution
  std::vector< std::vector<double> > _fitTable; // lookup table for fitness and dispersal values
  std::vector< std::vector<double> > _dispTable;
}
; // class TPlant
#endif // _species_pool_h

