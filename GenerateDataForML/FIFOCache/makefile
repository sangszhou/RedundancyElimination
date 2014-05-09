
test: startProgram.o FIFOCache.o CircularQueue.o 
	g++ -std=c++11 -g -o test FIFOCache.o CircularQueue.o startProgram.o

startProgram.o: startProgram.cpp FIFOCache.h
	g++ -std=c++11 -g -c startProgram.cpp

FIFOCache.o: FIFOCache.cpp FIFOCache.h CircularQueue.h
	g++ -std=c++11 -g -c FIFOCache.cpp

CircularQueue.o: CircularQueue.cpp CircularQueue.h
	g++ -std=c++11 -g -c CircularQueue.cpp

clean:
	rm *.o test