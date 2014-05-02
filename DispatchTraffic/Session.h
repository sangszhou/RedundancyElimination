
#ifndef SESSION_H_
#define SESSION_H_

#include <string>
using namespace std;

class Session  {
public:
	Session(const int &_starTime, const int &_endTime, const string &_etherAddr)
	: startTime(_starTime), endTime(_endTime), etherAddr(_etherAddr) {}

	Session()  {
		Session(0, 0, "");
	}
public:
	int startTime; 	// timestamp
	int endTime;
	string etherAddr;
};


#endif /* SESSION_H_ */
