///////////////////////////////////////////////////////////////////////////
// FILE NAME: TChooseWeighted.cpp
// VERSION: $Revision: 1.1 $
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// REMARKS: Class for random choice among weighted options
// Copyright © 2002, 2003, 2004     
///////////////////////////////////////////////////////////////////////////
// REVISION LOG:
// $Log: TChooseWeighted.cpp $
// Revision 1.1  2005/02/10 00:33:39  schwilkd
// .
//
// Revision 1.1  2005/02/10 00:18:04  schwilkd
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
 * Implementation file for TChooseWeighted class
 */

#include "dws/dws_random.h"
#include "TChooseWeighted.h"

#include <cassert>

namespace DWS{
    //////////////////////////////////////////////////////////////////////
    // Construction/Destruction
    //////////////////////////////////////////////////////////////////////

    TChooseWeighted::TChooseWeighted()
        : cumWeight(0)
    {
    }
    TChooseWeighted::~TChooseWeighted()
    {}
        
    TChooseWeighted::TChooseWeighted(unsigned reserve)
        : cumWeight(0)
    {
        weights.reserve(reserve);
    }
    

    void TChooseWeighted::Reset(unsigned reserve)
    {
        cumWeight=0;
        weights.clear();
        weights.reserve(reserve);
    }
 
    void TChooseWeighted::Reset()
    {
        cumWeight=0;
        weights.resize(0);
    }  

    // AddChoice
    ///////////////////////////////////////////////////////////////////////
    void TChooseWeighted::AddChoice(int id, double weight)
    {
        cumWeight+=weight;
        weights.push_back(weightT(id,cumWeight));
    }

    // Choose
    int TChooseWeighted::Choose() const
    {
        using namespace std;
        double ran;

        ran=DWS::random_real(0,cumWeight);
        for(wVectorT::const_iterator i=weights.begin();;++i)
        {
          if (ran <= i->second) return i->first;
        }
        assert(false); // shouldn't get here!
        return -1;
    }

}; // namespace DWS
