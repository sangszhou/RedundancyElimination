#include "FIFOCache.h"
#include <iostream>

using namespace std;

int main()  {

	FIFOCache fifoCache(4);

	cout << fifoCache.HasKey(1) << endl;
	fifoCache.AddKey(1);
	cout << fifoCache.HasKey(1) << endl;

	fifoCache.AddKey(2);
	fifoCache.AddKey(3);
	fifoCache.AddKey(4);

	cout << fifoCache.HasKey(2) << endl;
	cout << fifoCache.HasKey(3) << endl;
	cout << fifoCache.HasKey(4) << endl;
	cout << fifoCache.HasKey(1) << endl;
	
	fifoCache.AddKey(5);
	fifoCache.AddKey(6);
	fifoCache.AddKey(7);
	fifoCache.AddKey(1);
	fifoCache.AddKey(8);
	fifoCache.AddKey(4);

	cout << fifoCache.HasKey(2) << endl;
	cout << fifoCache.HasKey(3) << endl;
	cout << fifoCache.HasKey(4) << endl;
	cout << fifoCache.HasKey(1) << endl;
	
	return 0;
}