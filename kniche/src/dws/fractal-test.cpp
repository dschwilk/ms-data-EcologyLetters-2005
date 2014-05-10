/******************************************************************************/
// FILE NAME: main.cpp
// VERSION: 1.3
// REMARKS: main function in dniche simulation
//------------------------------------------------------------------------------
// Copyright © 2002, 2003, 2004                                                         
//------------------------------------------------------------------------------
// AUTHOR/S: Dylan Schwilk (pricklysoft.org)
//------------------------------------------------------------------------------
// CREATION DATE: 21/09/02 08:05
//--------+----------+--------------------------------------+-------------------
//   VER      DATE    CHANGES
//--------+----------+--------------------------------------+------------------      
/******************************************************************************/

#include "dws_getopt.h"
#include "dws_fractal.h"

#include <fstream>
#include <cstring>
#include <stdexcept>
#include <ctime>


// Constants
static const char* PROGRAM = "fractal-test";
static const char* VERSION = "1.1";
static const char* AUTHOR = "Dylan Schwilk (www.pricklysoft.org)";

// Functions
bool ProcessOptions(int argc, char *argv[]);
void PrintHelp();


// Static vars - defaults
static char PopHistoryOut[256] = "\0";
static char LandscapeOut[256] = "\0";
static char Input[256] = "\0";
static unsigned NReps = 1;
static long RSeed = 0;


// Function: main
// --------------
int main(int argc, char *argv[])
{
    using namespace std;

    int lsize(1000);
    double H(1.5);
    double scale(1);
        double mi=0;
    double ma=0;
    
    if(RSeed == 0) RSeed = ((long) std::time(NULL)); // default if no seed provided by options
    if(!ProcessOptions(argc, argv)) exit(1);
    DWS::set_randomizer_seed(RSeed);

    std::vector<std::complex<double> > V(lsize);
    
    DWS::spectral_synthesis_1D( V.begin(), lsize, H);

    for(unsigned i=0;i<lsize;i++) {
        mi = std::min(V[i].real(), mi);
        ma = std::max(V[i].real(), ma);
    }
    // normalize
    for(unsigned i=0;i<lsize;i++) {
        cout << ((V[i].real() - mi) / (ma-mi)) * scale << '\n';
    }
    cout << endl;
    
    return 0;
} // main()



// Function: ProcessOptions
// ------------------------
bool ProcessOptions(int argc, char *argv[])
{
    using namespace std;
    char opt;

    while ((opt = DWS::getopt(argc, argv, "hvr:l:p:s:")) != EOF) {
        switch (opt) {
          case 'h':
            PrintHelp();
            exit(0);
          case 'r':
            NReps = atoi(DWS::optarg);
            break;
          case 's':
            RSeed = atoi(DWS::optarg);
            break;
          case 'l':
            strcpy(LandscapeOut, DWS::optarg);
            break;
          case 'p':
            strcpy(PopHistoryOut, DWS::optarg);
            break;
          case 'v':
            cout << PROGRAM << " version " << VERSION << ", by " <<  AUTHOR << endl;
            exit(0);
          case '?':
            cout << "ERROR: illegal option " << argv[DWS::optind-1] << '\n';
            return false;
            break;


          default:
            cout << "WARNING: no handler for option " << opt << '\n';
            return false;
            break;
            }
        }
    if(DWS::optarg) strcpy(Input, DWS::optarg);
    return true;
}


// Function: PrintHelp
// -------------------
void PrintHelp()
{
    using namespace std;
//    cout << PROGRAM << ' ' << VERSION << '\n';
    cout << "Usage: " << PROGRAM << " [hv] [-r NUMBER] [-l FILE] [INPUT FILE]\n";
    cout << "options:\n" ;
    cout << "-h\t\thelp\t\t\tPrint this help message\n";
    cout << "-l FILE\t\tlandscape output\tOutput file for landscape output\n";
    cout << "-p FILE\t\tpop history\t\tOutput file for population histories\n";
    cout << "-s NUMBER\tseed\t\t\tSet random number seed\n";
    cout << "-r NUMBER\treps\t\t\tSet number of reps per input line\n";
    cout << "-v\t\tversion\t\t\tPrint version info\n";
}

// main.cpp
