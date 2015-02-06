#ifndef __KMeans_H_
#define __KMeans_H_

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::ostream;

// Base Point for inherence
class Point
{

public:
    Point()=default;
    Point(const vector<float>& v):_data(v) {}
    
    const vector<float>& getData() const { return _data;}
    const size_t size() const {return _data.size();}
    float& operator[](int index) {return _data[index];}
    const float operator[](int index) const {return _data[index];}
    
    static float dist(const Point& lhs, const Point& rhs);
    
private:
    vector<float> _data;
};

// Sample point in Lloyd's K-Means Algorithm
class SamplePoint:public Point
{
public:
    friend ostream& operator<<(ostream& os, const SamplePoint& point);

    SamplePoint()=default;
    SamplePoint(const Point& rhs):Point(rhs), indexOfCluster(-1){}
    
    const int getIndexOfCluster() const { return indexOfCluster;}
    void setIndexOfCluster(int i) { indexOfCluster = i ;}
    
private:
    int indexOfCluster;
};

// Sample point in Accerated K-Means Algorithm
class ACPoint:public SamplePoint{
public:
    ACPoint()=default;
    ACPoint(const Point& rhs):SamplePoint(rhs) {}
    
    const float getMinDist() const { return minDist; }
    void setMinDist(float dist) { minDist = dist;}
    const float getSecDist() const { return secMinDist;}
    void setSecDist(float dist) {secMinDist = dist;}

private:
    float minDist;      // the minist distance to all centroid
    float secMinDist;   // the second minist distance to all centroid
};

// Cluster using in Lloyd's K-Means Algorithm
class Cluster {
public:
    Cluster(){}
    Cluster(const Point& point):nPoints(0),centroid(point), sumPoints(point.size(), 0.0){}
    
    const Point& getCentroid() const { return centroid;}
    const int size() const { return nPoints; }
    const vector<float>& getSumPoints() const { return sumPoints;}
    
    void add(const Point& point);
    void remove(const Point& point);
    void update();
    
private:
    int nPoints;        // num of points belong to the cluster
    Point centroid;
    vector<float> sumPoints;
};

// Cluster using in Accerated K-Means Algorithm
class ACCluster:public Cluster {
public:
    ACCluster()=default;
    ACCluster(const Point& point):Cluster(point){}
    
    const float getMinDist() const { return minDist;}
    void setMinDist(float dist) {minDist = dist;}
    const float getMoveDist() const { return moveDist;}
    void setMoveDist(float dist) {moveDist = dist;}
    static void calMinDist(vector<ACCluster>& clusters);
private:
    float minDist;        // the minist distance to other centroid
    float moveDist;       // moved distance after one iteration
};

class KMeans
{
public:
    static int iteration;
    static long numUpdate;
    
    static void runKmeans(vector<SamplePoint>& points, vector<Cluster>& clusters, int iteration);
    static void runKmeans(vector<ACPoint>& points, vector<ACCluster>& clusters, int iteration);
    
private:
    // Lloyd's  Algorithm
    static void point2centroid(SamplePoint& point, vector<Cluster>& clusters);
    static void init(vector<SamplePoint>& points, vector<Cluster>& clusters);
    static void update(vector<Cluster>& clusters);
    
    // Accerated Algorithm
    static void point2centroid(ACPoint& point, vector<ACCluster>& clusters);
    static void init(vector<ACPoint>& points, vector<ACCluster>& clusters);
    static void moveCentroids(vector<ACCluster>& clusters);
    static void updateBounds(vector<ACPoint>& points, const vector<ACCluster>& clusters);
   
};

#endif //__AKMeans_H_
