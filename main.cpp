#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <ctime>
#include "ACKMeans.h"
#include "SLKMeans.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

void readPoints(string filePath, vector<Point>& basic_points, vector<Point>& centroids, int k)
{

    std::ifstream input(filePath);
    string line;
    while (getline(input, line)) {
        std::istringstream iss(line);
        vector<float> lineVec;
        while (iss) {
            float item;
            if (iss >> item)
                lineVec.push_back(item);
        }
        basic_points.push_back(lineVec);
    }
    input.close();
    
    vector<Point> tmpVec = basic_points;
    std::srand(0); // for debug
    std::srand((unsigned)std::time(nullptr));
    std::random_shuffle(tmpVec.begin(), tmpVec.end());
    std::copy_n(tmpVec.begin(), k, std::back_inserter(centroids));
}

void runAccerateKMeans(const vector<Point>& basic_points, const vector<Point>& basic_centroids, int iteration)
{
    size_t nPoints = basic_points.size();
    size_t nClusters = basic_centroids.size();
    
    vector<ACPoint> points(nPoints);
    vector<ACCluster> clusters(nClusters);
    
    for (int i = 0; i < nPoints; ++i) {
        points[i] = basic_points[i];
    }
    for (int i = 0; i < nClusters; ++i) {
        clusters[i] = basic_centroids[i];
    }

    ACKMeans::kmeans(points, clusters, iteration);
    
    std::ofstream output("AC.txt");
    for (auto it = points.begin(); it != points.end(); ++it) {
        output << it->indexOfCluster << endl;
    }
    output.close();
}

void runSlowKMeans(const vector<Point>& basic_points, const vector<Point>& basic_centroids, int iteration)
{
    size_t nPoints = basic_points.size();
    size_t nClusters = basic_centroids.size();
    
    vector<SamplePoint> points(nPoints);
    vector<Cluster> clusters(nClusters);
    
    for (int i = 0; i < nPoints; ++i) {
        points[i] = basic_points[i];
    }
    for (int i = 0; i < nClusters; ++i) {
        clusters[i] = basic_centroids[i];
    }

    SLKMeans::kmeans(points, clusters, iteration);

    std::ofstream output("SL.txt");
    for (auto it = points.begin(); it != points.end(); ++it) {
        output << it->indexOfCluster << endl;
    }
    output.close();
}

void exit_with_help(string proc)
{
    cout << "Usage: " << proc << " data_file num_of_centorids [iterations]";
    cout << endl;
    exit(-1);
}

int main(int argc, const char * argv[])
{
    int iteration = 200;
    int k = 3;
    string trainFile;
    if (argc == 3) {
        trainFile = argv[1];
        k = atoi(argv[2]);
    } else if (argc == 4) {
        trainFile = argv[1];
        k = atoi(argv[2]);
        iteration = atoi(argv[3]);
    } else {
        exit_with_help(argv[0]);
    }

    vector<Point> points;
    vector<Point> centroids;
    
    readPoints(trainFile, points, centroids, k);
    
    typedef std::chrono::high_resolution_clock Clock;
    auto begin = Clock::now();
    runAccerateKMeans(points, centroids, iteration);
    cout << "AccerateKMeans cost time " << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - begin).count() << " milliseconds." << endl;
    cout << "iterations: " << ACKMeans::iteration << " updates " << ACKMeans::numUpdate << endl;
    
    cout << endl;
    begin = Clock::now();
    runSlowKMeans(points, centroids, iteration);
    cout << "SlowKMeans cost time " << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - begin).count() << " milliseconds." << endl;
    cout << "iterations: " << SLKMeans::iteration << " updates " << SLKMeans::numUpdate << endl;
    
    cout << endl;
    cout << "Finish..." << endl;
    
    return 0;
}
