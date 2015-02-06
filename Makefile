CXX = g++
CFLAGS = -Wall -std=c++11 -O3 -fPIC

all: KMeansContrast

KMeansContrast: main.cpp KMeans.o
	$(CXX) $(CFLAGS) main.cpp KMeans.o -o KMeansContrast -lm
KMeans.o: KMeans.cpp KMeans.h
	$(CXX) $(CFLAGS) -c KMeans.cpp
clean:
	rm -f *~ KMeans.o KMeansContrast
rebuild:
	make clean && make