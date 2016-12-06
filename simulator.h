#ifndef simulator_h
#define simulator_h

#include <stdio.h>
#include <stdlib.h>

//Sorting process
void process();

//Simulator functions
void init(int psize, int winsize);
void put(unsigned int address,int value);
int get(unsigned int address);
void done();

#endif