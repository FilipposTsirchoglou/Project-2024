/*find_check.hpp*/

#pragma once
//#include "include/util.hpp"
#include <vector>

using namespace  std;

vector<int> temp_vectors_equal(vector<GraphNode>&, vector<int>&);


vector<GraphNode> find_k_closest_for_single_query_point(vector<graph_node>&, vector<float>&, int, int);


vector<vector<GraphNode>> find_k_closest_for_all_query_points(vector<graph_node>&, vector<vector<float>>&, int, int);


float check_groundtruth_for_single_query_point(vector<GraphNode>&, vector<int>&);


float check_groundtruth_for_all_query_points(vector<vector<GraphNode>>&, vector<vector<int>>&);



