#ifndef LOADDATA_H
#define LOADDATA_H

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include "SplitRadius.h"
#include "DataStruct.h"
using namespace std;

// map<string, vector<Session>> is IP - Session relation
// map<string, vector<EtherPattern>> is ethernet address connection properities

// load csv and store to map
// raw csv data is incomplete due to packets loss
// error handling and information repairing is needed 
void LoadSession(const char*, map<string, vector<Session> >&);

// write session to disk
void StoreSession(const char*, const map<string, vector<Session> >&);

// extract session information from IP-Session relationship
void LoadEtherPattern(const map<string, vector<Session> >&, map<string, vector<EtherPattern> >&);

// write client ethernet and corresponding to disk
void StoreEtherPattern(const char*, map<string, vector<EtherPattern> >&);

#endif