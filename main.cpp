#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "KMeans.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

typedef void initFuncType(vector<Point>& points, vector<Point>& centroids, int k);

void chooseInitCentroid(vector<Point>& points, vector<Point>& centroids, int k)
{
    //std::srand(0); // for debug
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::random_shuffle(points.begin(), points.end());
    std::copy_n(points.begin(), k, std::back_inserter(centroids));
}

float nearestCluster(const Point& point, const vector<Point>& centroids)
{
    float ret = std::numeric_limits<float>::infinity();
    for (auto it = centroids.begin(); it != centroids.end(); ++it) {
        float distance = Point::dist(point, *it);
        if (distance < ret)
            ret = distance;
    }
    return ret;
}

void kmeanspp(const vector<Point>& points, vector<Point>& centroids, int k)
{
    vector<float> dist;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int index = std::rand() % points.size();
    centroids.push_back(points[index]);
    size_t nPoints = points.size();
    
    while (--k) {
        float sum = 0;
        for (size_t j = 0; j < nPoints; ++j) {
            float distance = nearestCluster(points[j], centroids);
            dist.push_back(distance);
            sum += distance;
        }
        sum *= static_cast<double>(std::rand())/RAND_MAX;
        size_t nDist = dist.size();
        for (size_t j = 0; j < nDist; ++j) {
            sum -= dist[j];  // if dist[j] == 0, would not choose it to be a new centroid
            if (sum < 0) {
                centroids.push_back(points[j]);
                break;
            }
        }
    }
}

void readPoints(string filePath, vector<Point>& basic_points, vector<Point>& centroids, int k, initFuncType initFunc=chooseInitCentroid)
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
    
//    initFunc = chooseInitCentroid;
    initFunc(basic_points, centroids, k);
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

    KMeans::runKmeans(points, clusters, iteration);
    
    std::ofstream output("AC.txt");
    for (auto it = points.begin(); it != points.end(); ++it) {
        output << it->getIndexOfCluster() << endl;
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

    KMeans::runKmeans(points, clusters, iteration);

    std::ofstream output("SL.txt");
    for (auto it = points.begin(); it != points.end(); ++it) {
        output << it->getIndexOfCluster() << endl;
    }
    output.close();
}

void exit_with_help(string proc)
{
    cout << "Usage: " << proc << " data_file  num_of_centorids  [iterations=200]";
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
    
    readPoints(trainFile, points, centroids, k, chooseInitCentroid);
    
    typedef std::chrono::high_resolution_clock Clock;
    auto begin = Clock::now();
    runAccerateKMeans(points, centroids, iteration);
    cout << "AccerateKMeans cost time " << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - begin).count() << " milliseconds." << endl;
    cout << "iterations: " << KMeans::iteration << " updates " << KMeans::numUpdate << endl;
    
    cout << endl;
    KMeans::numUpdate = 0;
    begin = Clock::now();
    runSlowKMeans(points, centroids, iteration);
    cout << "SlowKMeans cost time " << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - begin).count() << " milliseconds." << endl;
    cout << "iterations: " << KMeans::iteration << " updates " << KMeans::numUpdate << endl;
    
    cout << endl;
    cout << "Finish..." << endl;
    
    return 0;
}
