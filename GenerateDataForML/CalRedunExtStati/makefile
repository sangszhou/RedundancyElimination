
test: Statistic.o ChooseMission.o bloom.o PcapParser.o CalRedunExtStati.o Murmurhash2.o
	g++ -pthread -o  test Statistic.o ChooseMission.o bloom.o PcapParser.o CalRedunExtStati.o Murmurhash2.o \
	-L/usr/local/boost_1_55_0/stage/lib/ -lboost_system -lboost_filesystem 

CalRedunExtStati.o: CalRedunExtStati.cpp CalRedunExtStati.h ../Statistic/Statistic.h ../Utility/MyFileSystem.h \
	../BloomFilter/bloom.h ../Utility/PcapParser.h
	g++ -g -c CalRedunExtStati.cpp

Statistic.o: ../Statistic/Statistic.cpp  ../Statistic/Statistic.h
	g++ -g -c ../Statistic/Statistic.cpp

#MyFileSystem.o: ../Utility/MyFileSystem.cpp ../Utility/MyFileSystem.h
#	g++ -g -c ../Utility/MyFileSystem.cpp

ChooseMission.o: ../Utility/ChooseMission.cpp ../Utility/ChooseMission.h
	g++ -std=c++11 -g -c ../Utility/ChooseMission.cpp

Murmurhash2.o: ../BloomFilter/Murmurhash2.cpp ../BloomFilter/Murmurhash2.h 
	g++ -g -c ../BloomFilter/Murmurhash2.cpp

bloom.o: ../BloomFilter/bloom.cpp ../BloomFilter/bloom.h ../BloomFilter/Murmurhash2.h
	g++ -g -c ../BloomFilter/bloom.cpp


PcapParser.o: ../Utility/PcapParser.cpp ../Utility/PcapParser.h ../Utility/headers.h
	g++ -g -c ../Utility/PcapParser.cpp

clean:
	rm *.o test