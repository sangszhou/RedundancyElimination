/*
 * MyFileSystem.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: sangs
 */

#include "MyFileSystem.h"

void getFileNames(const char *dirName, vector<string> &fileNameList)  {
	path p(dirName);

	try {
		if(exists(p))  {

			if(is_regular_file(p))  {
				printf("User log file has been found\n");
			}  else if(is_directory(p))  {
				printf("%s is directory\n", dirName);
			}  else  {
				printf("Error, %s is not a file\n", dirName);
			}

		}  else  {
			printf("%s does not exist\n", dirName);
		}
	}  catch(const filesystem_error &ex)  {
		printf("Open %s error\n", dirName);
	}

	vector<path> tmp;
	copy(directory_iterator(p), directory_iterator(), back_inserter(tmp));
	for(vector<path>::iterator it_vec = tmp.begin(); it_vec != tmp.end(); it_vec ++)  {
		if(is_regular_file(*it_vec))  {
			fileNameList.push_back(it_vec->string());
		}
	}
}


void getUserLog(const char *filename, map<string, vector<Session> > &userLog)  {
	path p(filename);

	try {
		if(exists(p))  {
			if(is_regular_file(p))  {
				printf("User log file has been found\n");
			}  else if(is_directory(p))  {
				printf("%s is directory\n", filename);
				return;
			}  else  {
				printf("Error, %s is not a file\n", filename);
				return;
			}
		}  else  {
			printf("%s does not exist\n", filename);
			return;
		}
	}  catch(const filesystem_error &ex)  {
		printf("Open %s error\n", filename);
		return;
	}

	freopen(filename, "r", stdin);

	string ip_addr;
	int jac_num;
	string etherAddr;
	int startTime, endTime;
	

	while(cin >> ip_addr)  {
		cin >> jac_num;
		vector<Session> tmp;
		for(size_t i = 0; i < jac_num; i ++)  {
			cin >> etherAddr >> startTime >> endTime;
			tmp.push_back(Session(startTime, endTime, etherAddr));
		}
		userLog[ip_addr] = tmp;
	}

	fclose(stdin);
}



