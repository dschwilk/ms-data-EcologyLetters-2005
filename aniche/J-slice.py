#!/usr/bin/python

# script to produce input for a-niche
# example use: J-slice.py | a-niche -s 

# Parameters

J = range(2341, 3003,2)
E = [1.0,]
NB = [0.05,]

if __name__ == "__main__"   :
    """"Main program:just produces an input stream: one
        paramter list per line."""
    for j in J:
        for e in E:
         #   for db in DB:
                for nb in NB:
                    print "%d %f %f %d\n" % (j, e, nb, j/20.0),
   
