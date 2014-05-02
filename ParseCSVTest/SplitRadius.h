#ifndef SPLITRADIUS_H
#define SPLITRADIUS_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
using namespace std;


char** str_split(char* str, char delim, int *numSplits);

// set year to 2013 and month to 1 for default
int ConvertStrToTimeStamp(const char* str, const int _day);

#endif