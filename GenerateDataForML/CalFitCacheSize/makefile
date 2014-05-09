test:	ChooseMission.o FIFOCache.o CircularQueue.o  PcapParser.o CalFitCacheSize.o Murmurhash2.o
	g++ -std=c++11 -g -o test ChooseMission.o FIFOCache.o CircularQueue.o PcapParser.o CalFitCacheSize.o Murmurhash2.o \
	-L/usr/local/boost_1_55_0/stage/lib/ -lboost_system -lboost_filesystem 

CalFitCacheSize.o: CalFitCacheSize.cpp CalFitCacheSize.h ../Utility/ChooseMission.h ../Utility/PcapParser.h ../Utility/headers.h \
	../FIFOCache/FIFOCache.h 
	g++ -std=c++11 -g -c CalFitCacheSize.cpp

ChooseMission.o: ../Utility/ChooseMission.cpp ../Utility/ChooseMission.h
	g++ -std=c++11 -g -c ../Utility/ChooseMission.cpp

FIFOCache.o: ../FIFOCache/FIFOCache.cpp ../FIFOCache/FIFOCache.h ../FIFOCache/CircularQueue.h
	g++ -std=c++11 -g -c ../FIFOCache/FIFOCache.cpp

Murmurhash2.o: ../BloomFilter/Murmurhash2.cpp ../BloomFilter/Murmurhash2.h
	g++ -std=c++11 -g -c ../BloomFilter/Murmurhash2.cpp

CircularQueue.o: ../FIFOCache/CircularQueue.cpp ../FIFOCache/CircularQueue.h
	g++ -std=c++11 -g -c ../FIFOCache/CircularQueue.cpp 

PcapParser.o: ../Utility/PcapParser.cpp ../Utility/PcapParser.h ../Utility/headers.h
	g++ -std=c++11 -g -c ../Utility/PcapParser.cpp

clean:
	rm *.o test