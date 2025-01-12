#include <iostream>
#include "../include/util.hpp"
#include "../include/find_check.hpp"
#include "../include/vamana.hpp"
#include "../include/file_operations.hpp"
#include <unistd.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(void) {

    /*dummy node of id -1 (it is a placeholder)*/
    graph_node dummy_node;
    dummy_node.id = -1;
    dummy_node.pos = -1;
    dummy_node.cords = {};
    dummy_node.filters = {};
    dummy_node.outgoing = {};
    dummy_node.eucl_dist = -1;

    //Read the configuration file and store its values
    string DATA_FILE, QUERIES_FILE, GRAPH, GRAPH_NAME, METHOD_TYPE;
    int L, R, k, th, L_small, R_small, R_stich, ACTUAL_MEDOIDS, F0_TRICK;
    float a;
    chrono::high_resolution_clock::time_point start, stop , start_query, stop_query;

    read_config_file("config.txt", DATA_FILE, QUERIES_FILE, GRAPH, GRAPH_NAME, R, L, a, k, th, L_small, R_small, R_stich, METHOD_TYPE, ACTUAL_MEDOIDS, F0_TRICK);  //method type F0 or F1 means filtered_vamana type 0 or 1 (same as S0,S1 for stitced)
    cout << METHOD_TYPE << endl;
    cout << "DATA_FILE: " << DATA_FILE << " QUERIES_FILE: " << QUERIES_FILE << "GRAPH: " << GRAPH << "GRAPH_NAME: " << GRAPH_NAME << " R: " << R << " L: " << L << " a: " << a << " k: " << k << " th: " << th << " L_small: " << L_small << " R_small: " << R_small << " R_stich: " << R_stich << " method_type: " << METHOD_TYPE << "actual_medoids " << ACTUAL_MEDOIDS<< endl;

    vector<vector<float>> nodes;    //all the points of the dataset
    ReadBin(DATA_FILE, 102, nodes);

    vector<vector<float>> queries;  //all the queries
    ReadBin(QUERIES_FILE, 104, queries);

    vector<vector<float>> gt;      //groundtruth
    ReadBin("GroundTruth.bin", 101, gt);    //gt has 101 dimensios. gt[0] is the query id and the rest are the 100 closest points

    vector<graph_node> graph;
    vector<int> filter_set = find_filter_set(nodes);

    /*Graph Creation*/
    start = chrono::high_resolution_clock::now();
    for(int i = 0 ; i < nodes.size() ; i++) {
        graph_node g;
        g.id = i;
        g.pos = i;

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

        if (GRAPH == "NO") {
            /*Creates a new graph and saves it to a .bin file*/
            filteredVamana(graph, a, L, R, filter_set, th, med);
            stop = chrono::high_resolution_clock::now();
            save_graph(GRAPH_NAME, graph, dummy_med);
        } else if (GRAPH == "YES") {
            /*Reads the graph from a .bin file*/
            read_graph(GRAPH_NAME, graph, med_id);
        }

        int count = 0;
        float recall_sum = 0;
        if (METHOD_TYPE == "F1") {
            start_query = chrono::high_resolution_clock::now();
            for (int i=0 ; i<queries.size() ; i++) {

                if (queries[i][0] == 1) {     // Type 1
                    cout << "query: " << i <<endl;
                    count++;
            
                    graph_node gq;              // Create a temporary graph_node
                    gq.id = graph.size();
                    for(int j = 4 ; j < queries[i].size() ; j++) {
                        gq.cords.push_back(queries[i][j]);
                    }

                    gq.eucl_dist = -1;
                    gq.outgoing = {};

                    gq.filters.push_back(queries[i][1]);

                    graph.push_back(gq);        // Push the graph_node in the graph vector

                    vector<int> s1;
                    int query_filter = queries[i][1];
                    s1.push_back(med[query_filter]);                

                    // Run the filtered greedy search to find k closest neighbours
                    pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(graph, s1, graph[graph.size() - 1], k, 200);
                    vector<GraphNode> k_closest = result.first;
                
                    graph.erase(graph.end()-1); //erase the temp graph_node

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
            stop_query = chrono::high_resolution_clock::now();

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 1 queries using filtered vamana: " << avg_recall << endl;

            auto duration = duration_cast<seconds>(stop - start);
            cout << "Index creation: " << duration.count() << " seconds" << endl;

            auto duration_queries = duration_cast<seconds>(stop_query - start_query);
            cout << "Queries: " << (duration_queries.count())<< " seconds" << endl;
            cout << "Queries per second: " << queries.size() / (double)(duration_queries.count())<< endl;

        } else if (METHOD_TYPE == "F0") {
            start_query = chrono::high_resolution_clock::now();
            for (int i=0 ; i<queries.size() ; i++) {

                if (queries[i][0] == 0) {     // Type 0
                    //cout << "query: " << i <<endl;
                    count++;
                    
                    // Create and initialize a temmporary graph node
                    graph_node gq;              
                    gq.id = graph.size();
                    for(int j = 4 ; j < queries[i].size() ; j++) {
                        gq.cords.push_back(queries[i][j]);
                    }
                    gq.eucl_dist = -1;
                    gq.outgoing = {};
                    for (int k=0 ; k<filter_set.size() ; k++) {
                        gq.filters.push_back(filter_set[k]);
                    }      
                    graph.push_back(gq);        // Push the graph_node in the graph vector

                    vector<int> s1;
                    
                    // Trick to achieve better recall in filtered queries
                    for (auto it=med.begin() ; it!=med.end() ; ++it) {
                        s1.push_back(it->second);
                    }
                    
                    vector<GraphNode> k_closest;
                    if (F0_TRICK == 0) {
                        pair<vector<GraphNode>, vector<GraphNode>> result1 = filteredGreedySearch(graph, s1, graph[graph.size() - 1], k, L);
                        k_closest = result1.first;
                    } else {
                        vector<int> k_closest_results_id;
        
                        for (int j=0 ; j<filter_set.size() ; j++ ) {
                            vector<int> s_temp;
                            s_temp.push_back(med[filter_set[j]]);
                            pair<vector<GraphNode>, vector<GraphNode>> result1 = filteredGreedySearch(graph, s_temp, graph[graph.size() - 1], 1, L);
                            k_closest_results_id.push_back(result1.first[0]->id);
                        }          

                        // Run the filtered greedy search to find k closest neighbours
                        pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(graph, k_closest_results_id, graph[graph.size() - 1], k, L*2);
                        k_closest = result.first;
                    }
                
                    graph.erase(graph.end()-1);

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
            stop_query = chrono::high_resolution_clock::now();

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using filtered vamana: " << avg_recall << endl;

            auto duration = duration_cast<seconds>(stop - start);
            cout << "Index creation: " << duration.count() << " seconds" << endl;

            auto duration_queries = duration_cast<seconds>(stop_query - start_query);
            cout << "Queries: " << (duration_queries.count())<< " seconds" << endl;
            cout << "Queries per second: " << queries.size() / (double)(duration_queries.count())<< endl;
        }  
    } else if (METHOD_TYPE == "S0" || METHOD_TYPE == "S1") {
            chrono :: high_resolution_clock::time_point start_s0, stop_s0;
        
        if (METHOD_TYPE == "S0") {

            int count = 0;
            float recall_sum = 0;
            
            //vector<vector<float>> all_points;
            vector<int> nothing;
            for (int i = 0 ; i < graph.size() ; i++) {
                
                //all_points.push_back(graph[i].cords);
                nothing.push_back(i);
                
            }

            GraphNode med = NULL;
            //vector<graph_node> stitched_vamana_graph;
            int med_id = -1;
   
            if (GRAPH == "NO") {
                start_s0 = chrono::high_resolution_clock::now();
                vamana(a, L, R, med, graph);
                stop_s0 = chrono::high_resolution_clock::now();
                save_graph(GRAPH_NAME, graph, med);
            } else if (GRAPH == "YES") {
                read_graph(GRAPH_NAME, graph, med_id);
                med=&graph[med_id];
                //med = find_medoid(stitched_vamana_graph);
            }
            //med = find_medoid(stitched_vamana_graph);
            // At this point the stitched vamana has been created
            start_query = chrono::high_resolution_clock::now();
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

                    graph.push_back(gq);
                    
                    int id_temp = gq.id;


                    pair<vector<GraphNode>, vector<GraphNode>> greedy_result_for_filter = greedy_search(graph, *med, graph[graph.size()-1], k, L, 0);
                    
                    vector<GraphNode> k_closest_for_query = greedy_result_for_filter.first;
                    
                    graph.erase(graph.end()-1);

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
            stop_query = chrono::high_resolution_clock::now();

            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 0 queries using stitched vamana: " << avg_recall << endl;

            auto duration = duration_cast<seconds>(stop_s0 - start_s0);
            cout << "Index creation: " << duration.count() << " seconds" << endl;

            auto duration_queries = duration_cast<seconds>(stop_query - start_query);
            cout << "Queries: " << (duration_queries.count())<< " seconds" << endl;
            cout << "Queries per second: " << queries.size() / (double)(duration_queries.count())<< endl;
        }
        else if (METHOD_TYPE == "S1") {
            
            int count = 0;
            float recall_sum = 0;
            vector<vector<graph_node>> stich_vamana_graph;

            for (int i=0 ; i<4 ; i++) {
                vector<graph_node> graph_temp;
                for (int j=0 ; j<nodes.size() ; j++) {
                    if (filter_set[i] == nodes[j][0]) {
                        graph_temp.push_back(graph[j]);
                    }
                }
                for (int j=0 ; j<graph_temp.size() ; j++) {
                    graph_temp[j].pos = j;
                }

                int rn = get_random_number(graph_temp.size()-1);
                GraphNode med = &graph_temp[rn];
                vamana(a, L, R, med, graph_temp);

                for (int j=0 ; j<queries.size() ; j++) {
                    
                    if (queries[j][0] == 1 && queries[j][1] == filter_set[i]) {
                        cout << "query: " << j << endl;
                        cout << "filter: " << filter_set[i] << endl;
                        
                        graph_node gq;
                        gq.id = graph.size();
                        gq.pos = graph_temp.size();
                        for (int k=4 ; k<queries[j].size() ; k++) {
                            gq.cords.push_back(queries[j][k]);
                        }
                        gq.eucl_dist = -1;
                        gq.outgoing = {};
                        gq.filters.push_back(filter_set[i]);

                        graph_temp.push_back(gq);
                        rn = get_random_number(graph_temp.size()-1);
                        GraphNode med_temp = &graph_temp[rn];
                        pair<vector<GraphNode>, vector<GraphNode>> result = greedy_search(graph_temp, *med_temp, graph_temp[graph_temp.size()-1], k, L, 0);
                        vector<GraphNode> k_closest_for_query = result.first;
                        
                        // Search the id of the corresponding query in the groundtruth
                        int id=-10;                
                        for (int k=0 ; k<gt.size() ; k++) {

                            if (gt[k][0] == j) {

                                id = k;
                                break;

                            }
                        }
                        int same_points = vectors_equal(k_closest_for_query, gt[id]);

                        float  recall = (float)same_points / 100.0;
                        recall_sum += recall;
                        count++;
                    }

                }
                stich_vamana_graph.push_back(graph_temp);
            }
            float avg_recall =(float)(recall_sum / count);
            cout << "Average recall for Type 1 queries using stitched vamana: " << avg_recall << endl;
        }
    }    
    return 0;
}