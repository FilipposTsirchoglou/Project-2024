/*util.cpp*/

#include "../include/util.hpp"
#include <math.h>
#include <time.h>
#include <queue>

using namespace std;



/*Returns a vector with the difference of the k-approx NNs and the visited nodes*/
vector<GraphNode> vectors_difference(vector<GraphNode>& l_closest, vector<GraphNode>& visited) {  
    
    bool are_same;
    vector<GraphNode> difference = {};

    for(int i = 0 ; i < l_closest.size() ; i++) {
        are_same = false;
        for(int j = 0 ; j < visited.size() ; j++) {
            if(l_closest[i] == visited[j]) {     
                are_same = true;
                break;
            }
        }
        if (are_same == false) {
            difference.push_back(l_closest[i]);
        }
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
GraphNode find_min_eucl(vector<GraphNode>& p, graph_node& xq) {
    priority_queue<GraphNode, vector<GraphNode>, CompareDistance> pq;   // Priority queue using a min heap. Inserts the elements with priority the smallest euclidian distance
        
    for (int i=0 ; i<p.size() ; i++) {
        p[i]->eucl_dist = euclidean(p[i]->cords, xq.cords);
        pq.push(p[i]);          
    }

    GraphNode p_star = pq.top();
    
    return p_star;
    
}