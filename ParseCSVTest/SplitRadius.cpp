#include "SplitRadius.h"
using namespace std;

char** str_split(char* str, char delim, int *numSplits) {
	char** ret;
	int retLen;
	char* c;

	if((str == NULL) || (delim == '\0')) {
		ret = NULL;
		retLen = -1;
		printf("Input is NULL or invalid delim\n");
	}else {
		retLen = 0;
		c = str;
		/*
		 * Pre-calculate number of elements
		 */
		do {
			if(*c == delim) {
				retLen ++;
			}
			c ++;
		} while(*c != '\0');
	}
	ret = (char**)malloc((retLen+1) * sizeof(*ret));
	ret[retLen] = NULL;

	c = str;
	retLen = 1;
	ret[0] = str;

	do {
		if(*c == delim) {
			ret[retLen++] = &c[1];
			*c = '\0';
		}
		c++;
	} while(*c != '\0');

	if(numSplits != NULL) {
		*numSplits = retLen;
	}else {
		printf("Invalid numSplits\n");
	}

	return ret;
}

int ConvertStrToTimeStamp(const char* str, const int _day)  {
	struct tm* timeinfo;
	time_t rawtime;
	int year, month, day;

	year = 2013;
	month = 1;
	day = _day;

	int hour = 0, mini = 0, sec = 0;

	int cursor = 0;
	while(str[cursor] != ':')  {
		hour = hour * 10 +(str[cursor] - '0');
		cursor ++;
	}
	cursor ++;
	while(str[cursor] != ':')  {
		mini = mini * 10 +(str[cursor] - '0');
		cursor ++;
	}
	cursor ++;
	while(str[cursor] != '\0')  {
		sec = sec * 10 + (str[cursor] - '0');
		cursor ++;
	}

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;

	timeinfo->tm_mday = day;
	timeinfo->tm_hour = hour;
	timeinfo->tm_min = mini;
	timeinfo->tm_sec = sec;

	time_t curTime = mktime(timeinfo);

	int res = (int)curTime;	// time_t is 8 byte in 64 bit machine
	//res += 3600 * 12;	// convert to beijing time
	return res;
}