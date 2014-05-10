/**************************************************************************/
// FILE NAME:dws_getopt.cpp
// VERSION: $Revision: 1.2 $
// FUNCTION: minimal port of unix/linux gnu getopt.h to platform-independent
//           code. based on Hans Dietrich's (hdietrich2@hotmail.com) 
//           version for win32 / MFC.  Released into public domain.
//------------------------------------------------------------------------*/
// Copyright © 2002 */
/*------------------------------------------------------------------------*/
// AUTHOR/S: Hans Dietrich, Dylan Schwilk (pricklysoft.net) */
//------------------------------------------------------------------------*/
// CREATION DATE: 2002
//--------+----------+----------------------------------------------------*/
// REVISION LOG:
// $Log: dws_getopt.cpp $
// Revision 1.2  2005/01/26 17:23:39  schwilkd
// eliminated MFC dependencies.  I use it with mingw.
//
// Revision 1.1  2005/01/26 17:22:30  schwilkd
// Initial revision
//
//
//*************************************************************************/
//
// Original comment header:
// XGetopt.cpp  Version 1.1
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
// Modified by:
//          Dylan W. Schwilk (protea@schwilk.org)
//          on 18/09/02 09:43
//          Now uses standard headers rather than ms <stdafx>
//          
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "dws_getopt.h"

#include <cstring>  // D. Schwilk
#include <cstdio>  // D. Schwilk

namespace DWS {

char	*optarg;		// global argument pointer
int		optind = 0; 	// global argv index

int getopt(int argc, char *argv[], char *optstring)
{
	static char *next = NULL;
	if (optind == 0)
		next = NULL;

	optarg = NULL;

	if (next == NULL || *next == '\0')
	{
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
		{
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		if (std::strcmp(argv[optind], "--") == 0)
		{
			optind++;
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		next = argv[optind]+1;
		optind++;
	}

	char c = *next++;
	char *cp = std::strchr(optstring, c);  // added std:: - D. Schwilk

	if (cp == NULL || c == ':')
		return '?';

	cp++;
	if (*cp == ':')
	{
		if (*next != '\0')
		{
			optarg = next;
			next = NULL;
		}
		else if (optind < argc)
		{
			optarg = argv[optind];
			optind++;
		}
		else
		{
			return '?';
		}
	}

	return c;
}

}; // namespace DWS;

