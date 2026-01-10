# Author: David Holmqvist <daae19@student.bth.se>

CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -pthread

all: blur blur_par

# ===== SEKVENTIELL =====
blur: blur.o filters.o matrix.o ppm.o
	$(CXX) $(CXXFLAGS) blur.o filters.o matrix.o ppm.o -o blur

filters.o: filters.cpp filters.hpp
	$(CXX) $(CXXFLAGS) -c filters.cpp -o filters.o

# ===== PARALLELL =====
blur_par: blur_par.o filters_par.o filters.o matrix.o ppm.o
	$(CXX) $(CXXFLAGS) blur_par.o filters_par.o filters.o matrix.o ppm.o -o blur_par

filters_par.o: filters_par.cpp filters.hpp
	$(CXX) $(CXXFLAGS) -c filters_par.cpp -o filters_par.o

# ===== GEMENSAMT =====
blur.o: blur.cpp
	$(CXX) $(CXXFLAGS) -c blur.cpp -o blur.o

blur_par.o: blur_par.cpp
	$(CXX) $(CXXFLAGS) -c blur_par.cpp -o blur_par.o

matrix.o: matrix.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c matrix.cpp -o matrix.o

ppm.o: ppm.cpp ppm.hpp
	$(CXX) $(CXXFLAGS) -c ppm.cpp -o ppm.o

clean:
	rm -f *.o blur blur_par
