# trans-res-div.py
# ------------
# Determines diversity of transient and resident species
# in a population history run produced by kniche -pFILE.
#

import string, sys
from operator import add, mul

TRANSIENT_TIME = 100

mean = lambda v : float((reduce(add,v))) / float(len(v))    

def getResTransDiversity(run):
    '''Return run parameters, diversity of residents, diversity of transients for run'''

    #print 'Run %s has %d species and %d time points' % (run[0].split()[1], (len(run) - 3), len(run[3].split()) -1 )
    residents = 0
    transients = 0

    for i in range(3,len(run) ) :
        run_params = run[1]
        pops = map(int,run[i].split())
        #print 'species: %s, final popsize: %s' % (pops[0], pops[-1])
        if pops[-1] > 0 :   # only do diversity on species that existed at end
            #print pops[-1]
            mtime = mean(getResTimes(pops[1:]))
            if mtime >  TRANSIENT_TIME :
                residents += 1
            else :
                transients += 1
            
    return (run_params, residents, transients)


def getResTimes(pop_history) :
    '''Returns list of residence times for Species population history'''
    rtimes = [] # new list of res times
    count = 0
    for p in pop_history :
        num = int(p)
        if num > 0:
            count += 1
        elif num == 0 :
            if count > 0 :
                #print count
                rtimes.append(count)
                count=0
        else:
            print 'Error'
    if count > 0 :
        rtimes.append(count) # get final res time
    return rtimes
        

        

def splitIntoRuns(lineList) :
    '''split population history outputinto runs'''
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

    diversities = map(getResTransDiversity, splitIntoRuns(lines) )

    for d in diversities :
        print '%s\t%d\t%d' % (d[0][0:-1], d[1], d[2])
            

    
        

    
