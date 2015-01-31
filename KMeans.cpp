#import <math.h>
#include "KMeans.h"

float Point::dist(const Point &lhs, const Point &rhs) {
    size_t n = lhs.size();
    float sum = 0.0;
    for (int i = 0; i < n; ++i)
        sum += pow((lhs._data[i]-rhs._data[i]), 2);
    return sqrtf(sum);
}

ostream& operator<<(ostream& os, const SamplePoint& point)
{
    for (auto it = point._data.begin(); it != point._data.end(); ++it) {
        os << *it << " ";
    }
    os << point.indexOfCluster;
    return os;
}

void Cluster::add(const Point &point) {
    ++nPoints;
    size_t n = point.size();
    for (int i = 0; i < n; ++i)
        sumPoints[i] += point[i];
}

void Cluster::remove(const Point &point) {
    --nPoints;
    size_t n = point.size();
    for (int i = 0; i < n; ++i)
        sumPoints[i] -= point[i];
}

void Cluster::update() {
    size_t n = centroid.size();
    for (int i = 0; i < n; ++i)
        centroid[i] = sumPoints[i] / nPoints;
}

void ACCluster::calMinDist(vector<ACCluster> &clusters) {
    size_t n = clusters.size();
    for (int i = 0; i < n; ++i) {
        float min = std::numeric_limits<float>::infinity();
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                float distance = ACPoint::dist(clusters[i].centroid, clusters[j].centroid);
                if (distance < min)
                    min = distance;
            }
        }
        clusters[i].minDist = min;
    }
}