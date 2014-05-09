#ifndef CHOOSEMISSION_H_
#define CHOOSEMISSION_H_

#include <vector>
#include <string>

using namespace std;

// chose user traffic files from data set
// and store qulified files to vector

void FilterMission(vector<string> &missionList, const char *dirPath);

// save file names to hard disk
// this file names will be used in redundancy calculation,
// statistic information extraction and fit cache size calculation

void SaveMission(const vector<string> &missionList, const char *filePath);

// load file names when calculate redundancy, statistic information
// and fit cache size
	
void LoadMission(vector<string> &missionList, const char *filePath);

#endif