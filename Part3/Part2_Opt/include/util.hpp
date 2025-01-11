/*util.hpp*/
#pragma once

#include <vector>
#include <forward_list>

using namespace std;



// GraphNode is a pointer to a graph node
typedef struct graph_node* GraphNode;



// This is a graph node
typedef struct graph_node {
    int id;   
    int pos;                                  // Node's unique id
    vector<float> cords;                        // Each node has a vector. These are the vector's cords
    forward_list<GraphNode> outgoing;           // List of pointers to graph_nodes
    double eucl_dist;                           // The euclidean distance between the nodes' vector to a query vector
    vector<int> filters;                       // The filters that have been applied to the node
}graph_node;



// This struct compares two graph node pointers based on their euclidean distance with the query point
struct CompareDistance {
    bool operator()(GraphNode p1, GraphNode p2) {
        // Return true if p1 has higher distance, so it has lower priority
        return p1->eucl_dist > p2->eucl_dist;
        
    }
};



/*Returns a vector with the difference of the k-approx NNs and the visited nodes*/
vector<GraphNode> vectors_difference(vector<GraphNode>&, vector<GraphNode>&);



/*Takes 2 vectors and returns the euclidean distance*/
double euclidean(const vector<float>& , const vector<float>&);



/*Returns true if euclidean of first element < euclidean of second element*/
bool compare_eucl(GraphNode, GraphNode);



/*Returns a random integer in a certain range*/
int get_random_number(int);



/*Returns a pointer to the graph_node that has the min euclidean distance from xq*/ 
GraphNode find_min_eucl(vector<GraphNode>&, graph_node&, vector<double>&);


/*Returns the filter set of the data set vector passed*/
vector<int> find_filter_set(vector<vector<float>>&);