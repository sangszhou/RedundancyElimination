#ifndef CalFitCacheSize_H_
#define CalFitCacheSize_H_

#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

// canche size will be set between 10M to 200M
// it can be presented by type int

void DispatchMission(const char *fileName);

// used to create threads

void HandleGroupMission(const vector<string> &missionList, int threads, int cacheSize);


void HandleAMission(const char *srcFileName, FILE *output, int blockSize, int samplingRate, int cacheSize, double &redun);


#endif