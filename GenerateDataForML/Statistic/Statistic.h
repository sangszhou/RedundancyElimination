#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class Statistic  {
public:

	Statistic();

	void setConnectionTimes(double);
	void setConnectionDuration(double);

	void setUpLoadTraffic(double);
	void setDownLoadTraffic(double);
	void setUpLoadRedun(double);
	void setDownLoadRedun(double);
	
	void setUDPTraffic(double);	
	void setTCPTraffic(double);
	void setUDPRedun(double);	
	void setTCPRedun(double);
	

	void setPortDNSTraffic(double);		// port 53
	void setPortHTTPTraffic(double);		// port 80, 8000, 8080
	void setPortRTSPTraffic(double);		// port 554, 7070
	void setPortHTTPSTraffic(double);	// port 443
	void setPortFTPTraffic(double);		// port 20

	void setPortDNSRedun(double);		// port 53
	void setPortHTTPRedun(double);		// port 80, 8000, 8080
	void setPortRTSPRedun(double);		// port 554, 7070
	void setPortHTTPSRedun(double);		// port 443
	void setPortFTPRedun(double);		// port 20


	void setSampleNum(double);
	void setHitNum(double);
	void setIncreHitNum(double);

	void setTotalSize(double);
	// write above attributes as well as client id (mac address) 
	// to disk which will be used as machine learning model
	// input in the future

	void WriteToFile(FILE *fp);	 

private:
	double attributes[100];
	int attributeNum;
};

#endif