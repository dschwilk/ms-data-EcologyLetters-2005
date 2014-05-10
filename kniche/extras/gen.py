# gen.py
# ------
# Dylan W. Schwilk
# script to generate input files for kniche
# v3 for use with kniche v4

import string

#NB = (0.01, 0.02, 0.03, .05, 0.7, 0.1)
#DB = (10,20,30,50,70,100)
DB=(50,)
# in landscape units, enter as if for lsize=1000, this will be normalized below under some versions

NB = (0.05,)
# in env units
IM = (1.0e-4, 1.0e-3, 1.0e-2)
THRESH = 0.0
Stable_cycles = 10

LSize = (100, 500, 1000, 1500, 2000, 2500)
NSpecies =(2000,)
Env = (0.0, 0.5, 1.0, 1.5, 2.0, 2.5)
#Env = (0.0,)


for l in LSize :
    for sp in NSpecies :
                for e in Env :
                    for im in IM :
                        for nb in NB :
                            for db in DB :
                                norm_db = db * l / 1000.0
                                print "%d\t%f\t%f\tLinearGenerator_%f\t%d\t%d\t%f\t%f" % (l,THRESH,im,e,sp,Stable_cycles,nb,norm_db)


# gen.py
