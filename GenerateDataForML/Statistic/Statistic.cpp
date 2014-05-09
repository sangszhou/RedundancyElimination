#include "Statistic.h"

using namespace std;

Statistic::Statistic()  {
	attributeNum = 24;
	
	for(int i = 0; i < attributeNum; i ++)  {
		attributes[i] = 0.0;
	}

}


void Statistic::setConnectionTimes(double traffic)  {
	attributes[0] += traffic;
}
void Statistic::setConnectionDuration(double traffic)  {
	attributes[1] += traffic;
}


void Statistic::setUpLoadTraffic(double traffic)  {
	attributes[2] += traffic;
}
void Statistic::setDownLoadTraffic(double traffic)  {
	attributes[3] += traffic;
}

void Statistic::setUpLoadRedun(double traffic)  {
	attributes[4] += traffic;
}
void Statistic::setDownLoadRedun(double traffic)  {
	attributes[5] += traffic;
}

void Statistic::setUDPTraffic(double traffic)  {
	attributes[6] += traffic;
}	
void Statistic::setTCPTraffic(double traffic)  {
	attributes[7] += traffic;
}

void Statistic::setUDPRedun(double traffic)  {
	attributes[8] += traffic;
}	
void Statistic::setTCPRedun(double traffic)  {
	attributes[9] += traffic;
}

void Statistic::setPortDNSTraffic(double traffic)  {		// port 53
	attributes[10] += traffic;
}		
void Statistic::setPortHTTPTraffic(double traffic)  {	// port 80, 8000, 8080
	attributes[11] += traffic;
}		
void Statistic::setPortRTSPTraffic(double traffic)  {	// port 554, 7070
	attributes[12] += traffic;
}		
void Statistic::setPortHTTPSTraffic(double traffic)  {	// port 443
	attributes[13] += traffic;
}	
void Statistic::setPortFTPTraffic(double traffic)  {		// port 20
	attributes[14] += traffic;
}

void Statistic::setPortDNSRedun(double traffic)  {		// port 53
	attributes[15] += traffic;
}		
void Statistic::setPortHTTPRedun(double traffic)  {	// port 80, 8000, 8080
	attributes[16] += traffic;
}		
void Statistic::setPortRTSPRedun(double traffic)  {	// port 554, 7070
	attributes[17] += traffic;
}		
void Statistic::setPortHTTPSRedun(double traffic)  {	// port 443
	attributes[18] += traffic;
}	
void Statistic::setPortFTPRedun(double traffic)  {		// port 20
	attributes[19] += traffic;
}

void Statistic::setSampleNum(double num)  {
	attributes[20] += num;
}
void Statistic::setHitNum(double num)  {
	attributes[21] += num;
}
void Statistic::setIncreHitNum(double num)  {
	attributes[22] += num;
}

void Statistic::setTotalSize(double num)  {
	attributes[23] += num;
}

void Statistic::WriteToFile(FILE *fp)  {
	
	for(int i = 0; i < attributeNum; i ++)  {
		fprintf(fp, "%lf ", attributes[i]);
	}
	fprintf(fp, "\n");
}
