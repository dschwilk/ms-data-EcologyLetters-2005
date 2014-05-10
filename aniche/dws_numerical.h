/******************************************************************************/
// FILE NAME:dws_numerical.h
// VERSION: 1.1.0
// FUNCTION: Dylan Schwilk's numerical function library
//------------------------------------------------------------------------------
// Copyright © 2003
//------------------------------------------------------------------------------
// AUTHOR/S:  Dylan W. Schwilk 
//------------------------------------------------------------------------------
// CREATION DATE: 19/04/03 23:56
//--------+------+---------------------------------------------+----------------
// VER    | DATE | CHANGES
//--------+------+---------------------------------------------+----------------
//         031103  Added rounding and sig digits functions
/******************************************************************************/


//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////

/*!
 * \file
 * The dws_numerical libary provides a set of non-templated mathematical
 * functions.  Many of these functions are modeled after those in <em> Numerical
 * Recipes in C</em>.
 */ 

#ifndef _DWS_NUMERICAL_H
    #define _DWS_NUMERICAL_H

    #include <limits>
    #include <iterator>
    #include <stdexcept>
    
    namespace DWS
    {
    
    /*! 
     *\defgroup dws_numerical DWS Numerical Library
     * @{
     */
     
        // Exception classes
        
        // Class: convergence_error
        // ------------------------
        /*!
         * Used when iterations fail to converge in numerical functions.
         */
        class convergence_error: public std::runtime_error
        {
          public:
            convergence_error(std::string w) 
              : std::runtime_error(w)
              {}
        };
    
        // type definitions
        typedef double REAL_TYPE;  // can change to float
    
        // constants:
        const REAL_TYPE EPS(std::numeric_limits<REAL_TYPE>::epsilon());
        const REAL_TYPE REAL_MIN(std::numeric_limits<REAL_TYPE>::min());
        const REAL_TYPE PI(3.14159265358979323846);
        const REAL_TYPE E(2.718281828459045235360);
    
        // Library functions
        void SetMAXIT(int it);  //!< Sets maximum iterations for iterative functions.
    
        // Special Mathematical functions
        REAL_TYPE betai(REAL_TYPE a, REAL_TYPE b, REAL_TYPE x) throw(std::invalid_argument);//!< Incomplete Beta function (Numerical recipes p. 227)
        REAL_TYPE gammln(REAL_TYPE x); //!< Gamma fctn (Num. Rec. p. 214)
        REAL_TYPE gammp(REAL_TYPE a, REAL_TYPE x) throw(std::invalid_argument, convergence_error); //!< Incomplete Gamma Function (Num. Rec. p. 218)
        REAL_TYPE erff(REAL_TYPE x); //!< Error function (erf(x)) Num Rec. p 220
        REAL_TYPE factrl(int n); //!< Factorial: returns n! as floating pt num
        REAL_TYPE bico(int n, int k); //!< Binomial coefficient: returns binomial coefficient as floating pt num
        REAL_TYPE factln(int n); //!< Natural log: returns ln(n!)
        
    
        /*! \brief Probability density function evaluated at \a distance.
         *
         * Returns Gaussian probability function according to distance from mode
         * (\a distance: d) and standard deviation (\a sd: s).  Function returns
         * \f[ \frac{e^{\left( -d^2/2s^2\right)}}{\sqrt{2\pi}} \f]
         */
        REAL_TYPE normal_probability(REAL_TYPE distance, REAL_TYPE sd);
        
        /*! \brief Rounds a number to a specified number of digits.
         *
         * doValue is the number you want to round.
         * Num_digits specifies the number of digits to which you want to round number.
         * If num_digits is greater than 0, then number is rounded to the specified number of decimal places.
         * If num_digits is 0, then number is rounded to the nearest integer.
         * Examples
         *		precision(2.15, 1)		equals 2.2
         *  	precision(2.149, 1)		equals 2.1
         *  	precision(-1.475, 2)	equals -1.48
         */
        REAL_TYPE precision(REAL_TYPE doValue, int nPrecision);
        
        /*! \brief Round to significant digits. 
         *
         * Examples:
         * sig_fig(1.23456, 2)		equals 1.2
         * sig_fig(1.23456e-10, 2)	equals 1.2e-10
         * sig_fig(1.23456, 5)		equals 1.2346
         * sig_fig(1.23456e-10, 5)	equals 1.2346e-10
         * sig_fig(0.000123456, 2)	equals 0.00012
         */
        REAL_TYPE sig_fig(REAL_TYPE x, int sigFigs) throw(std::invalid_argument);

        /*! \brief Compare two numbers to sig digits.  
         * 
         * Returns TRUE if x and y are equal to sigFigs significant
         * digits.
         */        
        REAL_TYPE compare_sig(REAL_TYPE x, REAL_TYPE y, int sigFigs)throw(std::invalid_argument);
  
                      
///////////////////////////////////////////////////////////////////////////////
// Classes and structs
///////////////////////////////////////////////////////////////////////////////

        // Struct: QuadraticT
        // ---------------
        /*!
         * class for solving quadratic equations. the struct holds a, b and c 
         * parameters to a quadratic function.
         * The Solve() method fills \a first and \a second with the two 
         * solutions to the quadratic equation.
         */
        struct QuadraticT
        {
            REAL_TYPE _a, _b, _c;
            QuadraticT() {_a = _b = _c = 0;}
            
            /*! 
             * Fills \a first and \a second with two roots of quadratic
             * function
             */
            void Solve(REAL_TYPE& first, REAL_TYPE& second ) const;
            
            /*!
             * Returns derivative of the quadratic function.
             */
            REAL_TYPE DSolve() const;
        };


  /*@}*/
  
    }; /* namespace DWS */
    
    
#endif
// _DWS_MATH_H
