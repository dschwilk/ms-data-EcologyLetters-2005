#!/usr/bin/python
# gen-J.py
# ------
# Dylan W. Schwilk
# script to generate input files for kniche
# v4 for use with kniche v4.2

#import string

#NB = (0.01, 0.02, 0.03, .05, 0.7, 0.1)
#DB = (10,20,30,50,70,100)
DB=(50,)
# in landscape units, enter as if for lsize=1000, this will be normalized below under some versions

NB = (0.05,)
# in env units
IM = (1.0e-4,)
#THRESH = 0.0
Stable_cycles = 5

J = (101,201,501, 1001,1501,2001,2501)
#E = (0.01, 0.1,0.2,0.5,1.0,1.5,2.0,2.5)
E = (1.0,)
NSpecies =(2000,)

for j in J :
    for sp in NSpecies :
                for e in E  :
                    for im in IM :
                        for nb in NB :
                            for db in DB :
                                norm_db = db * j / 1000.0
                                EGen = "LinearGenerator_%f_%f" % (e , 0) 
                                MCGen = EGen  # Local and global environments the same
                                print "%d\t%f\t%s\t%s\t%d\t%d\t%f\t%f" % (j,im,EGen, MCGen,sp,Stable_cycles,nb,norm_db)


# gen.py
