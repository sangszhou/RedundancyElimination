test: handleMultifiles.o Murmurhash2.o bloom.o  MyFileSystem.o
	g++ -g -o test Murmurhash2.o bloom.o handleMultifiles.o  MyFileSystem.o \
	-L/usr/local/boost_1_55_0/stage/lib/ -lboost_system -lboost_filesystem 


handleMultifiles.o: handleMultifiles.cpp bloom.h MyFileSystem.h headers.h
	g++ -g -c handleMultifiles.cpp

MyFileSystem.o: MyFileSystem.cpp MyFileSystem.h 
	g++ -g -c MyFileSystem.cpp

bloom.o: bloom.cpp bloom.h Murmurhash2.h
	g++ -g -c bloom.cpp

Murmurhash2.o: Murmurhash2.cpp Murmurhash2.h
	g++ -g -c Murmurhash2.cpp

clean:
	rm *.o test