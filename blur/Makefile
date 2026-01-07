# Author: David Holmqvist <daae19@student.bth.se>

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pthread

COMMON_OBJS = matrix.o ppm.o
SEQ_OBJS = filters.o $(COMMON_OBJS)
PAR_OBJS = filters_par.o filters.o $(COMMON_OBJS)  

all: blur blur_par

# Sekventiell version
blur: blur.cpp $(SEQ_OBJS)
	$(CXX) $(CXXFLAGS) blur.cpp $(SEQ_OBJS) -o blur

# Parallell version
blur_par: blur_par.cpp $(PAR_OBJS)
	$(CXX) $(CXXFLAGS) blur_par.cpp $(PAR_OBJS) -o blur_par

filters.o: filters.cpp filters.hpp
	$(CXX) $(CXXFLAGS) -c filters.cpp -o filters.o

filters_par.o: filters_par.cpp filters.hpp
	$(CXX) $(CXXFLAGS) -c filters_par.cpp -o filters_par.o

matrix.o: matrix.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c matrix.cpp -o matrix.o

ppm.o: ppm.cpp ppm.hpp
	$(CXX) $(CXXFLAGS) -c ppm.cpp -o ppm.o

clean:
	rm -f blur blur_par *.o *.ppm 2>/dev/null
