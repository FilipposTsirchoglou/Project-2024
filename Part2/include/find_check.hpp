/*find_check.hpp*/

#pragma once
//#include "include/util.hpp"
#include <vector>



using namespace  std;



/*Takes a vector of graph node pointers and a vector of node ids and returns a vector of the ids that are in the same vector position but are different*/
vector<int> temp_vectors_equal(vector<GraphNode>&, vector<int>&);



/*Takes the vamana graph, a query point , k and a search list of size L and returns a vector of the k closest neighbours of the query point*/
vector<GraphNode> find_k_closest_for_single_query_point(vector<graph_node>&, vector<float>&, int, int, graph_node&);



/*Returns a vector of vectors with the k closest neighbours of every single query point*/
vector<vector<GraphNode>> find_k_closest_for_all_query_points(vector<graph_node>&, vector<vector<float>>&, int, int, graph_node&);



/*Compares the k closest neighbours of a single point we found using the vamana algorithm, with the ones of the groundtruth file and returs the recall*/
float check_groundtruth_for_single_query_point(vector<GraphNode>&, vector<int>&);



/*Compares the k closest neighbours of all the query points we we found using the vamana algorithm, with the ones of the groundtruth file and returs the average recall*/
float check_groundtruth_for_all_query_points(vector<vector<GraphNode>>&, vector<vector<int>>&);
/*TEST BRANCH 3*/
/*TEST BRANCH 2*/
/*niaou*/
/*niaou2*/