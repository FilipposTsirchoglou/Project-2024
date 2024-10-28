/*vamana.hpp*/
#pragma once 

#include <utility>
#include <vector>

using namespace std;

pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>&, graph_node&, graph_node&, int, int);

void robust_prune(GraphNode, vector<GraphNode>&, int, int);

vector<graph_node> vamana(vector<vector<float>>&, float, int, int);

void create_random_graph(vector<graph_node>&, vector<vector<float>>&, int);

GraphNode find_medoid(vector<graph_node>&);

vector<int> give_random_permutation(int);