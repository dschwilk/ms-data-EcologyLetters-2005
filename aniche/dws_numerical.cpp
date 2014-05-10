/******************************************************************************/
// FILE NAME:dws_numerical.cpp
// VERSION: 1.0.0
// FUNCTION: Dylan Schwilk's numerical function library
// REMARKS:
//----------------------------------------------------------------------------*/
// Copyright © 2003
/*----------------------------------------------------------------------------*/
// AUTHOR/S:  Dylan W. Schwilk */
//----------------------------------------------------------------------------*/
// CREATION DATE: 020830
//--------+------+---------------------------------------------+------------------*/
// VER    | DATE | CHANGES */
//--------+------+---------------------------------------------+------------------*/
// 1.0.0   030418  These functions were taken from dws_math.h and put here.
//                  I made some minor changes to remove dependency on dws_math
//                  template library.  Now dws_math and dws_numerical are 
//                  independent.
/******************************************************************************/

#include "dws_numerical.h"

#include "dws_math.h"
//#include "dws_random.h"

#include <cmath>
#include <iostream>
#include <assert.h>

namespace DWS{
    // Static constants
    static int MAXIT(100); 
    void SetMAXIT(int it) { MAXIT=it; }

    // local static functions
    REAL_TYPE square(REAL_TYPE a);
    REAL_TYPE betacf(REAL_TYPE a, REAL_TYPE b, REAL_TYPE x) throw(convergence_error);
    void gser(REAL_TYPE *gamser, REAL_TYPE a, REAL_TYPE x, REAL_TYPE *gln) throw(convergence_error); // Num Rec. p. 218.
    void gcf(REAL_TYPE *gammcf, REAL_TYPE a, REAL_TYPE x, REAL_TYPE *gln) throw(convergence_error); // Numerical recipes p. 219

/******************************************************************************
 * Public Functions 
 ******************************************************************************/

    // Function: gammln(x)
    //
    REAL_TYPE gammln(REAL_TYPE xx)
    {
        double x,y,tmp,ser;
        static double cof[6] =
        {
            76.18009172947146,-86.50532032941677,24.01409824083091,
            -1.231739572450155, 0.1208650973866179e-2,-0.5395239384953e-5
        };
        int j;

        y=x=xx;
        tmp=x+5.5;
        tmp -= (x+0.5)*std::log(tmp);
        ser=1.000000000190015;
        for(j=0;j<=5;j++) ser+=cof[j]/++y;
        return -tmp+std::log(2.5066282746310005*ser/x);
    }

    // Function: gammp(a,x)
    //
    REAL_TYPE gammp(REAL_TYPE a, REAL_TYPE x) throw(std::invalid_argument, convergence_error)
    {
        REAL_TYPE gamser, gammcf, gln;

        if((x<0.0||a<=0.0)) throw(std::invalid_argument(std::string("gammp():x must be >= 0 and a must be > 0"))); 
        if(x<(a+1.0))
        {
            gser(&gamser,a,x,&gln);
            return gamser;
        }
        else
        {
            gcf(&gammcf,a,x,&gln);
            return 1.0-gammcf;
        }
    }

    // Function: gammq(a,x)
    //
    REAL_TYPE gammq(REAL_TYPE a, REAL_TYPE x)
    {
        REAL_TYPE gamser, gammcf, gln;

        if((x<0.0||a<=0.0)) throw(std::invalid_argument(std::string("gammp():x must be >= 0 and a must be > 0")));
        if(x<(a+1.0))
        {
            gser(&gamser,a,x,&gln);
            return 1.0-gamser;
        }
        else
        {
            gcf(&gammcf,a,x,&gln);
            return gammcf;
        }
    }


    /* Incomplete beta function
     * ------------------------
     * Numerical Recipes pg 227
     */
    REAL_TYPE betai(REAL_TYPE a, REAL_TYPE b, REAL_TYPE x) throw(std::invalid_argument)
    {
        REAL_TYPE bt;
        if(x<0.00||x>1.00) throw(std::invalid_argument(std::string("betai():x must be between 0.0 and 1.0")));

        if(x==0.0 || x==1.0) bt=0.0;
        else bt=std::exp(gammln(a+b)-gammln(a)-gammln(b)+a*std::log(x)+b*std::log(1.0-x));

        if(x< (a+1.0)/(a+b+2.0))
        {
            return (bt*betacf(a,b,x)/a);
        }
        else
        {
            return(1.0-bt*betacf(b,a,1.0-x)/b);
        }
    }

    REAL_TYPE erff(REAL_TYPE x)
    {
        return x<0.0?1.0+gammp(0.5,x*x):gammq(0.5,x*x);
    }

    /* function: factrl(int n)
     * ------------------------
     * Numerical Recipes p. 214
     */
    REAL_TYPE factrl(int n)
    {
        static int ntop=4;
        static REAL_TYPE a[33]={1.0,1.0,2.0,6.0,24.0};
        int j;
        if(n<0) throw std::invalid_argument("negative factorial in routine factrl");
        if(n>32) return std::exp(gammln(n+1.0));
        while(ntop<n)
        {
            j=ntop++;
            a[ntop]=a[j]*ntop;
        }
        return a[n];
    }


    /* function bico(int n, int k)
     * ----------------------------
     * Binomial coefficient
     */
    REAL_TYPE bico(int n, int k)
    {
        return std::floor(0.5+std::exp(factln(n)-factln(k)-factln(n-k)));
    }


    // function: factln
    // ----------------
    REAL_TYPE factln(int n)
    {
        static REAL_TYPE a[101];

        if (n<0) throw std::invalid_argument("Negative factorial in function factln, dwsmath");
        if(n<=1) return 0.0;
        if (n<= 100) return a[n]?a[n]:(a[n]=gammln(n+1.0));
        else return gammln(n+1.0); // out of range table
    }


    // function: normal_probability
    // ----------------------------
    REAL_TYPE normal_probability(REAL_TYPE distance, REAL_TYPE sd)
    {
        static const double SQRT2PI(2.5066282746310005024147107274575);
        double twoSDSqr = square(2.0*sd);
        return (std::pow(E, - square(distance) / twoSDSqr)  / SQRT2PI  ); // 
    }


    // function: precision
    // -------------------
    REAL_TYPE precision(REAL_TYPE doValue, int nPrecision)
    {
    	static const REAL_TYPE doBase = 10.0;
    	REAL_TYPE doComplete5, doComplete5i;
    	
    	doComplete5 = doValue * std::pow(doBase, (REAL_TYPE) (nPrecision + 1));
    	
    	if(doValue < 0.0)
    		doComplete5 -= 5.0;
    	else
    		doComplete5 += 5.0;
    	
    	doComplete5 /= doBase;
    	std::modf(doComplete5, &doComplete5i);
    	
    	return doComplete5i / std::pow(doBase, (REAL_TYPE) nPrecision);
    }

    // function: sig_fig
    // -------------------
    REAL_TYPE sig_fig(REAL_TYPE x, int sigFigs) throw(std::invalid_argument)
    {
        if(sigFigs <= 0) throw(std::invalid_argument("sig_fig(): sigFigs must be greater than 0."));
    
        int sign;
    	if(x < 0.0)
    		sign = -1;
    	else
    		sign = 1;
    
        x = std::abs(x);
        REAL_TYPE powers = std::pow(10.0, std::floor(std::log10(x)) + 1.0);
        
        return sign * precision(x / powers, sigFigs) * powers;
    }
    

    // function: compare_sig
    // ---------------------
    REAL_TYPE compare_sig(REAL_TYPE x, REAL_TYPE y, int sigFigs) throw(std::invalid_argument)
    {
        return (std::abs(sig_fig(x,sigFigs) - sig_fig(y, sigFigs)) < DWS::EPS); 
    }



///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

    void QuadraticT::Solve(REAL_TYPE& first, REAL_TYPE& second ) const
    {
        REAL_TYPE root = std::sqrt(_b*_b-4*_a*_c)/2*_a;
        first = -_b+ root;
        second = -_b-root;
    }

    REAL_TYPE QuadraticT::DSolve() const 
    {
        return(_b/(-2.0*_a));
    }


  

///////////////////////////////////////////////////////////////////////////////
// Static functions
///////////////////////////////////////////////////////////////////////////////

    inline REAL_TYPE square(REAL_TYPE a)
    {
        return a*a;
    }
     
    // Num Rec. p. 218.
    // Returns incomplete gamma fctn P(a,x) evaluated by its series representation gamser
    void gser(REAL_TYPE *gamser, REAL_TYPE a, REAL_TYPE x, REAL_TYPE *gln) throw(convergence_error)
    {
        REAL_TYPE sum,del,ap;

        assert(x>=0.0);
        *gln=gammln(a);
        if(x<=0.0)
        {
            *gamser=0.0;
            return;
        }
        else
        {
            ap=a;
            del=sum=1.0/a;
            for(int n=1;n<=MAXIT;n++)
            {
                ++ap;
                del *= x/ap;
                if(std::abs(del) < std::abs(sum)*EPS)
                {
                    *gamser=sum*std::exp(-x+a*std::log(x)-(*gln));
                    return;
                }
            }
            throw(DWS::convergence_error(std::string("a too large, MAXIT too small in routine gser")));
        }
    }


    /* Function: gcf
     * -------------
     * Numerical recipes p. 219
     * Returns Incomplete gamma fctn Q(a,x) evaluated by it's continued
     * fraction representation as gammcf
     */
    void gcf(REAL_TYPE *gammcf, REAL_TYPE a, REAL_TYPE x, REAL_TYPE *gln) throw(convergence_error)
    {
        int i;
        REAL_TYPE an,b,c,d,del,h;

        *gln=gammln(a);
        b=x+1.0-a;
        c=1.0/REAL_MIN;
        d=1.0/b;
        h=d;
        for(i=1;i<=MAXIT;i++)
        {
            an=-i*(i-a);
            b+=2.0;
            d=an*d+b;
            if(std::abs(d)<REAL_MIN) d =REAL_MIN;
            c=b+an/c;
            if(std::abs(c) < REAL_MIN) c=REAL_MIN;
            d=1.0/d;
            del=d*c;
            h*=del;
            if(std::abs(del-1.0) < EPS) break;
        }
        if(i>MAXIT) throw(DWS::convergence_error("a too large, MAXIT too small in routine gcf"));
        *gammcf=std::exp(-x+a*std::log(x)-(*gln))*h;
    }

    /* Function: betacf
     * ----------------
     * Used by betai.  evaluates continued fractin for incomplete beta function by modified
     * Lentz's method.
     */
    REAL_TYPE betacf(REAL_TYPE a, REAL_TYPE b, REAL_TYPE x) throw(convergence_error)
    {
        int m,m2;
        REAL_TYPE h, aa,c,d, del,qab, qam, qap;
        
        qab=a+b;
        qap=a+1.0;
        qam=a-1.0;
        c=1.0;
        d = 1.0-qab*x/qap;
        if(std::abs(d) < REAL_MIN) d=REAL_MIN;
        d=1.0/d;
        h=d;

        for(m=1;m<=MAXIT;m++)
        {
            m2=2*m;
            aa=m*(b-m)*x/((qam+m2)*(a+m2));
            d=1.0+aa*d;
            if( std::abs(d) < REAL_MIN) d=REAL_MIN;
            c=1.0+aa/c;
            if(std::abs(c) < REAL_MIN) c=REAL_MIN;
            d=1.0/d;
            h*=d*c;
            aa= -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
            d=1.0+aa*d;
            if( std::abs(d) < REAL_MIN) d=REAL_MIN;
            c=1.0+aa/c;
            if(std::abs(c) < REAL_MIN) c=REAL_MIN;
            d=1.0/d;
            del=d*c;
            h *= del;
            if(std::abs(del-1.0) < EPS) break;
        }
        if(m>MAXIT) throw(DWS::convergence_error("MAXIT too small in function betacf()"));
        return h;
    }



}; // namespae DWS
