/**************************************************************************/
// FILE NAME: dws_stlstats.h
// VERSION: $Revision 1.1 $
// REMARKS: Statistical functions library.
//------------------------------------------------------------------------
// Copyright © 2003-2005 Dylan W. Schwilk, pricklysoft.net
//------------------------------------------------------------------------
// AUTHOR/S: Dylan Schwilk
//------------------------------------------------------------------------
// CREATION DATE: 18/04/03 11:39
//--------+----------+---------------------------------+------------------
// REVISION LOG:
// $Log: dws_stlstats.h $
// Revision 1.2  2005/02/18 05:01:43  dylan
// Added 'typename' to template average functions.  Not sre why this is
// necessary, buy g++ 3.4 balked.
//
// Revision 1.1  2005/01/26 17:41:16  schwilkd
// Initial revision
//
//
/*************************************************************************/

//////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.  This library
// is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General
// Public License for more details.  You should have received a copy
// of the GNU Library General Public License along with this library;
// if not, write to the Free Software Foundation, 59 Temple Place -
// Suite 330, Boston, MA 02111-1307, USA.
///////////////////////////////////////////////////////////////////////////

/*!
 * \file 
 * This library provides a minimal set of statistical functions for use on 
 * STL containers.  The functions are optimized according to iterator type.
 */
       
        
#ifndef __DWS_STLSTATS_H
#define __DWS_STLSTATS_H

#include <iterator>
#include <cmath>
#include <algorithm>
#include <numeric>

#include "dws_math.h"

namespace DWS{

 /*! \defgroup dws_stlstats DWS STLstats library
  * @{
  */
 
////////////////////////////////////////////////////////////////////////////////
// Implementations
////////////////////////////////////////////////////////////////////////////////

// Specialized for RandomIterators
template < class RandomIterator, class T, class UnaryOperation>
T _average(RandomIterator first, RandomIterator last, T t0, UnaryOperation operation, std::random_access_iterator_tag)
{
		typename std::iterator_traits<RandomIterator>::difference_type n = std::distance(first, last);
        for (; first != last; )
        {
                t0= t0 + operation(*first++);
        }
        return t0/n;
}

template < class RandomIterator, class T>
T _average(RandomIterator first, RandomIterator last, T t0, std::random_access_iterator_tag)
{
        typename std::iterator_traits<RandomIterator>::difference_type n = std::distance(first, last);
        t0= std::accumulate(first, last, t0);
        return t0/n;
}

// Default implementation for InputIterators  -- generic
template < class InputIterator, class T, class UnaryOperation>
T _average(InputIterator first, InputIterator last, T t0, UnaryOperation operation, std::input_iterator_tag)
{
       typename std::iterator_traits< InputIterator >::difference_type n = 0;
       for (; first != last; n++)
                t0= t0 + operation(*first++);
        return t0/n;
}


template < class InputIterator, class T>
T _average(InputIterator first, InputIterator last, T t0, std::input_iterator_tag)
{
       typename std::iterator_traits<InputIterator>::difference_type n = 0;
        for (; first != last; n++)
                t0= t0+ *first++;
        return t0/ n;
}




///////////////////////////////////////////////////////////////////////////////
// Client functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Function : average
// *******
///////////////////////////////////////////////////////////////////////////////
/*!
 *   Calculates the generic average of a sequence; defaults
 *   to arithmetic average. Heavily optimized.
 */
template < class Iterator, class T, class UnaryOperation >
inline T average(Iterator first, Iterator last, T t0, UnaryOperation operation)
{
    return _average(first, last, t0, operation, typename std::iterator_traits< Iterator >::iterator_category() );
}

// default to addition
template < class Iterator, class T>
inline T average(Iterator first, Iterator last, T t0)
{
    return _average(first, last, t0, typename std::iterator_traits<Iterator>::iterator_category());
}



///////////////////////////////////////////////////////////////////////////////
// Function : series
// *******
///////////////////////////////////////////////////////////////////////////////
/*!
 *  fills: 
 *  \code 
        *begin = init; than *<position ith>=
 *       unOp(*<position (i-1)th>)
 *   \endcode
 *      
 *   Similar to std::partial_sum but lets caller specify initial value.
 */
template <class OutputIterator, class T, class UnaryOperation>
void series(OutputIterator begin, OutputIterator end,
        T init, UnaryOperation unOp)
{
        if (begin == end)
                return;
        *begin = init;
        while (++ begin != end)
        {
                init = unOp(init);
                *begin= init;
        }
}


///////////////////////////////////////////////////////////////////////////////
// Function : binomdst
// *******
////////////////////////////////////////////////////////////////////////////
/*! Binomial distribution
 *
 * Evaluates the Poisson distribution P(k) k=[0..nElem] (heavily optimized).
 */
template <class OutputIterator>
void binomdst(OutputIterator begin, OutputIterator end,
        double p, double nElem)
{
        class Binomdst {
                double m_pbar;
                double m_n1; // n+1
                double m_i;
        public:
                Binomdst(double p, int nElem):
                        m_pbar(p/(1-p)), m_n1(nElem+1),
                        m_i(0){}
                double operator()(double dPrev)
                {return dPrev*(--m_n1)*m_pbar/++m_i;}
        } ;

        series(begin, end, pow(1-p, nElem),
                Binomdst(p, nElem));
}



///////////////////////////////////////////////////////////////////////////////
// Class : SD
// *******
///////////////////////////////////////////////////////////////////////////////
/*!
 * function object to calculate the standard
 *       deviation.
 */
template <class T>
class SD {
  T m_ave;
public:
  SD(T ave): m_ave(ave) {}
  T operator()(T x) {return sqr(x-m_ave);}
};


///////////////////////////////////////////////////////////////////////////////
// Struct: AveragesT
// *******
//////////////////////////////////////////////////////////////////////////////
/*!
 * Struct to hold average ( \a ave ) and standard devation ( \a sd ).
 */
struct SimpleDistributionT {
        double mean, sd;
};

///////////////////////////////////////////////////////////////////////////////
// Function: averages
// *******
///////////////////////////////////////////////////////////////////////////////
/*!
 * Fills a structure with average and SD of a sequence.
 */
template <class FwdIterator>
SimpleDistributionT simple_distribution(FwdIterator begin,FwdIterator end)
{
  SimpleDistributionT d;
  d.mean= average(begin, end, double(0));
 // std::cout << "average=" << ave.ave << std::endl;
  d.sd = std::sqrt(average(begin, end, double(0), SD<double>(d.mean)));
 // std::cout << "sd=" << ave.sd << std::endl;
  return d;
}

/*!
 @}
 */ // End group
 

    

}; // Namespace DWS



#endif  // __DWS_STLSTATS_H
