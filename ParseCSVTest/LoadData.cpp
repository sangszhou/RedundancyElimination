#include "LoadData.h"
#include <set>
using namespace std;

void LoadSession(const char* filename, map<string, vector<Session> >& IPLog)  {

	// num of mac address
	set<string> mac_addr;

	char zeroTime[20] = "00:00:00";
	char fullTime[20] = "23:59:59";
	const int FIRSTTIMESTAMP = ConvertStrToTimeStamp(zeroTime, 4);
	const int LASTTIMESTAMP  = ConvertStrToTimeStamp(fullTime, 4);
	map<string, Connection> connectionLog;

	FILE *fileHandler = fopen(filename, "r");
	int lineno = 0;	//line number
	char nextstring[2000];	// input buffer

	while(!feof(fileHandler))  {
		lineno ++;

		fgets(nextstring, 1024, fileHandler);
		
		if(lineno == 1) continue;	// skip csv file description
		
		

		char *strCpy;
		char **split;
		int num;
		strCpy = (char*)malloc((strlen(nextstring)+1) * sizeof(*nextstring));
		strCpy = strcpy(strCpy, nextstring);
		split = str_split(strCpy, ',', &num);
		
		/*
		if(split == NULL) {
			printf("str_split return NULL\n");
		}else {
			//printf("%dth line has %d columns\n",lineno, num);
		}
		*/

		if(num < 19) {	// standard RADIUS format
			break;
		}
		
		int timeStamp;
		string etherAddr;
		string status;
		string sessionId;
		string ipAddr;

		timeStamp = ConvertStrToTimeStamp(split[1], 4); // Jau 4
		etherAddr = split[4];
		status = split[5];
		sessionId = split[6];
		ipAddr = split[14];

		mac_addr.insert(etherAddr);

		if(status == "Start")  {
			if(connectionLog.count(sessionId) == 1) continue; // discard duplicate sessionId

			Connection con;
			con.etherAddr = etherAddr;
			con.startTime = timeStamp;
			con.endTime = 0;
			con.ipAddr = ipAddr;
			connectionLog[sessionId] = con;
		
		}  else if(status == "Stop")  {
			if(connectionLog.count(sessionId) == 1)  {	// found corresponding start packet
				
				if(ipAddr.size() < 2) continue; // invalid ip address. discard this session

				Session ses;
				ses.etherAddr = etherAddr;
				ses.startTime = connectionLog[sessionId].startTime;
				ses.endTime   = timeStamp;
				connectionLog.erase(connectionLog.find(sessionId));

				if(IPLog.count(ipAddr) == 0)  {	
					vector<Session> tmp;
					IPLog[ipAddr] = tmp;
				}

				IPLog[ipAddr].push_back(ses);
			}  else  {	// corresponding start session pakcet is lost
				if(ipAddr.size() < 2) continue; // invalid ip address

				Session ses;
				ses.etherAddr = etherAddr;
				ses.endTime = timeStamp;

				if(IPLog.count(ipAddr) == 0)  {	// no session related to ipAddr yet
					vector<Session> tmp;
					IPLog[ipAddr] = tmp;
					ses.startTime = FIRSTTIMESTAMP;	// set start time to 00:00:00
					IPLog[ipAddr].push_back(ses);
				}  else  {
					ses.startTime = IPLog[ipAddr].back().endTime;
					IPLog[ipAddr].push_back(ses);
				}
			}
		}

		free(strCpy);
	}

	// handle those item without stop status
	// do information repairing at the same time

	for(map<string, Connection>::iterator it_map = connectionLog.begin();
		it_map != connectionLog.end(); it_map ++)  {
		Connection con = it_map->second;

		if(con.ipAddr.size() < 2 || con.etherAddr.size() < 2) continue;	

		Session ses;
		ses.etherAddr = con.etherAddr;
		ses.startTime = con.startTime;

		if(IPLog.count(con.ipAddr) == 0)  {	// no client is assigned to this ip address
			ses.endTime = LASTTIMESTAMP;
			vector<Session> tmp;
			tmp.push_back(ses);
			IPLog[con.ipAddr] = tmp;
			continue;
		}

		int endTime = LASTTIMESTAMP;
		
		for(vector<Session>::iterator it_vec = IPLog[con.ipAddr].begin(); it_vec != IPLog[con.ipAddr].end(); it_vec ++)  {
			if(con.startTime <= it_vec->startTime)  {
				endTime = it_vec->startTime;
				break;
			}
		}
		ses.endTime = endTime;
		IPLog[con.ipAddr].push_back(ses);
	}

	fclose(fileHandler);
	printf("Exist %d different mac address\n", mac_addr.size());
}

void StoreSession(const char *filename, const map<string, vector<Session> >& IPLog)  {
	FILE *writeFileHandle = fopen(filename, "w");

	for(map<string, vector<Session> >::const_iterator it_map = IPLog.begin();
		it_map != IPLog.end(); it_map ++)  {

		int size = it_map->second.size();
		fprintf(writeFileHandle, "%s %d\n", it_map->first.c_str(), size);
		string etherAddr;
		int startTime, endTime;
		for(vector<Session>::const_iterator it_vec = it_map->second.begin(); 
			it_vec != it_map->second.end(); it_vec ++)  {
			etherAddr = it_vec->etherAddr;
			startTime = it_vec->startTime;
			endTime   = it_vec->endTime;
			fprintf(writeFileHandle, "%s %d %d\n", etherAddr.c_str(), startTime, endTime);
		}
	}
	fclose(writeFileHandle);
}