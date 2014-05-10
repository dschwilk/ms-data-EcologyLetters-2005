# popparser.py
# ------------
# determine residence times
#

import string, sys
from operator import add, mul


mean = lambda v : float((reduce(add,v))) / float(len(v))    

def getRSA(run):
    '''Get species abundance curve data for run endpoint'''
    result = {}
    for i in range(3, len(run)):
        history = run[i].split()
        abund = int(history[-1])
        if abund :
            result[history[0]] = abund
    return result


def getResTimes(run):
    '''Return residence time for each species in final landscape'''

#    print 'Run %s has %d species and %d time points' % (run[0].split()[1], (len(run) - 3), len(run[3].split()) -1 )
    ave_times = {}

    counting = 0
    n = 0
    for i in range(3,len(run) ) :
        hist = run[i].split()
        if not int(hist[-1]) : continue # only count for species in final landscape
        species = hist[0]
        pops = map(int, hist[1:]) 
        pops.reverse()
        try :
            invasion = pops.index(0) + 1
        except :
            invasion = len(pops)
            
        ave_times[species] = invasion
    return ave_times


def splitIntoRuns(lineList) :
    print 'split into runs'
    sp = 0
    cp = 1
    result = []
    while cp < len(lineList) :
        if lineList[cp][0:3] == 'RUN' :
            result.append(lineList[sp:cp])
            sp = cp
            cp += 1
        else :
            cp += 1
    result.append(lineList[sp:])
    print 'file has %d runs' % len(result)
    return result





if __name__== '__main__' :
    if len(sys.argv) > 1 :
        lines = open(sys.argv[1]).readlines()
    else :
        lines = sys.stdin.readlines()
        
    print 'file is %d lines long' % len(lines)

    for run in splitIntoRuns(lines):
        rsa = getRSA(run)
        rtimes = getResTimes(run)
        abundances = []
        for key in rsa.keys() :
            abundances.append((rsa[key], key))
        abundances.sort()
        abundances.reverse()
        for i in range(len(abundances)) :
            print "%s\t%s\t%d\t%d\t%d" % (run[1].strip(), abundances[i][1], abundances[i][0], i+1, rtimes[abundances[i][1]])
    


        

    
