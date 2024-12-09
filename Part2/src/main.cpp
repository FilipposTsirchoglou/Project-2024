/*#include <iostream>
#include <vector>
#include <forward_list>
#include <string>
#include <utility>
#include <bits/stdc++.h>
#include <math.h>
#include <time.h>
#include <queue>
#include "include/util.hpp"
#include <map>
#include <algorithm> 
#include "include/vamana.hpp"
#include "include/find_check.hpp" 
#include "include/file_operations.hpp"*/
#include <iostream>
#include "../include/util.hpp"
#include "../include/find_check.hpp"
#include "../include/vamana.hpp"
#include "../include/file_operations.hpp"

using namespace std;

int main(void) {

    /*dummy node of id -1 (it is a placeholder)*/
    graph_node dummy_node;
    dummy_node.id = -1;
    dummy_node.cords = {};
    dummy_node.filters = {};
    dummy_node.outgoing = {};
    dummy_node.eucl_dist = -1;

    //Read the configuration file and store its values
    string DATA_FILE, QUERIES_FILE, METHOD_TYPE;
    int L, R, k, th, L_small, R_small, R_stich;
    float a;

    read_config_file("config.txt", DATA_FILE, QUERIES_FILE, R, L, a, k, th, L_small, R_small, R_stich, METHOD_TYPE);  //method type F0 or F1 means filtered_vamana type 0
    cout << METHOD_TYPE << endl;
    cout << "DATA_FILE: " << DATA_FILE << " QUERIES_FILE: " << QUERIES_FILE << " R: " << R << " L: " << L << " a: " << a << " k: " << k << " th: " << th << " L_small: " << L_small << " R_small: " << R_small << " R_stich: " << R_stich << " method_type: " << METHOD_TYPE << endl;
    //cout << "L: " << L << " R: " << R << " a: " << a << " k: " << k << " th: " << th << " L_small: " << L_small << " R_small: " << R_small << " R_stich: " << R_stich << " method_type: " << METHOD_TYPE << endl;

    vector<vector<float>> nodes;    //all the points of the dataset
    ReadBin("dummy-data.bin", 102, nodes);

    vector<vector<float>> queries;  //all the queries
    ReadBin("dummy-queries.bin", 104, queries);

    vector<vector<float>> gt;      //groundtruth
    ReadBin("GroundTruth.bin", 101, gt);    //gt has 101 dimensios. gt[0] is the query id and the rest are the 100 closest points

    vector<graph_node> graph;
    vector<int> filter_set = find_filter_set(nodes);

    for(int i = 0 ; i < nodes.size() ; i++) {
    
        //graph creation
        graph_node g;
        g.id = i;

        for(int j = 2 ; j < nodes[i].size() ; j++) {
        g.cords.push_back(nodes[i][j]);
        }

        g.eucl_dist = -1;
        g.outgoing = {};
        g.filters.push_back(nodes[i][0]);

        graph.push_back(g);

    }

    vector<int> how_many_points_by_filter;    //shows how many points each filter has eg. filter[1] has the numver of filters for the filter 1

    for (int i=0 ; i<filter_set.size() ; i++) {   //for every filter

        int filter_count = 0;
        for (int j=0 ; j<nodes.size() ; j++) {
            if (nodes[j][0] == filter_set[i]) {
                filter_count++;
            }
        }
        how_many_points_by_filter.push_back(filter_count);
    }

    map<int, int> med = filteredFindMedoid(graph, 1, filter_set);   //has one medoid for every filter
    cout << METHOD_TYPE << endl;
    if (METHOD_TYPE == "F0" || METHOD_TYPE == "F1") {
        //call vamana to return the vamana indexing graph
        cout << "in F0 or F1" << endl;  
        vector<graph_node> vamana_filtered = filteredVamana(graph, a, L, R, filter_set, th, med);

        int count = 0;
        float recall_sum = 0;

        if (METHOD_TYPE == "F1") {
            cout << "F1" << endl;

            for (int i=0 ; i<queries.size() ; i++) {

                if (queries[i][0] == 1) {     //thas is of type 1
                    cout << "query: " << i <<endl;
                    count++;
            
                    graph_node gq;              //create a temorary graph_node
                    gq.id = vamana_filtered.size();
                    for(int j = 4 ; j < queries[i].size() ; j++) {
                        gq.cords.push_back(queries[i][j]);
                    }

                    gq.eucl_dist = -1;
                    gq.outgoing = {};
                    for (int k=0 ; k<filter_set.size() ; k++) {
                        gq.filters.push_back(filter_set[k]);
                    }

                    vamana_filtered.push_back(gq);        //push the graph_node in the graph vector

                    vector<GraphNode> s1;
                
                    s1.push_back(&vamana_filtered[med[queries[i][1]]]);                

                    //run the filtered greedy search to find k closest neighbours
                    pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(vamana_filtered, s1, vamana_filtered[vamana_filtered.size() - 1], k, 200);
                    vector<GraphNode> k_closest = result.first;
                
                    vamana_filtered.erase(vamana_filtered.end()-1);

                    //if kclosest is less than k fill it with dummy nodes
                    if (k_closest.size() < 100) {
                        for (int j = k_closest.size() ; j < 100 ; j++) {
                            k_closest.push_back(&dummy_node);
                        }
                    }
                
                    //search the id of the corresponding query in the groundtruth
                    int id=-10;                
                    for (int j=0 ; j<gt.size() ; j++) {
                        if (gt[j][0] == i) {
                            id = j;
                            break;
                        }
                    }

                    //find how many points are same between the algrithms result and the groundtruth
                    int same_points = vectors_equal(k_closest, gt[id]); 

                    //calculate recall
                    float recall = (float)same_points / 100.0;
                    recall_sum += recall;
                } 
            }

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using filtered vamana: " << avg_recall << endl;

        } else if (METHOD_TYPE == "F0") {

            for (int i=0 ; i<queries.size() ; i++) {

                if (queries[i][0] == 0) {     //thas is of type 0
                    cout << "query: " << i <<endl;
                    count++;
                    
                    //create and initialize a temmporary graph node
                    graph_node gq;              
                    gq.id = vamana_filtered.size();
                    for(int j = 4 ; j < queries[i].size() ; j++) {
                        gq.cords.push_back(queries[i][j]);
                    }
                    gq.eucl_dist = -1;
                    gq.outgoing = {};
                    for (int k=0 ; k<filter_set.size() ; k++) {
                        gq.filters.push_back(filter_set[k]);
                    }      
                    vamana_filtered.push_back(gq);        //push the graph_node in the graph vector

                    vector<GraphNode> s1;
                
                    //trick to achieve better recall in filtered queries
                    for (auto it=med.begin() ; it!=med.end() ; ++it) {
                        s1.push_back(&vamana_filtered[it->second]);
                    }

                    vector<GraphNode> k_closest_results;
      
                    for (int j=0 ; j<filter_set.size() ; j++ ) {
                        vector<GraphNode> s_temp;
                        s_temp.push_back(&vamana_filtered[med[filter_set[j]]]);
                        pair<vector<GraphNode>, vector<GraphNode>> result1 = filteredGreedySearch(vamana_filtered, s_temp, vamana_filtered[vamana_filtered.size() - 1], 1, L);
                        k_closest_results.push_back(result1.first[0]);
                    }          

                    //run the filtered greedy search to find k closest neighbours
                    pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(vamana_filtered, k_closest_results, vamana_filtered[vamana_filtered.size() - 1], k, L);
                    vector<GraphNode> k_closest = result.first;
                
                    vamana_filtered.erase(vamana_filtered.end()-1);

                    //if kclosest is less than k fill it with dummy nodes
                    if (k_closest.size() < 100) {
                        for (int j = k_closest.size() ; j < 100 ; j++) {
                            k_closest.push_back(&dummy_node);
                        }
                    }
                
                    //search the id of the corresponding query in the groundtruth
                    int id=-10;                
                    for (int j=0 ; j<gt.size() ; j++) {
                        if (gt[j][0] == i) {
                            id = j;
                            break;
                        }
                    }

                    //find how many points are same between the algrithms result and the groundtruth
                    int same_points = vectors_equal(k_closest, gt[id]); 

                    //calculate recall
                    float recall = (float)same_points / 100.0;
                    recall_sum += recall;
                } 
            }

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using filtered vamana: " << avg_recall << endl;
        }  
    } else if (METHOD_TYPE == "S0" || METHOD_TYPE == "S1") {

        int count = 0;
        float recall_sum = 0;
        if (METHOD_TYPE == "S1") {
            vector<vector<graph_node>> stich_vamana_graph;
            vector<GraphNode> medoids;
            cout << "filter set" << endl;
            for (int i=0 ; i<filter_set.size() ; i++) {
                cout << filter_set[i] << endl;
            }
            for (int i=0 ; i<filter_set.size() ; i++) {
                vector<vector<float>> all_points_with_same_filter;
                for (int j=0 ; j<nodes.size() ; j++) {
                    if (filter_set[i] == nodes[j][0]) {
                    
                        all_points_with_same_filter.push_back(nodes[j]);

                    }
                }
                cout << i << " " << all_points_with_same_filter.size() << endl; 
                //data set now is all_points_with_same_filter
                GraphNode med = NULL;
                vector<graph_node> vamana_graph = vamana(all_points_with_same_filter, a, L, R, med);
                stich_vamana_graph.push_back(vamana_graph);
                medoids.push_back(med);
            }

            //at this point the stich vamana has been created

            for (int i=0 ; i<queries.size() ; i++) {
                
                if (queries[i][0] == 1) {
                    if (queries[i][1] > filter_set.size()) {
                        continue;
                    }

                    cout << "query " << i << endl;
                    count++;
                    int filter = queries[i][1];

                    //temporary graph_noode creation
                    graph_node gq;
                    gq.id = stich_vamana_graph[filter].size();
                    gq.filters.push_back(filter);
                    gq.eucl_dist = -1;
                    for (int k=4 ; k<queries[i].size() ; k++) {
                        gq.cords.push_back(queries[i][k]);
                    }
                    gq.outgoing = {};

                    stich_vamana_graph[filter].push_back(gq);
                    
                    int id_temp = gq.id;

                    pair<vector<GraphNode>, vector<GraphNode>> greedy_result_for_filter = greedy_search(stich_vamana_graph[filter], stich_vamana_graph[filter][id_temp], stich_vamana_graph[filter][stich_vamana_graph[filter].size()-1], k, L);
                    vector<GraphNode> k_closest_for_query = greedy_result_for_filter.first;
                    

                    cout << "in main" << endl;
                    //cout << endl;

                    stich_vamana_graph[filter].erase(stich_vamana_graph[filter].end()-1);

                    //if kclosest is less than k fill it with dummy nodes
                    if (k_closest_for_query.size() < 100) {
                        for (int j = k_closest_for_query.size() ; j < 100 ; j++) {
                            k_closest_for_query.push_back(&dummy_node);
                        }
                    }
                
                    //search the id of the corresponding query in the groundtruth
                    int id=-10;                
                    for (int j=0 ; j<gt.size() ; j++) {
                        if (gt[j][0] == i) {
                            id = j;
                            break;
                        }
                    }

                    //find how many points are same between the algrithms result and the groundtruth
                    cout << "same points" << endl;
                    int same_points = vectors_equal(k_closest_for_query, gt[id]); 

                    //calculate recall
                    float recall = (float)same_points / 100.0;
                    cout  << recall << endl;
                    recall_sum += recall;

                }
            } 
            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using filtered vamana: " << avg_recall << endl;
        }

    }
    return 0;
}