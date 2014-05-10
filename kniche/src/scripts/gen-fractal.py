#!/usr/bin/python
# gen-fractal.py
# ----------------
# Dylan W. Schwilk
# script to generate input files for kniche
# For use with kniche v4.5

import string

#NB = (0.01, 0.02, 0.03, .05, 0.7, 0.1)
DB = (25, 50, 75, 100)
#DB=(50,)
# in landscape units, enter as if for lsize=1000, this will be normalized below under some versions

NB = (0.05,)
# in env units
IM = (0.1,)
epochs = 5000
scale =0.5
H = (0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0)
J = (1000,)
NSpecies =(2500,)

for j in J :
    for sp in NSpecies :
                for h in H  :
                    for im in IM :
                        for nb in NB :
                            for db in DB :
                                norm_db = db * j / 1000.0
                                EGen = "FractalGenerator_%f_%f" % (h , scale) 
                                MCGen = EGen  # Local and global environments the same
                                print "%d\t%f\t\t%s\t%s\t%d\t%d\t%f\t%f" % (j,im,EGen, MCGen,sp,epochs,nb,norm_db)


