#ifndef DS_H_
#define DS_H_

#include <string>
using namespace std;

class Session {
public:
	string etherAddr;
	int startTime;
	int endTime;
};

class EtherPattern  {
public:
	int connections;	// connection times to wifi
	int avgTime;		// average connection time 
	int varTime;		// variance time for each connection
};

class Connection  {
public:
	string etherAddr;
	string ipAddr;
	int startTime;
	int endTime;
};

#endif	// DS_H_