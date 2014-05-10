#!/usr/bin/python
# gen.py
# ------
# Dylan W. Schwilk
# script to parse kniche landscape files and produce list realized similarities

import sys, string

class Run:
    def __init__(self):
        self.species = {} # dict
    def Read(self, lines):
        self.id = lines[0].split()[1]
        self.J, self.M, self.T, self.I = lines[2].split()
        self.J = int(self.J)
        for line in lines[4:self.J+4]:
            #get a dict of species niche optima as keys and lists of occupied environments
            # as values
            ls = line.split()
            #print ls
            opt = ls[7]
            env  = float(ls[1])
            el = self.species.get(opt,[])
            if len(el) <1 : self.species[opt] = el
            el.append(env)
            #print el
        return self.J+5 # return next line num

    def ApparentSim(self):
        """Produce list of apparent similarities"""
        #print "apparent sim"
        locations = []
        
        for opt, el in self.species.items():
            locations.append(sum(el) / float(len(el)))

        locations.sort()
        distances = []
        for i,l in enumerate(locations[:-1]):
            distances.append(locations[i+1] - l)

        return sum(distances) / float(len(distances))            
            
            
            
            
            
    
if __name__ == "__main__" :
    
    src = sys.stdin.readlines()
    end = len(src)
    stats = open("Jstats.txt").readlines()
    b = 0
    count = 0
    while 1:
        try :
            r = Run()
            b += r.Read(src[b:end])
            count +=1
            s = r.ApparentSim()
            print "%s\t%f" % (stats[count].strip(), s)
        except :
            break
            
    
    