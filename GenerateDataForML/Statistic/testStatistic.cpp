#include "Statistic.h"

using namespace std;

int main()  {
	Statistic statistic;

	statistic.setUDPTraffic(11);

	statistic.WriteToFile("/re/unitTest/GenerateDataForML/aaa", "aaa");
	return 0;
}