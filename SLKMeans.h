#ifndef __ACKMeans__SLKMeans__
#define __ACKMeans__SLKMeans__

#include <vector>
#include "KMeans.h"

using std::vector;

class SLKMeans {
public:
    static int iteration;
    static long numUpdate;
    
    static void point2centroid(SamplePoint& point, vector<Cluster>& clusters);
    static void init(vector<SamplePoint>& points, vector<Cluster>& clusters);
    static void update(vector<Cluster>& clusters);
    static void kmeans(vector<SamplePoint>& points, vector<Cluster>& clusters, int iteration=200);
};

#endif /* defined(__ACKMeans__SLKMeans__) */
