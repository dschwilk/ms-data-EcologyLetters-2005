#
# Makefile for 'aniche'.
#
# Type 'make' or 'make aniche' to create the executable.
# Type 'make clean' to delete all temporaries.
#

CC = g++

# build target specs
CFLAGS = -g -Wall -DDEBUG 
OUT_DIR = debug_build
LIBS = 

# first target entry is the target invoked when typing 'make'
default: aniche

aniche: $(OUT_DIR)/dws_getopt.cpp.o $(OUT_DIR)/dws_numerical.cpp.o $(OUT_DIR)/main.cpp.o
	$(CC) $(CFLAGS) -o aniche $(OUT_DIR)/dws_getopt.cpp.o $(OUT_DIR)/dws_numerical.cpp.o $(OUT_DIR)/main.cpp.o $(LIBS)

$(OUT_DIR)/dws_getopt.cpp.o: dws_getopt.cpp dws_getopt.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/dws_getopt.cpp.o -c dws_getopt.cpp

$(OUT_DIR)/dws_numerical.cpp.o: dws_numerical.cpp dws_numerical.h \
 dws_math.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/dws_numerical.cpp.o -c dws_numerical.cpp

$(OUT_DIR)/main.cpp.o: main.cpp dws_getopt.h dws_numerical.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/main.cpp.o -c main.cpp

clean:
	rm -f aniche $(OUT_DIR)/*.o

profile:
	gprof aniche > profile.txt; ./gprof2dot.py profile.txt | dot -Tpng -o profile.png
