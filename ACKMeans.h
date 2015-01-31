//
// Created by marzin on 1/30/15.
// Copyright (c) 2015 Fudan. All rights reserved.
//


#ifndef __ACKMeans_H_
#define __ACKMeans_H_

#include <vector>
#include "KMeans.h"

using std::vector;

class ACKMeans {
public:
    static int iteration;
    static long numUpdate;
    
    static void point2centroid(ACPoint & point, vector<ACCluster>& clusters);
    static void init(vector<ACPoint>& points, vector<ACCluster>& clusters);
    static void moveCentroids(vector<ACCluster>& clusters);
    static void updateBounds(vector<ACPoint>& points, const vector<ACCluster>& clusters);
    static void kmeans(vector<ACPoint> &points, vector<ACCluster> &clusters, int iteration=200);
};


#endif //__ACKMeans_H_
