test : MyFileSystem.o DispatchTraffic.o StartProgram.o
	g++    -o test MyFileSystem.o DispatchTraffic.o StartProgram.o \
	-L/usr/local/boost_1_55_0/stage/lib/ -lboost_regex -lboost_system -lboost_filesystem

StartProgram.o : DispatchTraffic.h StartProgram.cpp
	g++   -c StartProgram.cpp

DispatchTraffic.o : DispatchTraffic.h DispatchTraffic.cpp MyFileSystem.h  Headers.h
	g++   -c DispatchTraffic.cpp 

MyFileSystem.o : Session.h MyFileSystem.h MyFileSystem.cpp
	g++   -c MyFileSystem.cpp 

clean :
	rm test StartProgram.o DispatchTraffic.o MyFileSystem.o
