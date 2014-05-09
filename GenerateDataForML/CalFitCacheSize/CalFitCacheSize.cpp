//#include <gmpxx.h>

#include "CalFitCacheSize.h"
#include "../FIFOCache/FIFOCache.h"
#include "../Utility/headers.h"
#include "../Utility/PcapParser.h"
#include "../Utility/ChooseMission.h"
#include "../BloomFilter/Murmurhash2.h"

using namespace std;

void HandleAMission(const char *srcFileName, FILE *output, int blockSize, int samplingRate, int cacheSize, double &redun)  {
	
	const int MAGICNUM = 33;	// hash for more, refer to 
	const int MAXINTEGER = 4294967295; // biggest unsigned int

	struct IP_hdr *ip_hdr;
	struct TCP_hdr *tcp_hdr;
	struct UDP_hdr *udp_hdr;

	char *payLoad;
	int proto;	// udp for 0, tcp for 1
	int payLoadLen;	

	// init pcapparser	
	
	PcapParser pcaparser(srcFileName);	

	// init FIFOCache
	
	FIFOCache fifocache(cacheSize);

	// statistic
	unsigned long totalByte = 0;
	unsigned long savingByte = 0;

	unsigned int localMax;
	unsigned int candiMax;

	unsigned int seed = 0x11223344; 

	int pindex = 0;

	int testBigHead = 0;

	while(pcaparser.NextPacket(&ip_hdr, &tcp_hdr, &udp_hdr, proto, &payLoad, payLoadLen) == 0)  {
		pindex ++;

		if(payLoadLen < 128) continue;

		totalByte += payLoadLen;

		for(int cursor = 1; cursor+samplingRate < payLoadLen; cursor += samplingRate)  {
			localMax = murmurhash2((void *)(payLoad+cursor), blockSize, seed);
			/*
			for(int offset = cursor+1; offset < cursor + samplingRate; offset ++)  {
				candiMax = murmurhash2((void*)(payLoad+offset), blockSize, seed);

				if(candiMax > localMax)  {
					localMax = candiMax;
				}
			}
			*/

			//printf("%u:", localMax);
			//printf("%.64s\n", payLoad+cursor);
			if(fifocache.HasKey(localMax))  {
				savingByte += blockSize;

				if(payLoadLen >= 500)  {
					testBigHead += blockSize;
				}

			}  else  {
				fifocache.AddKey(localMax);
			}

		}

	}

	//printf("Big head is %d\n", testBigHead);

	//printf("Saving byte is %ld byte\n", savingByte);
	//printf("Total byte is %ld\n", totalByte);
	redun = 1.0*savingByte/totalByte;
	printf("Redundancy rate is %lf\n", 1.0*savingByte/totalByte);
}




void DispatchMission(const char *fileName)  {
	vector<string> missionList;
	LoadMission(missionList, fileName);

	FILE *fp = fopen("/re/sampleData/output/fitCacheSize.txt", "w");

	double oriRedun;
	double candiRedun;

	int minSize, maxSize, midSize;

	for(int i = 0; i < missionList.size(); i ++)  {

		// start test
		if(i == 3)
			break;
		// end test

		minSize = 3000, maxSize = 2000000;	// min 210 KB, max 140MB

		HandleAMission(missionList[i].c_str(), fp, 64, 128, maxSize, oriRedun);
		candiRedun = oriRedun;
		
		while(minSize < maxSize)  {
			midSize = (minSize+maxSize) >> 1;

			HandleAMission(missionList[i].c_str(), fp, 64, 128, midSize, candiRedun);

			if(oriRedun - candiRedun < 0.01)  {
				maxSize = midSize - 10;
				//printf("Decrese cache size\n");
			}  else  {
				minSize = midSize + 10;
				//printf("Increse cache size\n");
			}
		}
		printf("For %dth, optimal Redundancy rate is %lf and fit Redundancy is %lf and fit cache size is %d\n", i, oriRedun, candiRedun, minSize);
		fprintf(fp, "%d %lf\n", minSize, candiRedun);
	}
	
	fclose(fp);
}

/*
void HandleGroupMission(const vector<string> &missionList, int threads, int cacheSize, const char *dstFileName)  {
	FILE *fp = fopen(dstFileName, "w");

	for(int i = 0; i < missionList.size(); i ++)  {
		if(i % threads == 0)  {
			HandleAMission(missionList[i].c_str(), fp, 64, 128, cacheSize);
		}
	}

	fclose(fp);
}
*/

int main()  {
	
	
	for(int i = 0; i < 1; i ++)  {
		DispatchMission("/re/sampleData/missionList.txt");
		//cacheSize = 10;
		//printf("Current Cache Size is %d\n", cacheSize);
	}
	

	return 0;
}