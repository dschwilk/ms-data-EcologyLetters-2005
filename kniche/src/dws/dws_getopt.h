/**************************************************************************/
// FILE NAME:dws_getopt.h
// VERSION: $Revision: 1.2 $
// REMARKS: minimal port of unix/linux gnu getopt.h to platform-independent
//          code.  based on Hans Dietrich's (hdietrich2@hotmail.com)
//          version for win32 / MFC.  Released into public domain.
//------------------------------------------------------------------------*/
// Copyright © 2002 - 2005 */
/*------------------------------------------------------------------------*/
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) */
//------------------------------------------------------------------------*/
// CREATION DATE: 2002
//------------------------------------------------------------------------*/
// REVISION LOG:
// $Log: dws_getopt.h $
// Revision 1.2  2005/01/26 17:19:35  schwilkd
//
//  0.1  9/18/02     eliminated MFC dependencies. Use new-style headers.
//  0.2  021110      Renamed to dws_getopt, put in DWS namespace
//**************************************************************************

/*! 
 * \file
 * \brief minimal port of unix/linux gnu getopt.h to platform-independent
 *        code.
 */
 

#ifndef DWS_GETOPT_H
#define DWS_GETOPT_H

namespace DWS {


 /*! 
  * \defgroup dws_getopt DWS Getopt function
  * @{
  */

extern int optind;  //!< The \a argv index of the next variable to be processed
extern int opterr;  //!<  Error code
extern char *optarg; //!< Pointer to the current argument.

/*! Command line parser like unix getopt.h
 *
*       The getopt() function parses the command line arguments. Its
*       arguments argc and argv are the argument count and array as
*       passed into the application on program invocation.  In the case
*       of Visual C++ programs, argc and argv are available via the
*       variables __argc and __argv (double underscores), respectively.
*       getopt returns the next option letter in argv that matches a
*       letter in optstring.
*
*       optstring is a string of recognized option letters;  if a letter
*       is followed by a colon, the option is expected to have an argument
*       that may or may not be separated from it by white space.  optarg
*       is set to point to the start of the option argument on return from
*       getopt.
*
*       Option letters may be combined, e.g., "-ab" is equivalent to
*       "-a -b".  Option letters are case sensitive.
*
*       getopt places in the external variable optind the argv index
*       of the next argument to be processed.  optind is initialized
*       to 0 before the first call to getopt.
*
*       When all options have been processed (i.e., up to the first
*       non-option argument), getopt returns EOF, optarg will point
*       to the argument, and optind will be set to the argv index of
*       the argument.  If there are no non-option arguments, optarg
*       will be set to NULL.
*
*       The special option "--" may be used to delimit the end of the
*       options;  EOF will be returned, and "--" (and everything after it)
*       will be skipped.
*
*  \return
*       For option letters contained in the string optstring, getopt
*       will return the option letter.  getopt returns a question mark (?)
*       when it encounters an option letter not included in optstring.
*       EOF is returned when processing is finished.
*
*  \section Bugs
*       -#  Long options are not supported.
*       -#  The GNU double-colon extension is not supported.
*       -#  The environment variable POSIXLY_CORRECT is not supported.
*       -#  The + syntax is not supported.
*       -#  The automatic permutation of arguments is not supported.
*       -#  This implementation of getopt() returns EOF if an error is
*           encountered, instead of -1 as the latest standard requires.
*
*  \section Example
* \code
*       bool TMyApp::ProcessCommandLine(int argc, char *argv[])
*       {
*           using namespace std;
*           int c;
*
*           while ((c = getopt(argc, argv, "aBn:")) != EOF)
*           {
*               switch (c)
*               {
*                   case 'a':
*                       cout << "option a\n";
*                       *
*                       * set some flag here
*                       *
*                       break;
*
*                   case 'B':
*                       cout << "option B\n";
*                       *
*                       * set some other flag here
*                       *
*                       break;
*
*                   case 'n':
*                       cout << "option n: value=" <<  atoi(optarg);
*                       *
*                       * do something with value here
*                       *
*                       break;
*
*                   case '?':
*                       cout << "ERROR: illegal option " << argv[optind-1] << '\n';
*                       return FALSE;
*                       break;
*
*                   default:
*                       cout << "WARNING: no handler for option " << c << '\n');
*                       return FALSE;
*                       break;
*               }
*           }
*           *
*           * check for non-option args here
*           *
*           return TRUE;
*       }
* \endcode
*/
int getopt(int argc, char *argv[], char *optstring);

/*@}*/
}; // namespace DWS
#endif //DWS_GETOPT_H
