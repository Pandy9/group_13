# Author: David Holmqvist <daae19@student.bth.se>

CXX=g++-13
CXXFLAGS=-std=c++17 -Ofast -march=native -flto -DNDEBUG -Wall -Wextra

all: pearson pearson_par verify

pearson: pearson.o analysis.o dataset.o vector.o
	$(CXX) $(CXXFLAGS) pearson.o analysis.o dataset.o vector.o -o pearson

pearson.o: pearson.cpp analysis.hpp dataset.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c pearson.cpp

analysis.o: analysis.cpp analysis.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c analysis.cpp


pearson_par: pearson_par.o analysis_par.o dataset.o vector.o
	$(CXX) $(CXXFLAGS) pearson_par.o analysis_par.o dataset.o vector.o -o pearson_par

pearson_par.o: pearson_par.cpp analysis_par.hpp dataset.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c pearson_par.cpp

analysis_par.o: analysis_par.cpp analysis_par.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c analysis_par.cpp


dataset.o: dataset.cpp dataset.hpp vector.hpp
	$(CXX) $(CXXFLAGS) -c dataset.cpp

vector.o: vector.cpp vector.hpp
	$(CXX) $(CXXFLAGS) -c vector.cpp


verify: verify.c
	$(CC) $(CFLAGS) verify.c -o verify


clean:
	rm -rf pearson pearson_par verify *.o *.dSYM 2> /dev/null