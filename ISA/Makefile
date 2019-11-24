CXX=g++
CXXFLAGS= -std=c++14 -Wall -pedantic

all: isaclient isaserver

isaclient: isaclient.cpp
	$(CXX) $(CXXFLAGS) isaclient.cpp -o isaclient

isaserver: isaserver.cpp
	$(CXX) $(CXXFLAGS) isaserver.cpp -o isaserver

clean:
	rm -f isa*.o isaclient isaserver
