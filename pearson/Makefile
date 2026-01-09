# Author: David Holmqvist <daae19@student.bth.se>

CXX=g++-13
CXXFLAGS=-std=c++17 -g -Wunused -Wall -Wunused

all: pearson pearson_par verify

pearson: vector dataset analysis pearson.cpp 
	$(CXX) $(CXXFLAGS) pearson.cpp vector.o dataset.o analysis.o -o pearson

analysis: vector analysis.hpp analysis.cpp
	$(CXX) $(CXXFLAGS) -c analysis.cpp -o analysis.o

dataset: vector dataset.hpp dataset.cpp
	$(CXX) $(CXXFLAGS) -c dataset.cpp -o dataset.o

vector: vector.hpp vector.cpp
	$(CXX) $(CXXFLAGS) -c vector.cpp -o vector.o

verify: verify.c
	$(CC) verify.c -o verify

pearson_par: pearson_par.o analysis_par.o dataset.o vector.o
	$(CXX) $(CXXFLAGS) pearson_par.o analysis_par.o dataset.o vector.o -o pearson_par

pearson_par.o: pearson_par.cpp analysis_par.hpp dataset.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c pearson_par.cpp

analysis_par.o: analysis_par.cpp analysis_par.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c analysis_par.cpp

clean:
	rm -rf pearson pearson_par verify *.o *.dSYM 2> /dev/null
