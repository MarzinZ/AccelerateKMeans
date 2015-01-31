#include <iostream>
#include "SLKMeans.h"

int SLKMeans::iteration = 0;
long SLKMeans::numUpdate = 0;

void SLKMeans::point2centroid(SamplePoint& point, vector<Cluster>& clusters)
{
    float minDist = std::numeric_limits<float>::infinity();
    for (int i = 0; i < clusters.size(); ++i) {
        float distance = Point::dist(point, clusters[i].centroid);
        if (distance < minDist) {
            minDist = distance;
            point.indexOfCluster = i;
        }
    }
}

void SLKMeans::init(vector<SamplePoint>& points, vector<Cluster>& clusters)
{
    for (auto it = points.begin(); it != points.end(); ++it) {
        point2centroid(*it, clusters);
        clusters[it->indexOfCluster].add(*it);
    }
}

void SLKMeans::update(vector<Cluster>& clusters)
{
    for (auto it = clusters.begin(); it != clusters.end(); ++it) {
        it->update();
    }
}

void SLKMeans::kmeans(vector<SamplePoint>& points, vector<Cluster>& clusters, int iteration)
{
    int iter = 0;
    init(points, clusters);
    while (iter < iteration) {
        ++iter;
        for (auto it = points.begin(); it != points.end(); ++it) {
            int index = it->indexOfCluster;
            point2centroid(*it, clusters);
            ++SLKMeans::numUpdate;
            if (index != it->indexOfCluster) {
                clusters[index].remove(*it);
                clusters[it->indexOfCluster].add(*it);
            }
        }
        update(clusters);
    }
    SLKMeans::iteration = iter;
}