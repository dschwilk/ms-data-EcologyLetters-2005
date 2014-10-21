/******************************************************************************/
// FILE NAME: main.cpp
// VERSION: 0.3
// REMARKS: main function in a-niche numerical approximation
//------------------------------------------------------------------------------
// Copyright © 2003                                                         
//------------------------------------------------------------------------------
// AUTHOR/S: Dylan Schwilk, David Ackerly (Stanford University)
//------------------------------------------------------------------------------
// CREATION DATE: 30/10/03 09:37
//--------+----------+--------------------------------------+-------------------
//   VER      DATE    CHANGES
//--------+----------+--------------------------------------+-------------------
//    0.2   031031      Added L searching to C++ code, and removed from python
//                      front-end -- required adding sig digits checking to
//                      the dws_numerical library.
//    0.3   031125      Changed behavior so prog will loop through input
//                      lines and produce a full line of output for each set
//                      of input parameters.
/******************************************************************************/

#include "dws_getopt.h"
#include "dws_numerical.h"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <stdio.h>
#include <set>

typedef std::vector<double> DV;

// Constants
static const char* PROGRAM = "a-niche";
static const char* VERSION = "0.3";
static const char* AUTHOR = "Dylan Schwilk, David Ackerly; 2003";

// global vars with defaults
static bool SEARCH_FOR_L(false);
static int SIG_DIGITS(4);
static int J(1001);
static double E(1.0),  L(0.001), nb(0.005), db(50);
static int diversity(0);  // resident diversity


// Functions
void FillLandscape(DV& outV);
double E_at_X(int x); 
double InvasionProb(const DV& landscape);
double DoSearch(double& lastprob);
bool ProcessOptions(int argc, char *argv[]);
void PrintHelp();

// Function: main
// --------------
int main(int argc, char *argv[])
{
    if(!ProcessOptions(argc, argv)) std::exit(1);
    
    if(SEARCH_FOR_L) {
      while(std::cin.good()) {
        std::cin >> J >> E >> nb >> db;
        if(!std::cin.good()) break;
        double lastprob = 0;
        if (J%2 ==0) J++;
        double l = DoSearch(lastprob);
        std::cout << J << '\t' << E << '\t' << nb << '\t' << db << '\t' << diversity << '\t' << l << '\t' << lastprob << std::endl;
      }
    } else {
      while(std::cin.good()){
        std::cin >> J >> E >> L >> nb >> db; 
        if (J%2 ==0) J++;  
        DV landscape(J);
        FillLandscape(landscape);  
        std::cout << J << '\t' << E << '\t' << L << '\t' << nb << '\t' << db << '\t' << diversity << '\t' << InvasionProb(landscape) << '\t' << 1.0 / J << std::endl;
      }
    }
    return 0;
} // main()


// E_at_X
// ------
// Calculate Env value at position x
double E_at_X(int x) 
{
    return ( (((double) x) / J) * E);
}


// FillLandscape 
// -------------
void FillLandscape(DV& outV)
{
    diversity = 0;
    int middle, pop_start;
    double nopt;
    outV.resize(J);
        
    middle = J/2;  // assume J is odd
    
    nopt = E_at_X(middle+1) + 0.5*L;
    pop_start = middle+1;
    diversity=1;
    // assign niche optima forward
    for(int i=middle+1; i<J;i++) {
        if (E_at_X(i) - E_at_X(pop_start) > L) {
            nopt += L;
         //   if(nopt > E) nopt = E;  // so no optimum is beyond the Env range  -- ??
            pop_start = i;
            diversity++;
        }
        outV[i] = nopt;
    }
    
    // assign backward
    for(int i=middle-1; i >= 0; i--){
        outV[i] = E - outV[J-1-i];
    }
    
    diversity*= 2;
    
    //assign middle (invader)
    outV[middle] = E_at_X(middle);
}


// InvasionProb()
// --------------
// Calculate probability of immigrant invading.
double InvasionProb(const DV& landscape)
{
    using namespace DWS;
    double i_prob = 0;
    int middle(J/2);
    std::vector<DV> prob_matrix(J);
    
    //make res matrix
    
    for(int x=0;x<J;x++) {
    DV residents(J);
        for(int r=0;r<J;r++) {
            residents[r] = normal_probability(r-x, db) * normal_probability(E_at_X(x) - landscape[r], nb);
     //       std::cout << x << '\t' << r << '\t' << residents[r] << std::endl;
        }
        prob_matrix[x] = residents;
    }
        
    //std::cout << "matrix made" << std::endl;
    
    // calculate probs
    for(int x=0; x<J;x++) {
        double normalizer = std::accumulate(prob_matrix[x].begin(), prob_matrix[x].end(), 0.0);
        double invasion = (normal_probability(middle-x, db) * normal_probability(E_at_X(x) - landscape[middle], nb));
        if (x!=middle) i_prob += (invasion / normalizer);  // leaving middle out of sum
      //  r_prob += (normalizer - invasion) / normalizer;
    }
 //   std::cout << i_prob << '\t' << r_prob << std::endl;
    return i_prob / (J-1);  // why not /J ?
}



// Function: DoSearch
// ------------------
// Search for minimum limiting sim value that allows invasion.
// Simple binary search that works ok -- faster would be linear
// interpolation.
double DoSearch(double& lastprob)
{
    DV landscape(J);
    double Min(0.0), Max(E), InvProb(0);

    L = (Max - Min) / 2.0;
    while(!DWS::compare_sig(Max, Min,SIG_DIGITS)){
         FillLandscape(landscape); 
         InvProb = InvasionProb(landscape);
         // Now see which side of 1/J we fell and ajust max, min:
         if (InvProb >  1.0/J) {  // prob of death is 1/J
             Max = std::min(Max, L);
         } else {
             Min = std::max(Min, L);
         }
         L = (Max + Min) / 2.0;
         //std::cout << L << std::endl;
    }
    lastprob = InvProb;
    return (L);
}


// Function: ProcessOptions
// ------------------------
bool ProcessOptions(int argc, char *argv[])
{
    using namespace std;
    char opt;

    while ((opt = DWS::getopt(argc, argv, "hvd:s")) != EOF) {
        switch (opt) {
          case 'h':
            PrintHelp();
            exit(0);
          case 'v':
            cout << PROGRAM << " version " << VERSION << ", by " <<  AUTHOR << endl;
            exit(0);
          case 'd':
            SIG_DIGITS = std::abs(atoi(DWS::optarg));
            break;
          case 's':
            SEARCH_FOR_L = true;
            break;
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
    return true;
}


// Function: PrintHelp
// -------------------
void PrintHelp()
{
    using namespace std;
//    cout << PROGRAM << ' ' << VERSION << '\n';
    cout << "Usage: " << PROGRAM << " -[hvs] [-d NUMBER]\n";
    cout << "options:\n" ;
    cout << "-h\t\thelp\t\tPrint this help message\n";
    cout << "-d NUMBER\tdigits\t\tSignificant digits for L\n";
    cout << "-s \t\tsearch\t\tSearch for L\n";
    cout << "-v\t\tversion\t\tPrint version info\n";
}
