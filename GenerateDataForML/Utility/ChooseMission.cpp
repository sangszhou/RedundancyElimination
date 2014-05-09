#include "ChooseMission.h"
#include <boost/filesystem.hpp>
#include <stdio.h>

using namespace std;
using namespace boost::filesystem;



void FilterMission(vector<string> &missionList, const char *dirPath)  {

	const unsigned long MINFILESIZE = 20971520; 	// 20M
	const unsigned long MAXFILESIZE = 1610612736;	// 1.5G 

	unsigned long totalByte = 0;

	path p(dirPath);

	try {
		if(exists(p))  {
			if(is_regular_file(p))  {
				printf("User log file has been found\n");
			}  else if(is_directory(p))  {
				printf("%s is directory\n", dirPath);
			}  else  {
				printf("Error, %s is not a file\n", dirPath);
			}

		}  else  {
			printf("%s does not exist\n", dirPath);
		}
	}  catch(const filesystem_error &ex)  {
		printf("Open %s error\n", dirPath);
	}

	vector<path> tmp;
	copy(directory_iterator(p), directory_iterator(), back_inserter(tmp));

	for(vector<path>::iterator it_vec = tmp.begin(); it_vec != tmp.end(); it_vec ++)  {
		if(is_regular_file(*it_vec) && (file_size(*it_vec) >= MINFILESIZE) && (file_size(*it_vec) <= MAXFILESIZE))  {
			totalByte += file_size(*it_vec);
			missionList.push_back(it_vec->string());
		}
	}
	
	cout << totalByte/(1024*1024*1024) << endl;
}

void SaveMission(const vector<string> &missionList, const char *filePath)  {
	FILE *fp = fopen(filePath, "w");

	for(int i = 0; i < missionList.size(); i ++)  {
		fprintf(fp, "%s\n", missionList[i].c_str());
	}
	fclose(fp);
}

void LoadMission(vector<string> &missionList, const char *filePath)  {

	char temp[200];

	FILE *fp = fopen(filePath, "r");

	while(feof(fp) == 0)  {
		fscanf(fp, "%s", temp);
		missionList.push_back(temp);
	}

	fclose(fp);
}


/*

int main()  {

	vector<string> missionList;

	char dirPath[200] = "/media/sangs/ab2e6e33-9eee-4602-9a78-56aadd4b50c1/LeftOver";
	char outputName[200] = "/re/sampleData/missionList.txt";

	FilterMission(missionList, dirPath);
	SaveMission(missionList, outputName);

	vector<string> readMissionList;
	LoadMission(readMissionList, outputName);

	for(int i = 0; i < 3; i ++)  {
		cout << readMissionList[i] << endl;
	}

	
	return 0;
}
*/