# popparser.py
# ------------
# determine residence times
#

import string, sys
from operator import add, mul

def parseLine(line):
    result = {}
    l = line.split()
    for p in l[1:] :
        id, ab = p.split(":")
        result[int(id)]=int(ab)
    return result
    
def getResTimes(run):
    rtimes = []
    counting = 0
    n = 0
    abund=map(parseLine,run[4:])
    for s in range(2501):
        #print s
        count = 0
        for d in abund:
            if d.get(s,0) > 0 : count+=1
            elif count > 0 : rtimes.append(count); count = 0
            else : count = 0

    return rtimes

def getFinalResTimes(run):
    rtimes = []
    abund = map(parseLine, run[4:])
    abund.reverse()
    finalAbund = abund[0]

    for species in finalAbund.keys():
        count = 1
        for d in abund :
            if d.get(species,0) > 0 : count += 1
            else :break
        rtimes.append(count)
    
    return rtimes


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


mean = lambda v : (reduce(add,v)) / len(v)    


if __name__== '__main__' :
    if len(sys.argv) > 1 :
        lines = open(sys.argv[1]).readlines()
    else :
        lines = sys.stdin.readlines()
        
    print 'file is %d lines long' % len(lines)

    distributions = map(getResTimes, splitIntoRuns(lines) )
    for d in distributions:
        for t in d:
            print t

##    i = 1
##    for rtimes in distributions :
##        f = open('r%d-rtimes.txt' % i, 'w')
##        for t in rtimes :
##            f.write('%d' % t)
##            f.write('\n')
##        f.close()
##        i += 1
           
            

    
        

    
