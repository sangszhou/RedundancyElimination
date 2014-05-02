test: LoadData.o test.o SplitRadius.o
	g++ -g -rdynamic -o test LoadData.o test.o SplitRadius.o

test.o: test.cpp LoadData.h 
	g++ -g -rdynamic -c test.cpp

LoadData.o: LoadData.cpp LoadData.h DataStruct.h
	g++ -g -rdynamic -c LoadData.cpp

SplitRadius.o: SplitRadius.cpp SplitRadius.h
	g++ -g -rdynamic -c SplitRadius.cpp

clean: 
	rm test test.o LoadData.o 