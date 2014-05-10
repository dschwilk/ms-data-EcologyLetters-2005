/**************************************************************************/
// FILE NAME: dws_fractal.h
// VERSION: $Revision: 1.2 $
// REMARKS: The dws_fractal libary provides functions for fractal
//          synthesis.
//------------------------------------------------------------------------*/
// Copyright © 2002 */
/*------------------------------------------------------------------------*/
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) */
//------------------------------------------------------------------------*/
// CREATION DATE: 021112
//--------+-----+--------------------------------------+------------------*/
// REVISION LOG
// $Log: dws_fractal.h $
// Revision 1.2  2005/01/26 18:01:33  schwilkd
// Edited comments.
//
//
// 0.0.1    021112      Created
// 1.0.0    030418      Added doxygen comments, changed fxn names to reflect
//                      coding standards.
//*************************************************************************/

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
///////////////////////////////////////////////////////////////////////////

/*!
 * \file
 * The dws_fractal libary provides functions for fractal synthesis.
 */ 
 
#if !defined(DWS_FRACTAL_H)
#define DWS_FRACTAl_H

#include "dws_random.h"
#include "dws_fourier.h"


namespace DWS {

/*! 
 * \defgroup dws_fractal DWS Fractal Synthesis Libary
 * @{
 */


/*! @brief Fractal Synthesis using FFT 1 Dimension
 * 
 * This function produces a 1-dimensional fractal ussing fast fourier
 * transform.  The output is stored in the container pointed to by \a
 * X. \a N indicates the size of the output array and the \a H
 * parameter controls the fractal dimension \a D of the resulting
 * series: \a D = 2-H
 */
template<class OutIterator>
void spectral_synthesis_1D(OutIterator X, unsigned int N, double H)
{
    std::vector< complex_type > V(N);
    double rad, phase, beta;
    
    beta = 2.0*H+1.0;

//    Fill complex array
    for(unsigned i=0;i<N;i++) {
        rad = std::pow(i+1,-beta/2.0)*normal_deviate(0,1) ;
        phase = 2.0*DWS::PI*DWS::random_real(0,1);
        V[i] = std::complex<double>(rad*std::cos(phase), rad*std::sin(phase));
    }
    DWS::fourier_transform(V.begin(),X,N,false); 
}
/*@}*/

}; // namespace DWS

#endif // DWS_FRACTAL_H
