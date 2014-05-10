///////////////////////////////////////////////////////////////////////////
// FILE: TOption.h
// CLASS: < TOption >
// VERSION: $Revision: 1.2 $
// AUTHOR/S:  Dylan W. Schwilk 
// FUNCTION: A mixin class for Option functions.
// REMARKS: Copyright © 2002
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
///////////////////////////////////////////////////////////////////////////


/*!
 * \file
 * Header file for TOption class.
 */
#if !defined(_TOption_h)
#define _TOption_h

#include <string>
#include <iostream>


// class: TOption
// --------------
/*! @brief A mixin class for Option functions.
*
* Classes that inherit from TOption are
* meant to be function objects for use in programs.  The stream methods allow
* Easy text output.
* 
* @author Dylan W. Schwilk
* 
*/
class TOption 
{
public:
    TOption(const std::string n);
    virtual ~TOption();
    std::string Name();
    void SetName(std::string n){_name = n;}
    
    friend std::ostream& operator<<(std::ostream& os, const TOption& opt);
//    friend std::istream& operator>>(std::istream& is, Toption& opt; 
protected:
    std::string _name;
}; // class TOption
#endif
