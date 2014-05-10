///////////////////////////////////////////////////////////////////////////
// FILE NAME: TChooseWeighted.h
// VERSION: $Revision: 1.1 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: Class for random choice among weighted options
// Copyright © 2002, 2003, 2004     
///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TChooseWeighted.h $
// Revision 1.1  2005/02/10 00:36:15  schwilkd
// .
//
// Revision 1.1  2005/02/10 00:18:48  schwilkd
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
////////////////////////////////////////////////////////////////////////////****************************



/******************************************************************************/
/*!
 * \file
 * Header file for TChooseWeighted class
 */
#ifndef _chooseWeighted_h
    #define _chooseWeighted_h
    
    #include <vector>
     
    namespace DWS
    {
        //////////////////////////////////////////////////////////////////////
        // class TChooseWeighted
        //////////////////////////////////////////////////////////////////////
        /*!
         * This class provides a general method of choosing probabilistically 
         * from among a weighted set of choices.
         */
        class TChooseWeighted
        {
            public:
            TChooseWeighted(); //!< Constructor
            TChooseWeighted(unsigned reserve); //!< Constructor to use when you know how many choices
            virtual ~TChooseWeighted();
    
            void Reset(unsigned reserve);
            void Reset();
            void AddChoice(int id, double weight); //!< Add a new choice and weight.
            int Choose() const; //!< Get the result
    
            private:
            typedef std::pair< int, double> weightT ;
            typedef std::vector< weightT > wVectorT;
            double cumWeight;
            wVectorT weights;
        }; // class TChooseWeighted
    
    }; // namespace DWS
    
#endif // _chooseWeighted_h
