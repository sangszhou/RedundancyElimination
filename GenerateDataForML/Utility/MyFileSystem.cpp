#include <boost/filesystem.hpp>
#include "MyFileSystem.h"

using namespace std;
using namespace boost::filesystem;

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





