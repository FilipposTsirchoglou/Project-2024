/*util.cpp*/

#include <omp.h>

#include "../include/util.hpp"
#include <math.h>
#include <time.h>
#include <queue>
#include <algorithm>


#include <assert.h>

using namespace std;



/*Returns a vector with the difference of the k-approx NNs and the visited nodes*/
vector<GraphNode> vectors_difference(vector<GraphNode>& l_closest, vector<GraphNode>& visited) {  

    omp_set_num_threads(4);
    
    // A vector to store the final difference
    std::vector<GraphNode> difference;
    
    // Use thread-private temporary vectors to avoid race conditions
    #pragma omp parallel
    
    {
        vector<GraphNode> local_difference;   // Private vector for each thread

        #pragma omp for nowait
        for (int i = 0; i < l_closest.size(); i++) {
            bool are_same = false;
            for (int j = 0; j < visited.size(); j++) {
                if (l_closest[i] == visited[j]) {
                    are_same = true;
                    break;
                }
            }
            if (!are_same) {
                local_difference.push_back(l_closest[i]);
            }
        }

        // Merge local results into the shared vector
        #pragma omp critical
        difference.insert(difference.end(), local_difference.begin(), local_difference.end());
    }
    
    return difference;
}



/*Takes 2 vectors and returns the euclidean distance*/
double euclidean(const vector<float>& p1, const vector<float>& p2) {    
    
    float dist = 0.0;

    for(int i = 0 ; i < p1.size() ; i++) { 
        int temp = p1[i] - p2[i];
        dist += temp * temp;
    }

    return sqrt(dist);

}



/*Returns true if euclidean of first element < euclidean of second element*/
bool compare_eucl(GraphNode g1, GraphNode g2) {
    
    return g1->eucl_dist < g2->eucl_dist;

}



/*Returns a random integer in a certain range*/
int get_random_number(int max) {
    static bool seeded = false;

    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    return rand() % (max + 1);

}



/*Returns a pointer to the graph_node that has the min euclidean distance from xq*/
GraphNode find_min_eucl(std::vector<GraphNode>& p, graph_node& xq) {

    omp_set_num_threads(4);

    GraphNode* min_node = nullptr;
    float min_distance = numeric_limits<float>::max();

    #pragma omp parallel
    {
        GraphNode* local_min_node = nullptr;
        float local_min_distance = numeric_limits<float>::max();

        // Parallel computation of Euclidean distances
        #pragma omp for
        for (int i = 0; i < p.size(); i++) {
            float dist = euclidean(p[i]->cords, xq.cords);
            p[i]->eucl_dist = dist;

            // Update local minimum
            if (dist < local_min_distance) {
                local_min_distance = dist;
                local_min_node = &p[i];
            }
        }

        // Update global minimum using a critical section
        #pragma omp critical
        {
            if (local_min_distance < min_distance) {
                min_distance = local_min_distance;
                min_node = local_min_node;
            }
        }
    }

    return *min_node;
}



/*Returns the filter set of the data set vector passed*/
vector<int> find_filter_set(vector<vector<float>> &nodes) {

    omp_set_num_threads(4);

    vector<int> filter_set;  // Shared vector to store the result

    #pragma omp parallel
    {
        vector<int> local_filter_set;  // Thread-local vector for temporary storage

        #pragma omp for
        for (int i = 0; i < nodes.size(); i++) {
            bool filter_found = false;

            for (int j = 0; j < local_filter_set.size(); j++) {
                if (nodes[i][0] == local_filter_set[j]) {
                    filter_found = true;
                    break;
                }
            }

            if (!filter_found) {
                local_filter_set.push_back(nodes[i][0]);
            }
        }

        // Merge local_filter_set into the shared filter_set
        #pragma omp critical
        {
            for (int val : local_filter_set) {
                if (find(filter_set.begin(), filter_set.end(), val) == filter_set.end()) {
                    filter_set.push_back(val);
                }
            }
        }
    }

    // Sort the final filter_set in ascending order
    sort(filter_set.begin(), filter_set.end());

    return filter_set;
}