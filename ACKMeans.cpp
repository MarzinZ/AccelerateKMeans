#include <iostream>
#include "ACKMeans.h"

int ACKMeans::iteration = 0;
long ACKMeans::numUpdate = 0;

void ACKMeans::point2centroid(ACPoint& point, vector<ACCluster>& clusters)
{
    float minDist = std::numeric_limits<float>::infinity();
    float secDist = minDist;
    for (int i = 0; i < clusters.size(); ++i) {
        float distance = ACPoint::dist(point, clusters[i].centroid);
        if (distance < minDist) {
            minDist = distance;
            point.indexOfCluster = i;
        } else if (distance < secDist) {
            secDist = distance;
        }
    }
    point.minDist = minDist;
    point.secMinDist = secDist;
}

void ACKMeans::init(vector<ACPoint>& points, vector<ACCluster>& clusters)
{
    for (auto it = points.begin(); it != points.end(); ++it) {
        point2centroid(*it, clusters);
        clusters[it->indexOfCluster].add(*it);
    }
}

void ACKMeans::moveCentroids(vector<ACCluster>& clusters)
{
    for (auto it = clusters.begin(); it != clusters.end(); ++it) {
        ACCluster old = *it;
        it->update();
        it->moveDist = ACPoint::dist(old.centroid, it->centroid);
    }
}

void ACKMeans::updateBounds(vector<ACPoint>& points, const vector<ACCluster>& clusters)
{
    float maxMoveDist = 0.0f, secMaxMoveDist = 0.0f;
    int indexOfCenter = 0;
    for (int i = 0; i < clusters.size(); ++i) {
        float dist = clusters[i].moveDist;
        if (dist > maxMoveDist) {
            maxMoveDist = dist;
            indexOfCenter = i;
        } else if (dist > secMaxMoveDist) {
            secMaxMoveDist = dist;
        }
    }

    for (auto it = points.begin(); it != points.end(); ++it) {
        it->minDist += clusters[it->indexOfCluster].moveDist;
        if (it->indexOfCluster == indexOfCenter) {
            it->secMinDist -= secMaxMoveDist;
        } else {
            it->secMinDist -= maxMoveDist;
        }
    }
}

void ACKMeans::kmeans(vector<ACPoint> &points, vector<ACCluster> &clusters, int iteration)
{
    int iter = 0;
    init(points, clusters);
    while (iter < iteration) {
        ++iter;
        ACCluster::calMinDist(clusters);
        for (auto it = points.begin(); it != points.end(); ++it) {
            float bound = std::min(it->secMinDist, clusters[it->indexOfCluster].minDist/2);
            if (it->minDist > bound) {
                it->minDist = ACPoint::dist(*it, clusters[it->indexOfCluster].centroid);
                if (it->minDist > bound) {
                    int index = it->indexOfCluster;
                    point2centroid(*it, clusters);
                    ++numUpdate;
                    if (index != it->indexOfCluster) {
                        clusters[index].remove(*it);
                        clusters[it->indexOfCluster].add(*it);
                    }
                }
            }
        }
        moveCentroids(clusters);
        updateBounds(points, clusters);
    }
    ACKMeans::iteration = iter;
}