/*find_check.cpp*/

#include <iostream>
//#include "include/util.hpp"
//#include "../include/find_check.hpp"
#include "../include/util.hpp"
#include "../include/find_check.hpp"
#include "../include/vamana.hpp"

using namespace  std;



/*Takes a vector of graph node pointers and a vector of node ids and returns a vector of the ids that are in the same vector position but are different*/
vector<int> temp_vectors_equal(vector<GraphNode>& greedy_res, vector<int>& gt_res) {  
    
    bool are_same;
    vector<int> dif = {};

    for(int i = 0 ; i < greedy_res.size() ; i++) {
        are_same = false;
        for(int j = 0 ; j < greedy_res.size() ; j++) {
            if(greedy_res[i]->id == gt_res[j]) {     
                are_same = true;
                break;
            }
        }
        if (are_same == false) {
            dif.push_back(greedy_res[i]->id);
        }
    }
    
    return dif;

}



/*Takes the vamana graph, a query point , k and a search list of size L and returns a vector of the k closest neighbours of the query point*/
vector<GraphNode> find_k_closest_for_single_query_point(vector<graph_node>& vamana_graph, vector<float>& q_point, int k, int L) {
    
    graph_node query;
    query.id = vamana_graph.size();
    query.cords = q_point;
    query.eucl_dist = -1;                 // Putting the query point in a graph node and inserting it temporarily in the vamana graph
    query.outgoing={};                    // to find its k closest
    vamana_graph.push_back(query);
    
    // Performing greedy search in the vamana graph for the specific query point
    pair<vector<GraphNode>, vector<GraphNode>> greedy_result2 = greedy_search(vamana_graph, vamana_graph[8736], vamana_graph[vamana_graph.size() - 1], k, L);

    // After finding the k closest of the query, we rempve it from the graph
    vamana_graph.erase(vamana_graph.end()-1);
    
    return greedy_result2.first;

}



/*Returns a vector of vectors with the k closest neighbours of every single query point*/
vector<vector<GraphNode>> find_k_closest_for_all_query_points(vector<graph_node>& vamana_graph, vector<vector<float>>& q_points, int k, int L) {

    graph_node query;
    query.id = vamana_graph.size();
    query.cords = {};
    query.eucl_dist = -1;
    query.outgoing = {};
    vamana_graph.push_back(query);

    
    pair<vector<GraphNode>, vector<GraphNode>> greedy_result2;
    vector<vector<GraphNode>> k_closest_vec;
    //vector<int> all_dif;
    for (int i = 0 ; i < q_points.size() ; i++) {

        vamana_graph[vamana_graph.size()-1].cords = q_points[i];
        vamana_graph[vamana_graph.size()-1].outgoing = {};
        vamana_graph[vamana_graph.size()-1].eucl_dist = -1;

        greedy_result2 = greedy_search(vamana_graph, vamana_graph[8736], vamana_graph[vamana_graph.size() - 1], k, L);
        
        k_closest_vec.push_back(greedy_result2.first);
    }  

    vamana_graph.erase(vamana_graph.end()-1);

    return k_closest_vec;

}



/*Compares the k closest neighbours of a single point we found using the vamana algorithm, with the ones of the groundtruth file and returs the recall*/
float check_groundtruth_for_single_query_point(vector<GraphNode>& my_k_closest, vector<int>& gt_k_closest) {

    vector<int> dif = temp_vectors_equal(my_k_closest, gt_k_closest);
    float recall = (float)( my_k_closest.size()-dif.size() ) / (float)(my_k_closest.size());
    
    return recall;

}



/*Compares the k closest neighbours of all the query points we we found using the vamana algorithm, with the ones of the groundtruth file and returs the average recall*/
float check_groundtruth_for_all_query_points(vector<vector<GraphNode>>& my_k_closest_vec, vector<vector<int>>& gt_k_closest_vec) {
    
    float recall_sum = 0;
    
    for(int i = 0 ; i < my_k_closest_vec.size() ; i++) {
        
        vector<int> dif = temp_vectors_equal(my_k_closest_vec[i], gt_k_closest_vec[i]);
        float recall = (float)( my_k_closest_vec[i].size()-dif.size() ) / (float)(my_k_closest_vec[i].size());
        //cout << "recall: " << recall << endl;
        recall_sum += recall;

    }
    
    return recall_sum / (float)(gt_k_closest_vec.size());

}