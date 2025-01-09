/*find_check.cpp*/

#include <iostream>
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



/*Takes the k_closest vector from the greedy result and a vector of groundtruth ids and returns the number of the ids that are found in both vectors*/
int vectors_equal(vector<GraphNode>& greedy_res, vector<float>& gt_res) {  

    int counter=0;
    for(int i = 1 ; i < gt_res.size() ; i++) {
        for(int j = 0 ; j < greedy_res.size() ; j++) {
            
            if(greedy_res[j]->id == gt_res[i]) {   
                counter++;  
  
                break;
            }
        }
    }
    return counter;
}

