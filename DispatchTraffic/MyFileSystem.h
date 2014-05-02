/*
 * MyFileSystem.h
 *
 *  Created on: Apr 28, 2014
 *      Author: sangs
 */

#ifndef MYFILESYSTEM_H_
#define MYFILESYSTEM_H_

#include "Session.h"
#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

void getFileNames(const char *dirName, vector<string> &fileNameList);

void getUserLog(const char *filename, map<string, vector<Session> > &userLog);


#endif /* MYFILESYSTEM_H_ */
