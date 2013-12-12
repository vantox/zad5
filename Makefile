CXX=g++
CXXFLAGS= -Wall -O2 -std=c++11

all: test

test: virus_genealogy.h test.cc
	$(CXX) $(CXXFLAGS) test.cc -o test

clean:
	rm test