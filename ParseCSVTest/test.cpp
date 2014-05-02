#include "LoadData.h"
#include <stdio.h>
#include <iostream>
using namespace std;

int main()  {
	char inputname[100] = "/home/sangs/RE/UnitTest/DispatchTrafficTest/RADIUS Accounting 2013-01-04_anonymous.csv";
	char outputname[100] = "/home/sangs/RE/UnitTest/ParseCSVTest/IPInfo.txt";

	map<string, vector<Session> > record;

	LoadSession(inputname, record);
	StoreSession(outputname, record);

	return 0;
}