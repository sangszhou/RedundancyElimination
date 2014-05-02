/*
 * StartProgram.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: sangs
 */


#include "DispatchTraffic.h"
using namespace std;

int main(int argc, char *argv[])  {
	/*
	if(argc < 4 || argc > 4)  {
		printf("Invalid argument number!\n No whitespace is allowed in directory or file names\n");
		return 0;
	}

	printf("Source folder %s to Destination folder %s\n, user log file is %s\n", argv[1], argv[2], argv[3]);
	namespace fs = boost::filesystem;

	if(!fs::exists(argv[1]) || fs::is_directory(argv[1])) {
			printf("Error! Source directory doesn't exist\n");
			return 0;
	}
	if(!fs::exists(argv[2]))  {
		printf("Destination directory doesn't exist\n");
		printf("Building new directory for you...\n");
	}
	if(!fs::exists(argv[3]))  {
		printf("User log doesn't exist\n");
		return 0;
	}

	printf("Start dispatching files? Enter 1 to start, else to abort\n");
	int command;
	scanf("%d", &command);
	if(command != 1)  {
		return 0;
	}
	*/

	char filename1[100] = "/home/sangs/RE/UnitTest/DispatchTrafficTest/DataSet/Input";	// input directory
	char filename2[100] = "/home/sangs/RE/UnitTest/DispatchTrafficTest/DataSet/Output";	// output direcotry
	char filename3[100] = "/home/sangs/RE/UnitTest/ParseCSVTest/IPInfo.txt";			// csv file
	//choosePcapFile(argv[1], argv[2], argv[3]);
	ChoosePcapFile(filename1, filename2, filename3);

	return 0;
}

