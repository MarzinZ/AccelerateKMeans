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
    for (auto it = point.getData().begin(); it != point.getData().end(); ++it) {
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
                float distance = ACPoint::dist(clusters[i].getCentroid(), clusters[j].getCentroid());
                if (distance < min)
                    min = distance;
            }
        }
        clusters[i].minDist = min;
    }
}

int KMeans::iteration = 0;
long KMeans::numUpdate = 0;

void KMeans::runKmeans(vector<SamplePoint>& points, vector<Cluster>& clusters, int iteration)
{
    int iter = 0;
    init(points, clusters);
    while (iter < iteration) {
        ++iter;
        for (auto it = points.begin(); it != points.end(); ++it) {
            int index = it->getIndexOfCluster();
            point2centroid(*it, clusters);
            ++KMeans::numUpdate;
            if (index != it->getIndexOfCluster()) {
                clusters[index].remove(*it);
                clusters[it->getIndexOfCluster()].add(*it);
            }
        }
        update(clusters);
    }
    KMeans::iteration = iter;
}

void KMeans::runKmeans(vector<ACPoint>& points, vector<ACCluster>& clusters, int iteration)
{
    int iter = 0;
    init(points, clusters);
    while (iter < iteration) {
        ++iter;
        ACCluster::calMinDist(clusters);
        for (auto it = points.begin(); it != points.end(); ++it) {
            float bound = std::min(it->getSecDist(), clusters[it->getIndexOfCluster()].getMinDist()/2);
            if (it->getMinDist() > bound) {
                it->setMinDist(ACPoint::dist(*it, clusters[it->getIndexOfCluster()].getCentroid()));
                if (it->getMinDist() > bound) {
                    int index = it->getIndexOfCluster();
                    point2centroid(*it, clusters);
                    ++numUpdate;
                    if (index != it->getIndexOfCluster()) {
                        clusters[index].remove(*it);
                        clusters[it->getIndexOfCluster()].add(*it);
                    }
                }
            }
        }
        moveCentroids(clusters);
        updateBounds(points, clusters);
    }
    KMeans::iteration = iter;
}

void KMeans::point2centroid(SamplePoint& point, vector<Cluster>& clusters)
{
    float minDist = std::numeric_limits<float>::infinity();
    for (int i = 0; i < clusters.size(); ++i) {
        float distance = Point::dist(point, clusters[i].getCentroid());
        if (distance < minDist) {
            minDist = distance;
            point.setIndexOfCluster(i);
        }
    }
}

void KMeans::init(vector<SamplePoint> &points, vector<Cluster> &clusters)
{
    for (auto it = points.begin(); it != points.end(); ++it) {
        point2centroid(*it, clusters);
        clusters[it->getIndexOfCluster()].add(*it);
    }
}

void KMeans::update(vector<Cluster>& clusters)
{
    for (auto it = clusters.begin(); it != clusters.end(); ++it) {
        it->update();
    }
}

void KMeans::point2centroid(ACPoint& point, vector<ACCluster>& clusters)
{
    float minDist = std::numeric_limits<float>::infinity();
    float secDist = minDist;
    for (int i = 0; i < clusters.size(); ++i) {
        float distance = ACPoint::dist(point, clusters[i].getCentroid());
        if (distance < minDist) {
            minDist = distance;
            point.setIndexOfCluster(i);
        } else if (distance < secDist) {
            secDist = distance;
        }
    }
    point.setMinDist(minDist);
    point.setSecDist(secDist);
}

void KMeans::init(vector<ACPoint> &points, vector<ACCluster> &clusters)
{
    for (auto it = points.begin(); it != points.end(); ++it) {
        point2centroid(*it, clusters);
        clusters[it->getIndexOfCluster()].add(*it);
    }
}

void KMeans::moveCentroids(vector<ACCluster>& clusters)
{
    for (auto it = clusters.begin(); it != clusters.end(); ++it) {
        ACCluster old = *it;
        it->update();
        it->setMoveDist(ACPoint::dist(old.getCentroid(), it->getCentroid()));
    }
}

void KMeans::updateBounds(vector<ACPoint>& points, const vector<ACCluster>& clusters)
{
    float maxMoveDist = 0.0f, secMaxMoveDist = 0.0f;
    int indexOfCenter = 0;
    for (int i = 0; i < clusters.size(); ++i) {
        float dist = clusters[i].getMoveDist();
        if (dist > maxMoveDist) {
            maxMoveDist = dist;
            indexOfCenter = i;
        } else if (dist > secMaxMoveDist) {
            secMaxMoveDist = dist;
        }
    }
    
    for (auto it = points.begin(); it != points.end(); ++it) {
        it->setMinDist(it->getMinDist()+clusters[it->getIndexOfCluster()].getMoveDist());
        if (it->getIndexOfCluster() == indexOfCenter) {
            it->setSecDist(it->getSecDist()-secMaxMoveDist);
        } else {
            it->setSecDist(it->getSecDist()-maxMoveDist);
        }
    }
}


