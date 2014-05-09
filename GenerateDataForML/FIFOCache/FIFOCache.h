#include <unordered_set>

#include "CircularQueue.h"

using namespace std;

class FIFOCache {
public:
	FIFOCache(unsigned int maxSize = 10);

	bool HasKey(const unsigned int key);
	
	void AddKey(const unsigned int key);
	
private:
	CircularQueue queue;
	unordered_set<unsigned int> record;
};