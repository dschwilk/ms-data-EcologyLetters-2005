///////////////////////////////////////////////////////////////////////////
// FILE NAME: main.cpp
// VERSION: $Revision: 1.8 $
// REMARKS: main function in kniche simulation
// AUTHOR/S: Dylan Schwilk (pricklysoft.net) 
// Copyright © 2002, 2003, 2004, 2005     
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// GNU
// This library is free software; you can redistribute it and/or modify it
// under the terms oibrary General Public License as published 
// by the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.  This library is distributed in the
// hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU Library General Public License for more details.
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
///////////////////////////////////////////////////////////////////////////

#include "../config.h"

#if defined(HAVE_GETOPT_H)
  #include <getopt.h>
#else
  #include "dws/dws_getopt.h"
#endif

#include "dws/dws_stlstats.h"
#include "dws/dws_numerical.h"

#include "TRun.h"

#include <fstream>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <ctime>

// Constants

static const char* PROGRAM = "kniche";
//static const char* VERSION = "4.5 $Revision: 1.8 $ ";
static const char* AUTHOR = "Dylan Schwilk";

// Functions
bool ProcessOptions(int argc, char *argv[]);
void PrintHelp();
void Run(std::istream* runs, std::ostream* out,
         std::ostream* landout = NULL, std::ostream* popout=NULL);

// Static vars - defaults
static char PopHistoryOut[256] = "\0";
static char LandscapeOut[256] = "\0";
static char Input[256] = "\0";
static unsigned NReps = 1;
static long RSeed = 0;
bool Monotonic = false;

// Function: main
// --------------
int main(int argc, char *argv[])
{
    using namespace std; 
    
    if(RSeed == 0) RSeed = ((long) std::time(NULL)); // default if no seed provided by options
    if(!ProcessOptions(argc, argv)) exit(1);

    ifstream in;
    ofstream lo;
    ofstream po;
    if(Input[0]) in.open(Input);
    if(PopHistoryOut[0]) po.open(PopHistoryOut);
    if(LandscapeOut[0]) lo.open(LandscapeOut);
    // Run: input is cin or in, output is cout, long output is lo or NULL

    // Set random # generator
    DWS::set_randomizer_seed(RSeed);
    Run((Input[0] ? &in : &cin), &cout, (LandscapeOut[0] ? &lo : NULL),
        (PopHistoryOut[0] ? &po : NULL));
    return 0;
} // main()


// Function: Run
// -------------
void Run(std::istream* runs, std::ostream* out, std::ostream* landout,
         std::ostream* popout /*=NULL*/)
{
    using namespace DWS;
    int runcount = 0;
    TRun::WriteHeader(*out);
    *out << "\tdiversity\tdivRunningAve\tmeanRealSim";
    *out << "\tmean_dist_sd\tmean_niche_sd\toverlap\ttime" << std::endl;
    while(runs->good()) {
        try {
            char runstr[1000];
            runs->getline(runstr,1000);
            for(unsigned i=0;i<NReps;i++) {
                TRun R;
                std::stringstream s(runstr);
                s >> R;
                runcount++;
                R.Initialize();
                R.Run(runcount, out, landout, popout);
                if(Monotonic)
                {
                    R.GetLandscape()->Repopulate();
                    R.GetLandscape()->MakeEnvMonotonic();
                    R.Run(runcount, out, landout, popout);
                }
            }
        }  catch(std::runtime_error e) {
             std::cerr << e.what() << std::endl;
            break;
        }
    }     
}


// Function: ProcessOptions
// ------------------------
bool ProcessOptions(int argc, char *argv[])
{
  #ifndef HAVE_GETOPT_H
    using namespace DWS;
  #endif
    using namespace std;
    char opt;

    while ((opt = getopt(argc, argv, "hvmr:l:p:s:")) != EOF) {
        switch (opt) {
            case 'h':
                PrintHelp();
                exit(0);
            case 'v':
                cout << PROGRAM << " version " << VERSION << ", by " <<  AUTHOR << endl;
                exit(0);
            case 'm':
                Monotonic = true;
                break;
            case 'r':
                NReps = atoi(optarg);
                break;
            case 's':
                RSeed = atoi(optarg);
                break;
            case 'l':
                strcpy(LandscapeOut, optarg);
                break;
            case 'p':
                strcpy(PopHistoryOut, optarg);
                break;
            case '?':
                cout << "ERROR: illegal option " << argv[optind-1] << '\n';
                return false;
                break;
            default:
                cout << "WARNING: no handler for option " << opt << '\n';
                return false;
                break;
            }
        }
    if(optarg) strcpy(Input, optarg);
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
    cout << "-v\t\tversion\t\t\tPrint version info\n";
    cout << "-l FILE\t\tlandscape output\tOutput file for landscape output\n";
    cout << "-p FILE\t\tpop history\t\tOutput file for population histories\n";
    cout << "-m\t\tmonotonic\t\tRun landscape twice; second time\n";
    cout << "\t\t\t\t\twith environment ordered as monotonic\n";
    cout << "-s NUMBER\tseed\t\t\tSet random number seed\n";
    cout << "-r NUMBER\treps\t\t\tSet number of reps per input line\n";
}

// main.cpp
