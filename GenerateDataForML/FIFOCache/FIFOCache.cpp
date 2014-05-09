 #include "FIFOCache.h"

using namespace std;

// maxsize MB 
FIFOCache::FIFOCache(unsigned int MaxSize) :queue(MaxSize) {}

bool FIFOCache::HasKey(const unsigned int key)  {
	if(record.count(key) == 0)
		return false;
	return true;
}

void FIFOCache::AddKey(const unsigned int key)  {
	if(record.count(key) == 1) return;

	if(queue.IsFull())  {
		unsigned int x;
		queue.Delete(x);
		record.erase(record.find(x));
	} 

	queue.Add(key);
	record.insert(key);
}
