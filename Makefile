CXX = g++
CFLAGS = -Wall -std=c++11 -O3 -fPIC

all: KMeansContrast

KMeansContrast: main.cpp ACKMeans.o SLKMeans.o KMeans.o
	$(CXX) $(CFLAGS) main.cpp ACKMeans.o SLKMeans.o KMeans.o -o KMeansContrast -lm
ACKMeans.o: ACKMeans.cpp ACKMeans.h KMeans.o
	$(CXX) $(CFLAGS) -c ACKMeans.cpp
SLKMeans.o: SLKMeans.cpp SLKMeans.h KMeans.o
	$(CXX) $(CFLAGS) -c SLKMeans.cpp
KMeans.o: KMeans.cpp KMeans.h
	$(CXX) $(CFLAGS) -c KMeans.cpp
clean:
	rm -f *~ KMeans.o ACKMeans.o SLKMeans.o KMeansContrast
rebuild:
	make clean && make