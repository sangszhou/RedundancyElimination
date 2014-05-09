#ifndef _RABINKARP_H_
#define _RABINKARP_H_

#include <vector>

using namespace std;


void RabinKarpAlo(const char *payload, int payLoadLen, vector<unsigned int> &keys, int blockSize, int samplingRate);

#endif