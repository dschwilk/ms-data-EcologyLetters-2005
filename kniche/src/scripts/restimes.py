# popparser.py
# ------------
# determine residence times
#

import string, sys
from operator import add, mul


mean = lambda v : float((reduce(add,v))) / float(len(v))    

def getResTimes(run):
    '''Return average residence time for each species'''

    print 'Run %s has %d species and %d time points' % (run[0].split()[1], (len(run) - 3), len(run[3].split()) -1 )
    ave_times = []

    counting = 0
    n = 0
    for i in range(3,len(run) ) :
        pops = run[i].split()
        rtimes = [] # new list of res times
        popsizes = []
        count = 0
        for p in pops[501:] : #skip species id
            num = int(p)
            if num > 0:
                count += 1
            elif num == 0 :
                if count > 0 :
                    rtimes.append(count)
                    count=0
            else:
                print 'Error'

        if count :
            rtimes.append(count)
            
        if rtimes :
            ave_times.append(mean(rtimes))

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

    distributions = map(getResTimes, splitIntoRuns(lines) )

    i = 1
    for rtimes in distributions :
        f = open('r%d-rtimes.txt' % i, 'w')
        for t in rtimes :
            f.write('%d' % t)
            f.write('\n')
        f.close()
        i += 1
           
            

    
        

    
