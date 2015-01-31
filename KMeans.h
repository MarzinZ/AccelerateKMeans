#ifndef __KMeans_H_
#define __KMeans_H_

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::ostream;

class Point
{
public:
    Point(){}
    Point(const vector<float>& v):_data(v.begin(), v.end()) {}
    Point(const Point& rhs):_data(rhs._data.begin(), rhs._data.end()){}
    Point& operator=(Point rhs) {
        std::swap(_data, rhs._data);
        return *this;
    }
    
    const size_t size() const {return _data.size();}
    float& operator[](int index) {return _data[index];}
    const float operator[](int index) const {return _data[index];}
    static float dist(const Point& lhs, const Point& rhs);
protected:
    vector<float> _data;
};

class SamplePoint:public Point
{
public:
    friend ostream& operator<<(ostream& os, const SamplePoint& point);
    int indexOfCluster;

    SamplePoint(){}
    SamplePoint(const vector<float>& v):Point(v){};
    SamplePoint(const Point& rhs):Point(rhs){}
    SamplePoint(const SamplePoint& rhs):Point(rhs),
                                        indexOfCluster(rhs.indexOfCluster){}
    SamplePoint& operator=(Point rhs) {
        Point::operator=(rhs);
        return *this;
    }
};

class ACPoint:public SamplePoint{
public:
    float minDist; // the minist distance to all centroid
    float secMinDist; // the second minist distance to all centroid

    ACPoint(){}
    ACPoint(const Point& rhs):SamplePoint(rhs) {}
    ACPoint(const ACPoint& rhs):SamplePoint(rhs),
                                minDist(rhs.minDist),
                                secMinDist(rhs.secMinDist){}
    ACPoint& operator=(Point rhs) {
        SamplePoint::operator=(rhs);
        return *this;
    }
};

class Cluster {
public:
    int nPoints; // num of points belong to the cluster
    Point centroid;
    
    Cluster(){}
    Cluster(const Point& point):nPoints(0),centroid(point), sumPoints(point.size(), 0.0){}
    Cluster& operator=(Cluster rhs) {
        nPoints = rhs.nPoints;
        std::swap(centroid, rhs.centroid);
        std::swap(sumPoints, rhs.sumPoints);
        return *this;
    }
    void add(const Point& point);
    void remove(const Point& point);
    void update();
private:
    vector<float> sumPoints;
};

class ACCluster:public Cluster {
public:
    float minDist;  // the minist distance to other centroid
    float moveDist; // moved distance after one iteration
    
    ACCluster(){}
    ACCluster(const Point& point):Cluster(point){}
    ACCluster& operator=(ACCluster rhs) {
        Cluster::operator=(rhs);
        minDist = rhs.minDist;
        moveDist = rhs.moveDist;
        return *this;
    }
    
    static void calMinDist(vector<ACCluster>& clusters);
};

#endif //__AKMeans_H_
