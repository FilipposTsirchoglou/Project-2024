/*main.cpp*/

#include <iostream>
#include "../include/util.hpp"
#include "../include/find_check.hpp"
#include "../include/vamana.hpp"
#include "../include/file_operations.hpp"
#include <chrono>

using namespace  std;
using namespace std::chrono;

/*MAIN FUNCTION*/
int main(void){

    string DATA, QUERIES, GROUNDTRUTH;
    int R, L, k, RANDOM_MEDOID;
    float a;
    read_config_file("config.txt", DATA, QUERIES, GROUNDTRUTH, R, L, a, k, RANDOM_MEDOID);
    chrono::high_resolution_clock::time_point start, stop;
    
    /*Read the base file and store the data in the vector all_points*/
    vector<vector<float>> all_points;
    int flag = read_fvec_files(DATA, all_points);
    if (flag == -1) {
        return -1;
    }

    GraphNode med = NULL;
    
    /*Vamana handles the creation of the vamana indexing graph*/
    start = high_resolution_clock::now();
    vector<graph_node> vamana_graph = vamana(all_points, a, L, R, med, RANDOM_MEDOID);
    stop = high_resolution_clock::now();

    /*Reads the file with the queries and stores the data in the q_points vector*/
    vector<vector<float>> q_points;
    flag = read_fvec_files(QUERIES, q_points);
    if (flag == -1) {
        return -1;
    }

    /*Reads the file with the groundtruth and stores the data in the gt_points vector*/
    vector<vector<int>> gt_points;
    flag = read_ivec_files(GROUNDTRUTH, gt_points);
    if (flag == -1) {
        return -1;
    }

    string input;
    


    /*Finds k closest for random single query points and checks the recall based on the groundtruth file*/
    vector<GraphNode> my_k_closest = find_k_closest_for_single_query_point(vamana_graph, q_points[12], k, L, *med);
    float recall = check_groundtruth_for_single_query_point(my_k_closest, gt_points[12]);
    cout << "recall: " << recall << endl;

    my_k_closest = find_k_closest_for_single_query_point(vamana_graph, q_points[50], k, L, *med);
    recall = check_groundtruth_for_single_query_point(my_k_closest, gt_points[50]);
    cout << "recall: " << recall << endl;

    my_k_closest = find_k_closest_for_single_query_point(vamana_graph, q_points[76], k, L, *med);
    recall = check_groundtruth_for_single_query_point(my_k_closest, gt_points[76]);
    cout << "recall: " << recall << endl;

    my_k_closest = find_k_closest_for_single_query_point(vamana_graph, q_points[4], k, L, *med);
    recall = check_groundtruth_for_single_query_point(my_k_closest, gt_points[4]);
    cout << "recall: " << recall << endl;

    /*Finds k closest for all query points and returns the avergae recall*/
    auto start_queries = high_resolution_clock::now();
    vector<vector<GraphNode>> my_k_closest_vec = find_k_closest_for_all_query_points(vamana_graph, q_points, k, L, *med);
    auto stop_queries = high_resolution_clock::now();

    recall = check_groundtruth_for_all_query_points(my_k_closest_vec, gt_points);
    cout << "average recall: " << recall << endl;

    auto duration = duration_cast<seconds>(stop - start);
    cout << "Index creation: " << duration.count() << " seconds" << endl;

    auto duration_queries = duration_cast<microseconds>(stop_queries - start_queries);
    cout << "Queries: " << (duration_queries.count())<< " miliseconds" << endl;
    cout << "Queries per milisecond: " << q_points.size() / double(duration_queries.count())<< endl;

    return 0;
}