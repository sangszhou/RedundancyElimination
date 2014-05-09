#include <vector>
#include <string>
#include <stdio.h>
#include <cstring>
#include <boost/filesystem.hpp>
#include <pthread.h>

#include "CalRedunExtStati.h"
#include "../Statistic/Statistic.h"
#include "../Utility/headers.h"

#include "../Utility/ChooseMission.h"
#include "../Utility/PcapParser.h"
#include "../BloomFilter/bloom.h"

const int NUM_THREAD = 2;

using namespace std;
using namespace boost::filesystem;


class ThreadArgu  {
public:
	ThreadArgu(const vector<string> &_nameList, int _index, char *_dstFileName)  {

		for(vector<string>::const_iterator cit_vec = _nameList.begin(); cit_vec != _nameList.end(); cit_vec ++)  {
			nameList.push_back(*cit_vec);
		}

		index = _index;
		dstFileName = _dstFileName;
	} 

	ThreadArgu(const ThreadArgu &rhs)  {

		for(vector<string>::const_iterator cit_vec = rhs.nameList.begin(); cit_vec != rhs.nameList.end(); cit_vec ++)  {
			nameList.push_back(*cit_vec);
		}

		index = rhs.index;
		dstFileName = rhs.dstFileName;
	}

	vector<string> nameList;
	int index;
	char *dstFileName;
};


void ServeAClient(const char *srcFileName, FILE *output, int blockSize, int samplingRate)  {
	struct IP_hdr *ip_hdr;
	struct TCP_hdr *tcp_hdr;
	struct UDP_hdr *udp_hdr;

	char *payload;
	char src_ip_char[STRSIZE], dst_ip_char[STRSIZE];

	int proto;	// udp for 0, tcp for 1
	int payLoadLen;	

	PcapParser pcaparser(srcFileName);	// init pcapparser
	Statistic statistic;				// 
	bool upload = false;

	char *localMax;
	char *candiMax;

	// init bloom filter

	unsigned int fileSize = file_size(srcFileName);
	printf("File size is %d bytes\n", fileSize);
	fileSize /= samplingRate;	// items to be added into bloom filter
	bloom cache;
	bloom_init(&cache, fileSize, 0.01);	// set false positive rate to 1%


	// port num
	
	int srcPort, dstPort;

	// iterate packets in srcFileName
	// could exceeds million packets in a srcFileName 

	while(pcaparser.NextPacket(&ip_hdr, &tcp_hdr, &udp_hdr, proto, &payload, payLoadLen) == 0)  {
		
		// 'o' equals 111, 111 guarantee it is a dynamic allocated ip address

		// tcp or udp traffic
		if(payLoadLen < 128) continue;

		if(proto == 0)  {				
			statistic.setUDPTraffic(payLoadLen);
			srcPort = ntohs(udp_hdr->uh_sport);
			dstPort = ntohs(udp_hdr->uh_dport);

		}  else if(proto == 1) {
			statistic.setTCPTraffic(payLoadLen);
			srcPort = ntohs(tcp_hdr->th_sport);
			dstPort = ntohs(tcp_hdr->th_dport);
		}  else  {
			continue;
		}

		statistic.setTotalSize(payLoadLen);

		if(*(u_char*)(&ip_hdr->ip_src) == 'o')  {	// char '0' equals int 111 which guarantee its dynamic allocated ip address
			statistic.setUpLoadTraffic(payLoadLen);
			upload = true;
		}  else  {
			statistic.setDownLoadTraffic(payLoadLen);
			upload = false;
		}

		// port traffic

		if(80 == srcPort || 80 == dstPort || 8000 == srcPort || 8000 == dstPort || 8080 == srcPort || 8080 == dstPort)  {
			statistic.setPortHTTPTraffic(payLoadLen);
		}  else if(554 == srcPort || 554 == dstPort || 70 == srcPort || 70 == dstPort)  {
			statistic.setPortRTSPTraffic(payLoadLen);
		}  else if(443 == srcPort || 443 == dstPort)  {
			statistic.setPortHTTPSTraffic(payLoadLen);
		}  else if(20 == srcPort || 20 == dstPort)  {
			statistic.setPortFTPTraffic(payLoadLen);
		}  else if(53 == srcPort || 53 == dstPort)  {
			statistic.setPortDNSTraffic(payLoadLen);
		}  else  {
			// other port
		}

		// which port


		// iterator payload and get samples

		for(int cursor = 0; cursor + blockSize < payLoadLen; cursor += samplingRate)  {
			localMax = (payload + cursor);

			// MAXP algorithm, without below code it is MODP algorithm
			// MAXP algorithm, begins...
			/*
			for(int offset = cursor+1; offset < samplingRate+cursor; offset ++)  {
				candiMax = (payload + offset);
				if(strncmp(localMax, candiMax, blockSize) < 0)  {
					localMax = candiMax;
				}
			}
			*/
			// MAXP algorithm, ends...

			statistic.setSampleNum(blockSize);	// one more sample
			// check if localMax has already in cache

			if(bloom_check(&cache, localMax, blockSize) == 1)  {	// hit 
				statistic.setHitNum(blockSize);

				if(80 == srcPort || 80 == dstPort || 8000 == srcPort || 8000 == dstPort || 8080 == srcPort || 8080 == dstPort)  {
					statistic.setPortHTTPRedun(blockSize);
				}  else if(554 == srcPort || 554 == dstPort || 70 == srcPort || 70 == dstPort)  {
					statistic.setPortRTSPRedun(blockSize);
				}  else if(443 == srcPort || 443 == dstPort)  {
					statistic.setPortHTTPSRedun(blockSize);
				}  else if(20 == srcPort || 20 == dstPort)  {
					statistic.setPortFTPRedun(blockSize);
				}  else if(53 == srcPort || 53 == dstPort)  {
					statistic.setPortDNSRedun(blockSize);
				}  else  {
					// other port
				}

				if(proto == 0)  {
					statistic.setUDPRedun(blockSize);
				}  else if(proto == 1)  {
					statistic.setTCPRedun(blockSize);
				}

				if(upload)  {
					statistic.setUpLoadRedun(blockSize);
				}  else  {
					statistic.setDownLoadRedun(blockSize);
				}


				// incremental hit rate
				// to verify the locality of redundancy

				for(int j = cursor+1; j < samplingRate+cursor; j ++)  {
                    candiMax = (char*)(payload+j);
                    if(bloom_check(&cache, candiMax, blockSize) == 1)  {
                        statistic.setIncreHitNum(blockSize);
                    }
                }

			}  else  {
				bloom_add(&cache, (void*)localMax, blockSize);
			}

		}

	}


	// extract mac address
	// 17 is aa-bb-cc-dd-ee-ff

	//int len = strlen(srcFileName);
	statistic.WriteToFile(output);
}



//void ServeGroupClient(const vector<string> &nameList, int index, const char *dstFileName)  {
void* ServeGroupClient(void* threadArgu)  {
	ThreadArgu *ta = (ThreadArgu*)threadArgu;

	vector<string> nameList = ta -> nameList;
	int index = ta->index;
	char *dstFileName = ta ->dstFileName;

	FILE *output = fopen(dstFileName, "w");
	//FILE *output2 = fopen("/re/sampleData/output/statis1.txt", "w");	

	for(int i = 0; i < nameList.size(); i ++)  {
		if(i%NUM_THREAD == index)  {
			ServeAClient(nameList[i].c_str(), output, 64, 128);
		}
	}

	fclose(output);
}



void DispatchClient(const char *srcFileName, const char *dstFileName)  {
	vector<string> nameList;
	LoadMission(nameList, srcFileName);

	pthread_t threads[NUM_THREAD];

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int rc1, rc2;
	int ret1, ret2;

	char dstFileName1[100] = "/re/sampleData/output/statis0.txt";
	char dstFileName2[100] = "/re/sampleData/output/statis1.txt";

	ThreadArgu arg1(nameList, 0, dstFileName1);
	ThreadArgu arg2(nameList, 1, dstFileName2);

	rc1 = pthread_create(&threads[0], &attr, ServeGroupClient, (void*)(&arg1));
	rc2 = pthread_create(&threads[1], &attr, ServeGroupClient, (void*)(&arg2));

	if(rc1 != 0 || rc2 != 0)  {
		cout << "Error in creating threads" << endl;
	}

	pthread_attr_destroy(&attr);

	void *status;

	ret1 = pthread_join(threads[0], &status);
	if(ret1 != 0)  {
		cout << "pthread join error 0" << endl;
	}

	ret2 = pthread_join(threads[1], &status);
	if(ret2 != 0)  {
		cout << "Pthread join error 1" << endl;
	}

	/*
	FILE *output = fopen(dstFileName, "a");
	for(int i = 0; i < nameList.size(); i ++)  {

		//if(i == 1) break;

		printf("Processing %dth file,  %s\n", i+1, nameList[i].c_str());
		ServeAClient(nameList[i].c_str(), output, 64, 128);
		printf("\n");
	}
	fclose(output);
	*/
}


int main()  {
	DispatchClient("/re/sampleData/missionList.txt", 	"/re/sampleData/output/out0.txt");

	return 0;
}