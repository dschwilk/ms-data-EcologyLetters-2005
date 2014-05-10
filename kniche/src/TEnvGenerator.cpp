///////////////////////////////////////////////////////////////////////////
// FILE NAME:TEnvGenerator.cpp
// VERSION: $Revision: 1.8 $
// FUNCTION: Implementation of TEnvGenerator Class
// AUTHOR/S: Dylan Schwilk (pricklysoft.net)
// REMARKS:
// Copyright ©
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or modify it
// under the termLibrary General Public License as published 
// by the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.  This library is distributed in the
// hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU Library General Public License for more details.
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
/////////////////////////////////////////////////////////////////////////// 

#include "TEnvGenerator.h"
#include "dws/dws_fractal.h"
#include "dws/dws_stlstats.h"

#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>

TEnvGenerator::TEnvGenerator()
:TOption("TEnvGenerator")
{}

TEnvGenerator::TEnvGenerator(std::string id)
:TOption(id)
{}

TEnvGenerator::~TEnvGenerator()
{}

double TEnvGenerator::GenerateAt(TLPos pos) const
{
    assert(false);
     return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Class: TLinearGenerator
/////////////////////////////////////////////////////////////////////////////

TLinearGenerator::TLinearGenerator(double scale, int lsize, double intercept /*=0*/)
:TEnvGenerator("LinearGenerator"),
 _scale(scale),
 _lsize(lsize),
 _intercept(intercept)
{
    char buf[50];
    std::sprintf(buf, "_%g_%g",scale, intercept);
}

// nothing dtor

TLinearGenerator::~TLinearGenerator()
{}

//GenerateAt(pos)
double TLinearGenerator::GenerateAt(TLPos pos) const
{
    //     std::cout << "In Generate()" << std::endl;
    return  pos*this->_scale/_lsize + _intercept;
}


///////////////////////////////////////////////////////////////////////////////
// Class: TFractalGenerator
///////////////////////////////////////////////////////////////////////////////

TFractalGenerator::TFractalGenerator(double H, double scale, unsigned lsize)
    :TEnvGenerator("FractalGenerator"),
     _array(lsize)
{
    // make name
    char buf[50];
    std::sprintf(buf, "_%g",H);
    this->_name+= std::string(buf);
    std::sprintf(buf, "_%g",scale);
    this->_name+= std::string(buf);
    
    double mean=0;
    double sd=0;
    _array.resize(lsize);
    
    // Do spectral synthesis -- current version uses only 1/4 of the
    // series produced to maintain the relationship of variance with
    // distance and avoid wrapping around.
    std::vector<std::complex<double> > tv(lsize*4);
    DWS::spectral_synthesis_1D(tv.begin(),tv.size(),H);

    //    Unpack to _array
    for(unsigned i=0;i<lsize;i++) _array[i] = tv[i].real();
    // transform to z-scores
    mean = DWS::average(_array.begin(),_array.end(),double(0));
    sd = std::sqrt(average(
                       _array.begin(),
                       _array.end(), double( 0), DWS::SD<double>(mean)));
    for(int i=0;i<lsize;i++) _array[i] = ((_array[i] - mean) / sd)*scale;
}

// Function: ~TFractalGenerator()
TFractalGenerator::~TFractalGenerator(){} // nothing dtor

// Function: GenerateAt(pos)
double TFractalGenerator::GenerateAt(TLPos pos) const
{
    assert(pos>=0 & pos<_array.size());
    return  _array[pos];
}

///////////////////////////////////////////////////////////////////////////////
// Class: TConFractalGenerator
///////////////////////////////////////////////////////////////////////////////

TConFractalGenerator::TConFractalGenerator(unsigned lsize, double H, double scale)
    :TEnvGenerator("ConFractalGenerator"),
     _array(lsize)
{
    // make name
    char buf[50];
    std::sprintf(buf, "_%g",H);
    this->_name+= std::string(buf);
    std::sprintf(buf, "_%g",scale);
	this->_name+= std::string(buf);
    
    //_array.resize(lsize);
    std::vector< std::pair< double,int > > sortV(lsize);

    TFractalGenerator Fpart(H,1.0,lsize);
    TLinearGenerator Lpart(scale, lsize, 0.0);
    for(int i=0;i<lsize;i++){
        sortV[i].first = Fpart.GenerateAt(i);
        sortV[i].second = i;
    }
    std::sort(sortV.begin(),sortV.end());
    for(int i=0;i<lsize;i++){
        _array[sortV[i].second] = Lpart.GenerateAt(i);
    }
 }

// Function: ~TFractalGenerator()
TConFractalGenerator::~TConFractalGenerator(){} // nothing dtor

// Function: GenerateAt(pos)
double TConFractalGenerator::GenerateAt(TLPos pos) const
{
    assert(pos>=0 & pos<_array.size());
    return  _array[pos];
}




///////////////////////////////////////////////////////////////////////////////
// Class: TGaussianGenerator
///////////////////////////////////////////////////////////////////////////////

TGaussianGenerator::TGaussianGenerator(int lsize, double sd, double scale)
:TEnvGenerator("GaussianGenerator"),
 _lsize(lsize),
 _sd(sd),
 _scale(scale)

{
    char buf[50];
    std::sprintf(buf, "_%g_%g", sd, scale);
    this->_name+= std::string(buf);
}

TGaussianGenerator::~TGaussianGenerator(){} // nothing dtor

// Function: Generate(pos)
double TGaussianGenerator::GenerateAt(TLPos pos) const
{
    return  DWS::normal_probability(double(pos) / double (_lsize), _sd) * _scale;  
}

///////////////////////////////////////////////////////////////////////////
// Static functions
///////////////////////////////////////////////////////////////////////////

/*! @brief Creates generators from string description.
 *
 * GetEnvGenerator() translator function reads in a string describing
 * a generator and produces a new specialized TEnvGenerator, passing
 * memory responsibility to the caller.
 */
TEnvGenerator* TEnvGenerator::GetEnvGenerator(std::string s, int lsize)
{
    if(s=="TEnvironment") {
        return new TEnvGenerator;
    } else if (s.find("LinearGenerator") == 0) {
        
        int first_ = s.find_first_of('_');
        int second_ = s.find("_", first_+1);
        std::string s_scale = s.substr(first_+1, second_ - first_ - 1);
        std::string s_intercept = s.substr(second_+1, s.size()-second_);

        double scale = std::strtod( s_scale.c_str(), 0);
        double intercept = std::strtod(s_intercept.c_str(),0);
          
        return new TLinearGenerator(scale, lsize, intercept);
        
    } else if (s.find("FractalGenerator") == 0)
    {
        int first_ = s.find_first_of('_');
        int second_ = s.find("_", first_+1);
        std::string H_str = s.substr(first_+1, second_ - first_ - 1);
        std::string scale_str = s.substr(second_+1, s.size()-second_);

        double scale = std::strtod( scale_str.c_str(), 0);
        double H = std::strtod(H_str.c_str(),0);
          
        return new TFractalGenerator(H,scale,lsize);
        
    } else if (s.find("ConFractalGenerator") != s.npos)
    {
        int first_ = s.find_first_of('_');
        int second_ = s.find("_", first_+1);
        std::string H_str = s.substr(first_+1, second_ - first_ - 1);
        std::string scale_str = s.substr(second_+1, s.size()-second_);

        double scale = std::strtod( scale_str.c_str(), 0);
        double H = std::strtod(H_str.c_str(),0);
          
        return new TConFractalGenerator(lsize, H,scale);
    }
    else if (s.find("GaussianGenerator") == 0)
    {
        int first_ = s.find_first_of('_');
        int second_ = s.find("_", first_+1);
        std::string s_sd = s.substr(first_+1, second_ - first_ - 1);
        std::string s_scale = s.substr(second_+1, s.size()-second_);

        double scale = std::strtod( s_scale.c_str(), 0);
        double sd = std::strtod(s_sd.c_str(),0);
          
        return new TGaussianGenerator(lsize, sd, scale);
    } else {
        throw(std::runtime_error("Unknown Environment type: " + s));
    }
}
// TEnvGenerator.cpp
