
#TARGETS = CalRedundancy/test

#all: $(TARGETS)

CalRedundancy/test: handleMultifiles.o MyFileSystem.o bloom.o Murmurhash2.o
	g++ -o CalRedundancy/test handleMultifiles.o MyFileSystem.o bloom.o Murmurhash2.o \
	-L/usr/local/boost_1_55_0/stage/lib/ -lboost_system -lboost_filesystem 

handleMultifiles.o: CalRedundancy/handleMultifiles.cpp BloomFilter/bloom.h MyFileSystem/MyFileSystem.h
	g++ -c CalRedundancy/handleMultifiles.cpp

MyFileSystem.o: MyFileSystem/MyFileSystem.cpp MyFileSystem/MyFileSystem.h
	g++ -g -c MyFileSystem/MyFileSystem.cpp

bloom.o: BloomFilter/bloom.cpp BloomFilter/bloom.h BloomFilter/Murmurhash2.h
	g++ -g -c BloomFilter/bloom.cpp

Murmurhash2.o: BloomFilter/Murmurhash2.cpp BloomFilter/Murmurhash2.h
	g++ -g -c BloomFilter/Murmurhash2.cpp

clean:
	rm -f *.o CalRedundancy/test