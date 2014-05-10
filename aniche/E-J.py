#!/bin/python
# E-J.py

#!/usr/bin/python

# script to produce input for a-niche
# example use: E-J.py | a-niche -s 

# Parameters

J = [100, 200, 500, 1000, 1500, 2000, 2500]
E = [0.01, 0.1, 0.2, 0.5, 1.0, 1.5, 2.0, 2.5]
NB = [0.05,]

if __name__ == "__main__"   :
    "Produces parameter lists for a-niche-0.3"
    for j in J:
        for e in E:
         #   for db in DB:
                for nb in NB:
                    db = j/20 # scale db to J
                    print "%d %f %f %d" % (j, e, nb, db)
