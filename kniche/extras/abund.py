# abund.py
# ------------
# determine abundances from landscape file
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
    

def getAbund(run):
    counting = 0
    abundances = {}
    positions = run[4:]

    for pos in positions:
        #print pos
        p,e,sp = pos.split()
        p = int(p)
        e = float(e)
        sp = int(sp)
        a = abundances.get(sp,0)
        a+=1
        abundances[sp] = a

    result = []
    for sp, a in abundances.items():
        result.append((a,sp))
        
    return result


def splitIntoRuns(lineList) :
    print 'split into runs'
    sp = 0
    cp = 1
    result = []
    while cp < len(lineList) :
        if lineList[cp][0:3] == 'RUN' :
            result.append(lineList[sp:cp-1])
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

    distributions = map(getAbund, splitIntoRuns(lines) )
    for r, d in enumerate(distributions):
        d.sort()
        d.reverse()
        for a,sp in enumerate(d):
            print "%d\t%d" % (r+1,a+1),
            print "\t%d\t%d" % sp

##    i = 1
##    for rtimes in distributions :
##        f = open('r%d-rtimes.txt' % i, 'w')
##        for t in rtimes :
##            f.write('%d' % t)
##            f.write('\n')
##        f.close()
##        i += 1
           
            

    
        

    
