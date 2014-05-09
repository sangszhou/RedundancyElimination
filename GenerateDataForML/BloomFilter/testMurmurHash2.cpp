#include "Murmurhash2.h"
#include <iostream>

using namespace std;

int main()  {
	char str1[100] = "aabbcc";
	char str2[100] = "bbccdd";

	unsigned int seed = 100;

	//cout << murmurhash2((void*)(str1), 6, seed+100) << endl;

	unsigned int res = 0;

	for(int i = 0; i < 1000000; i ++)  {
		res = murmurhash2((void*)(str1), 6, seed+i);
		if(res < 100000)
			cout << res << endl;
	}


}