///////////////////////////////////////////////////////////////////////////
// FILE NAME:TEnvGenerator.h
// VERSION: $Revision: 1.7 $
// FUNCTION: Provides landscape environmental values.
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS:
// Copyright © 2002 */
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or
// modify it undef the GNU Library General Public License
// as published by the Free Software Foundation; either version 2at
// your option) any later version.  This library is distributed in the
// hope that it will be useful, but WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU Library General Public License for more
// details.  You should have received a copy of the GNU Library
// General Public License along with this library; if not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
///////////////////////////////////////////////////////////////////////////


#if !defined(_TEnvGenerator_h)
#define _TEnvGenerator_h

#include <vector>
#include "TOption.h"
#include "nichedefs.h"

/*!
 * @brief The base class for the environmental generator function.
 *
 * This class provides a generator environmental mapping function which
 * describes an environmental variable in terms of geographic position on the 
 * landscape.  All derived classes must provide the Generate(TLPos) 
 * function, which returns a value of the environmental variable for the 
 * position on the landscape.
 */
class TEnvGenerator : public TOption
{
public:
    TEnvGenerator();
    TEnvGenerator(std::string id);
    virtual ~TEnvGenerator();
    
    /*! Environment map.
     *
     * Overridable mapping function. Default behavior is monotonic gradient.
     */
    virtual double GenerateAt(TLPos pos) const;

    /*! @brief Return a specific generator according to a description string.
     *
     * Returns a specialized generator object whose class and
     * parameters are defined in a description string.  The default is
     * a TLinearGenerator.
     */
    static TEnvGenerator* GetEnvGenerator(std::string s, int lsize);
}; 


// class: TLinearGenerator
// -------------------------
/*!
 * @brief Maps a linear gradient.
 *
 * This class maps an environmental gradient onto the landscape.  The mapping 
 * function contains only one parameter: the slope of the gradient.  The gradient 
 * always ruins through zero.
 */
class TLinearGenerator : public TEnvGenerator
{
public:
    TLinearGenerator(double scale, int lsize, double intercept = 0);
    virtual ~TLinearGenerator();
    virtual double GenerateAt(TLPos pos) const;
private:
    double _scale;
    double _lsize;
    double _intercept;
};


// class: TFractalGenerator
// -------------------------
/*!
 * @brief Maps a fractal gradient.
 *
 * This class maps an variable onto the landscape.  The mapping
 * function contains three parameters: H; the scaling factor, and the
 * size of the landscape.  The method creates a fractal surface with
 * fourier transform and the fractal dimension of the resulting
 * landscape is D.  D = 2 - H.  The result is truly fractal (and
 * brownian) for 0 <= H <= 1 (1 <= D <= 2).  The result uses 1/2 of
 * the series produced, therefore the environment does not wrap around
 * as it would if the entire sequence was used.
 */
class TFractalGenerator : public TEnvGenerator
{
public:
    TFractalGenerator(double H, double scale, unsigned lsize);
    virtual ~TFractalGenerator();
    virtual double GenerateAt(TLPos pos) const;
private:
    std::vector<double> _array;
};


// class: TConFractalGenerator
// -------------------------
/*!
 * @brief Maps a fractal gradient with a uniform distribution of states.
 *
 * This class differs from TFractalGenerator in that the fractal
 * landscpe is reduced to rank-values which are then filled from a
 * linear environment --- in other words, the resulting landscape will
 * have a uniform distribution of states, but will have a patchiness
 * created by H.
 */
class TConFractalGenerator : public TEnvGenerator
{
public:
    TConFractalGenerator(unsigned lsize, double H, double scale);
    virtual ~TConFractalGenerator();
    virtual double GenerateAt(TLPos pos) const;
private:
    std::vector<double> _array;
};



// class: TGaussianGenerator
// -------------------- -------------------------
/*!
 * @brief Maps a Gaussian gradient.
 *
 *
 */
class TGaussianGenerator : public TEnvGenerator
{
public:
    TGaussianGenerator(int lsize, double sd, double scale);
    virtual ~TGaussianGenerator();
    virtual double GenerateAt(TLPos pos) const;
private:
    double _lsize;
    double _sd;
    double _scale;
};

#endif //  _TEnvGenerator_h
