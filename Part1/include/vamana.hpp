/*vamana.hpp*/
#pragma once 

#include <utility>
#include <vector>



using namespace std;



/*Returns a vector l_closest that cointains the k closest NN's and a vector v cointaining the visited nodes*/
pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>&, graph_node&, graph_node&, int, int);



/*Graph gets modified by setting at most R new outgoing neigbors for p*/
void robust_prune(GraphNode, vector<GraphNode>&, float, int);



/*Returns a vamana indexing graph*/
vector<graph_node> vamana(vector<vector<float>>&, float, int, int, GraphNode&);



/*Creates a random r-regular graph and returns it from the parameters*/
void create_random_graph(vector<graph_node>&, vector<vector<float>>&, int);



/*Finds and returns the medoid of the graph*/
GraphNode find_medoid(vector<graph_node>&);



/*Returns a random permutation of integers*/
vector<int> give_random_permutation(int);