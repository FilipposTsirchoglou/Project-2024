/*util.hpp*/
#pragma once

#include <vector>
#include <forward_list>

using namespace std;

// GraphNode is a pointer to a graph node
typedef struct graph_node* GraphNode;


// This is a graph node
typedef struct graph_node {
    int id;                                     // Node's unique id
    vector<float> cords;                        // Each node has a vector. These are the vector's cords
    forward_list<GraphNode> outgoing;      // List of pointers to graph_nodes
    double eucl_dist;                     // The euclidean distance between the nodes' vector to a query vector
}graph_node;

// This struct compares two graph node pointers based on their euclidean distance with the query point
struct CompareDistance {
    bool operator()(GraphNode p1, GraphNode p2) {
        // Return true if p1 has higher distance, so it has lower priority
        return p1->eucl_dist > p2->eucl_dist;
    }
};

// Returns a vector with the difference of the k-approx NNs and the visited nodes
vector<GraphNode> vectors_difference(vector<GraphNode>&, vector<GraphNode>&);

// Returns the euclidean distance between two vectors
double euclidean(const vector<float>& , const vector<float>& );



// Compares two graph node pointers based on their euclidean distance with a query point
bool compare_eucl(GraphNode, GraphNode);



// Returns a random integer in a certain range
int get_random_number(int);

GraphNode find_min_eucl(vector<GraphNode>&, graph_node&);