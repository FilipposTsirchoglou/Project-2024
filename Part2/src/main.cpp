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
    string DATA_FILE, QUERIES_FILE, GRAPH, GRAPH_NAME, METHOD_TYPE;
    int L, R, k, th, L_small, R_small, R_stich;
    float a;

    read_config_file("config.txt", DATA_FILE, QUERIES_FILE, GRAPH, GRAPH_NAME, R, L, a, k, th, L_small, R_small, R_stich, METHOD_TYPE);  //method type F0 or F1 means filtered_vamana type 0 or 1 (same as S0,S1 for stitced)
    cout << METHOD_TYPE << endl;
    cout << "DATA_FILE: " << DATA_FILE << " QUERIES_FILE: " << QUERIES_FILE << "GRAPH: " << GRAPH << "GRAPH_NAME: " << GRAPH_NAME << " R: " << R << " L: " << L << " a: " << a << " k: " << k << " th: " << th << " L_small: " << L_small << " R_small: " << R_small << " R_stich: " << R_stich << " method_type: " << METHOD_TYPE << endl;

    vector<vector<float>> nodes;    //all the points of the dataset
    ReadBin(DATA_FILE, 102, nodes);

    vector<vector<float>> queries;  //all the queries
    ReadBin(QUERIES_FILE, 104, queries);

    vector<vector<float>> gt;      //groundtruth
    ReadBin("GroundTruth.bin", 101, gt);    //gt has 101 dimensios. gt[0] is the query id and the rest are the 100 closest points

    vector<graph_node> graph;
    vector<int> filter_set = find_filter_set(nodes);

    /*Graph Creation*/
    for(int i = 0 ; i < nodes.size() ; i++) {
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

    map<int, int> med = filteredFindMedoid(graph, 1, filter_set);   // Has one medoid for every filter
    GraphNode dummy_med = &graph[0];
    int med_id = -1;
    cout << "Method used is: " << METHOD_TYPE << endl;
    if (METHOD_TYPE == "F0" || METHOD_TYPE == "F1") {

        vector<graph_node> vamana_filtered;
        if (GRAPH == "NO") {
            /*Creates a new graph and saves it to a .bin file*/
            vamana_filtered = filteredVamana(graph, a, L, R, filter_set, th, med);
            save_graph(GRAPH_NAME, vamana_filtered, dummy_med);
        } else if (GRAPH == "YES") {
            /*Reads the graph from a .bin file*/
            read_graph(GRAPH_NAME, vamana_filtered, med_id);
        }

        int count = 0;
        float recall_sum = 0;

        if (METHOD_TYPE == "F1") {

            for (int i=0 ; i<queries.size() ; i++) {

                if (queries[i][0] == 1) {     // Type 1
                    cout << "query: " << i <<endl;
                    count++;
            
                    graph_node gq;              // Create a temporary graph_node
                    gq.id = vamana_filtered.size();
                    for(int j = 4 ; j < queries[i].size() ; j++) {
                        gq.cords.push_back(queries[i][j]);
                    }

                    gq.eucl_dist = -1;
                    gq.outgoing = {};
                    for (int k=0 ; k<filter_set.size() ; k++) {
                        gq.filters.push_back(filter_set[k]);
                    }

                    vamana_filtered.push_back(gq);        // Push the graph_node in the graph vector

                    vector<GraphNode> s1;
                
                    s1.push_back(&vamana_filtered[med[queries[i][1]]]);                

                    // Run the filtered greedy search to find k closest neighbours
                    pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(vamana_filtered, s1, vamana_filtered[vamana_filtered.size() - 1], k, 200);
                    vector<GraphNode> k_closest = result.first;
                
                    vamana_filtered.erase(vamana_filtered.end()-1); //erase the temp graph_node

                    // If kclosest is less than k fill it with dummy nodes
                    if (k_closest.size() < 100) {
                        for (int j = k_closest.size() ; j < 100 ; j++) {
                            k_closest.push_back(&dummy_node);
                        }
                    }
                
                    // Search the id of the corresponding query in the groundtruth
                    int id=-10;                
                    for (int j=0 ; j<gt.size() ; j++) {
                        if (gt[j][0] == i) {
                            id = j;
                            break;
                        }
                    }

                    // Find how many points are same between the algrithms result and the groundtruth
                    int same_points = vectors_equal(k_closest, gt[id]); 

                    // Calculate recall
                    float recall = (float)same_points / 100.0;
                    cout << "recall: " << recall << endl;
                    recall_sum += recall;
                } 
            }

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 1 queries using filtered vamana: " << avg_recall << endl;

        } else if (METHOD_TYPE == "F0") {

            for (int i=0 ; i<queries.size() ; i++) {

                if (queries[i][0] == 0) {     // Type 0
                    cout << "query: " << i <<endl;
                    count++;
                    
                    // Create and initialize a temmporary graph node
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
                    vamana_filtered.push_back(gq);        // Push the graph_node in the graph vector

                    vector<GraphNode> s1;
                
                    // Trick to achieve better recall in filtered queries
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

                    // Run the filtered greedy search to find k closest neighbours
                    pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(vamana_filtered, k_closest_results, vamana_filtered[vamana_filtered.size() - 1], k, L*2);
                    vector<GraphNode> k_closest = result.first;
                
                    vamana_filtered.erase(vamana_filtered.end()-1);

                    // If kclosest is less than k fill it with dummy nodes
                    if (k_closest.size() < 100) {
                        for (int j = k_closest.size() ; j < 100 ; j++) {
                            k_closest.push_back(&dummy_node);
                        }
                    }
                
                    // Search the id of the corresponding query in the groundtruth
                    int id=-10;                
                    for (int j=0 ; j<gt.size() ; j++) {
                        if (gt[j][0] == i) {
                            id = j;
                            break;
                        }
                    }

                    // Find how many points are same between the algrithms result and the groundtruth
                    int same_points = vectors_equal(k_closest, gt[id]); 

                    // Calculate recall
                    float recall = (float)same_points / 100.0;
                    cout << "recall: " << recall << endl;
                    recall_sum += recall;
                } 
            }

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using filtered vamana: " << avg_recall << endl;
        }  
    } else if (METHOD_TYPE == "S0" || METHOD_TYPE == "S1") {

        
        if (METHOD_TYPE == "S0") {

            int count = 0;
            float recall_sum = 0;
            
            vector<vector<float>> all_points;
            vector<int> nothing;
            for (int i = 0 ; i < graph.size() ; i++) {
                
                all_points.push_back(graph[i].cords);
                nothing.push_back(i);
                
            }

            GraphNode med = NULL;
            vector<graph_node> stitched_vamana_graph;
            int med_id = -1;
   
            if (GRAPH == "NO") {
                stitched_vamana_graph = vamana(all_points, a, L, R, med, nothing);
                save_graph(GRAPH_NAME, stitched_vamana_graph, med);
            } else if (GRAPH == "YES") {
                read_graph(GRAPH_NAME, stitched_vamana_graph, med_id);
                med=&stitched_vamana_graph[med_id];
                //med = find_medoid(stitched_vamana_graph);

            }
            //med = find_medoid(stitched_vamana_graph);
            // At this point the stitched vamana has been created

            for (int i=0 ; i<queries.size() ; i++) {
                
                if (queries[i][0] == 0) {    //if it is a type 0 query

                    cout << "query " << i << endl;

                    count++;

                    //int filter = queries[i][1];

                    // Temporary graph_node creation
                    graph_node gq;
                    
                    gq.id = graph.size();
                    //gq.filters.push_back(filter);
                    gq.eucl_dist = -1;

                    for (int k=4 ; k<queries[i].size() ; k++) {

                        gq.cords.push_back(queries[i][k]);

                    }

                    gq.outgoing = {};

                    stitched_vamana_graph.push_back(gq);
                    
                    int id_temp = gq.id;


                    pair<vector<GraphNode>, vector<GraphNode>> greedy_result_for_filter = greedy_search(stitched_vamana_graph, *med, stitched_vamana_graph[stitched_vamana_graph.size()-1], k, L, 0);
                    
                    vector<GraphNode> k_closest_for_query = greedy_result_for_filter.first;
                    
                    stitched_vamana_graph.erase(stitched_vamana_graph.end()-1);

                    // If kclosest is less than k fill it with dummy nodes
                    if (k_closest_for_query.size() < 100) {

                        for (int j = k_closest_for_query.size() ; j < 100 ; j++) {

                            k_closest_for_query.push_back(&dummy_node);

                        }
                    }
                
                    // Search the id of the corresponding query in the groundtruth
                    int id=-10;                
                    for (int j=0 ; j<gt.size() ; j++) {

                        if (gt[j][0] == i) {

                            id = j;
                            break;

                        }
                    }

                    // Find how many points are same between the algrithms result and the groundtruth
                    cout << "same points" << endl;
                    int same_points = vectors_equal(k_closest_for_query, gt[id]); 

                    // Calculate recall
                    float recall = (float)same_points / 100.0;
                    cout  << recall << endl;
                    recall_sum += recall;

                }
            } 

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using stitched vamana: " << avg_recall << endl;
        }
        else if (METHOD_TYPE == "S1") {
            
            int count = 0;
            float recall_sum = 0;
            vector<vector<graph_node>> stich_vamana_graph;
            vector<GraphNode> medoids;

            for (int i=0 ; i<filter_set.size() ; i++) {
                float recall_sum = 0;
                vector<vector<float>> all_points_with_same_filter;
                vector<int> position_in_graph;

                for (int j=0 ; j<nodes.size() ; j++) {

                    if (filter_set[i] == nodes[j][0]) {
                        
                        all_points_with_same_filter.push_back(nodes[j]);
                        position_in_graph.push_back(j);

                    }
                }

                GraphNode med = NULL;
                vector<graph_node> vamana_graph = vamana(all_points_with_same_filter, a, L, R, med, position_in_graph);

                for (int j=0 ; j<queries.size() ; j++) {
                   if (queries[j][0] == 1 && queries[i][1] == i ) {    //if it is a type 1 query
                        if (queries[j][0] >= filter_set.size()) {
                            continue;
                        }

                        cout << "query: " << j << endl;
                        count++;

                        int filter = vamana_graph[0].cords[0];
                        cout << "filter: " << filter << endl;

                        graph_node gq; 
                        gq.id = vamana_graph.size();
                        gq.filters.push_back(filter);
                        gq.eucl_dist = -1;
                        for (int k=4 ; k<queries[j].size() ; k++) {
                            gq.cords.push_back(queries[j][k]);
                        }

                        gq.outgoing = {};

                        vamana_graph.push_back(gq);
                        int temp_id = gq.id;

                        GraphNode med = find_medoid(vamana_graph);

                        pair<vector<GraphNode>, vector<GraphNode>> greedy_result_for_filter = greedy_search(vamana_graph, *med, vamana_graph[vamana_graph.size()-1], k, L, 1);

                        vector<GraphNode> k_closest_for_query = greedy_result_for_filter.first;

                        // If kclosest is less than k fill it with dummy nodes
                        if (k_closest_for_query.size() < 100) {

                            for (int l = k_closest_for_query.size() ; l < 100 ; l++) {

                                k_closest_for_query.push_back(&dummy_node);

                            }
                        }

                        int id=-10; 
                        for (int z=0 ; z<gt.size() ; z++) {
                            if (gt[z][0] == j) {
                                id = z;
                                break;
                            }
                        }

                        // Find how many points are same between the algorithms result and the groundtruth                
                        int same_points = vectors_equal(k_closest_for_query, gt[id]); 

                        // Calculate recall
                        float recall = (float)same_points / 100.0;
                        cout  << recall << endl;
                        recall_sum += recall;
                    } 
                }
            }
            
            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using filtered vamana: " << avg_recall << endl;
        }
    }    
    return 0;
}