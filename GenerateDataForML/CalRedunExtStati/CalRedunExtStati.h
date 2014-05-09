#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

// call calculation one client's redundancy whose traffic is stored in srcFileName
// extract correspoing traffic statistic information during redundancy calculation
// write redundancy rate and statistic information into output
// use MAXP and MODP algorithm to calculate redundancy and algorithm's arguments 
// are assigned as blocksize and sampling rate

void ServeAClient(const char *srcFileName, FILE *output, int blockSize, int samplingRate);

// calculate redundancy for a group client 
// this function will run as a thread

void* ServeGroupClient(void* threadArgu);
//void ServeGroupClient(const vector<string> &nameList, int index, const char *dstFileName);

// read all files of client traffic data
// and classify them into one groups
// one thread will handle one group

void DispatchClient(const char *srcDirName, const char *dstFileName);

