# Makefile for kniche project
# Author: Dylan Schwilk
# 2014-10-21

bin_PROGRAMS = kniche

# set the include path found by configure
AM_CPPFLAGS = $(all_includes)

# the library search path.
kniche_LDFLAGS = $(all_libraries) 

noinst_HEADERS = mainpage.h nichedefs.h TChooseWeighted.h TEnvGenerator.h TEnvironment.h TLandscape.h TOption.h TPlant.h TPlantVector.h TPopGenerator.h TPopulation.h TRun.h TSpeciesPool.h
kniche_SOURCES = main.cpp mainpage.h TChooseWeighted.cpp TEnvGenerator.cpp TEnvironment.cpp TLandscape.cpp TOption.cpp TPlant.cpp TPlantVector.cpp TPopGenerator.cpp TPopulation.cpp TRun.cpp TSpeciesPool.cpp


kniche_DEPENDENCIES = dws
kniche_LDADD = $(top_builddir)/src/dws/libdws.a
SUBDIRS = dws
