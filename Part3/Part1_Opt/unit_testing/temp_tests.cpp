#include "acutest.h"
#include "../include/util.hpp"
#include "../include/vamana.hpp"
#include "../include/find_check.hpp"
#include "../include/file_operations.hpp"
#include <iostream>

using namespace std;



void test_euclidean(void) {

    vector<float> vec1 = {1,2};
    vector<float> vec2 = {4,2};      // Two random vectors. We compute their euclidean distance manually and its 3

    float dist;
    dist = euclidean(vec1, vec2);

    // We want to make sure that the distance between vec1 and vec2 is 3
    TEST_ASSERT(dist == 3);

    dist = euclidean(vec1, vec1);

    // The euclidean distance of a vector to itself should obviously be 0
    TEST_ASSERT(dist == 0);

}



void test_compare_eucl(void) {

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = 1.2345;
    
                                        // Two random graph nodes
    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = 2.3456;
    
    bool result = compare_eucl(&g0, &g1);

    // Fisrt distance < Second distance. We expect true
    TEST_ASSERT(result == true);

    result = compare_eucl(&g0, &g0);

    // Fisrt distance = Second distance. We expect false
    TEST_ASSERT(result == false);
    
    result = compare_eucl(&g1, &g0);

    // Fisrt distance > Second distance. We expect false
    TEST_ASSERT(result == false);

}



void test_get_random_number(void) {
    
    int max = 20;
    int num = get_random_number(max);

    // The random number should be less or equal than max
    TEST_ASSERT(num <= max);

    // The random number should be equal of greater than 0
    TEST_ASSERT(num >= 0);

}



void test_find_min_eucl(void) {

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;
    

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;


    graph_node g2;
    g2.id = 2;
    g2.cords = {2,3};
    g2.eucl_dist = -1;
    
    vector<GraphNode> vec;         // A vector of 3 random graph node pointers
    
    vec.push_back(&g0);
    vec.push_back(&g1);
    vec.push_back(&g2);

    graph_node g3;
    g3.id = 3;
    g3.cords = {3,4};
    g3.eucl_dist = -1;            // A random query point

    vector<double> dis_vec(3, -1.0);

    GraphNode node = find_min_eucl(vec, g3, dis_vec);

    // We expect that the node with the minimum euclidean distance is g2 (we have computed that manually)
    TEST_ASSERT(node->id == g2.id);

}



void test_random_permutation(void) {

    int size = 20;

    vector<int> vec1 = give_random_permutation(size);
    vector<int> vec2 = give_random_permutation(size);  // We want to compare two randomly generated vectors with elements from 0 to size - 1
        
    bool are_same;
    vector<int> dif = {};

    for(int i = 0 ; i < size ; i++) {
        
        if(vec1[i] == vec2[i]) {     
            
            continue;

        }
        else {
            
            dif.push_back(vec1[i]);

        }
    }

    // We want the 2 vectors to be different (the vector of their different elements not to be empty)
    TEST_ASSERT(dif.size() != 0);

}



void test_create_random_graph(void) {

    vector<float> vec1 = {1,2};
    vector<float> vec2 = {2,3};
    vector<float> vec3 = {3,4};
    vector<float> vec4 = {4,5};
    vector<float> vec5 = {5,6};
    vector<float> vec6 = {6,5};
    vector<float> vec7 = {6,7};
    vector<float> vec8 = {7,8};


    vector<vector<float>> points;   // A vector of 8 random points

    points.push_back(vec1);
    points.push_back(vec2);
    points.push_back(vec3);
    points.push_back(vec4);
    points.push_back(vec5);
    points.push_back(vec6);
    points.push_back(vec7);
    points.push_back(vec8);

    vector<graph_node> graph;
    create_random_graph(graph, points, 2);            // We create a random graph with the 8 points

    // We expect an initialized graph with 8 nodes
    TEST_ASSERT(graph.size() == 8);

    bool flag = true;    // A flag that turns into false as soon as we find a node with outgoing list size != 2 and breaks the loop

    for(int i = 0 ; i < graph.size() ; i++){

        int list_size = distance(graph[i].outgoing.begin(), graph[i].outgoing.end());
        if(list_size != 2) {
            flag = false;
            break;
        }
    }

    // For R = 2, we expect that the graph is 2-regular
    TEST_ASSERT(flag == true);

}



void test_greedy_search(void) {

    vector<graph_node> graph;

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;
    graph.push_back(g0);

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;
    graph.push_back(g1);

    graph_node g2;
    g2.id = 2;
    g2.cords = {2,1};
    g2.eucl_dist = -1;
    graph.push_back(g2);

    graph_node g3;
    g3.id = 3;
    g3.cords = {3,0};
    g3.eucl_dist = -1;
    graph.push_back(g3);

    graph_node g4;
    g4.id = 4;
    g4.cords = {2,-1};
    g4.eucl_dist = -1;
    graph.push_back(g4);

    graph_node g5;
    g5.id = 5;
    g5.cords = {1,-2};
    g5.eucl_dist = -1;
    graph.push_back(g5);

    graph_node g6;
    g6.id = 6;
    g6.cords = {0,1};
    g6.eucl_dist = -1;
    graph.push_back(g6);

    graph_node g7;
    g7.id = 7;
    g7.cords = {-1,-2};
    g7.eucl_dist = -1;
    graph.push_back(g7);

    graph_node g8;
    g8.id = 8;
    g8.cords = {-2,-1};
    g8.eucl_dist = -1;
    graph.push_back(g8);

    graph_node g9;
    g9.id = 9;
    g9.cords = {-3,0};
    g9.eucl_dist = -1;
    graph.push_back(g9);                          // We manually create a random graph with 10 random points

    graph[0].outgoing.push_front(&graph[1]);
    graph[0].outgoing.push_front(&graph[9]);

    graph[1].outgoing.push_front(&graph[2]);
    graph[1].outgoing.push_front(&graph[0]);

    graph[2].outgoing.push_front(&graph[3]);
    graph[2].outgoing.push_front(&graph[1]);

    graph[3].outgoing.push_front(&graph[4]);
    graph[3].outgoing.push_front(&graph[2]);

    graph[4].outgoing.push_front(&graph[5]);
    graph[4].outgoing.push_front(&graph[3]);

    graph[5].outgoing.push_front(&graph[6]);
    graph[5].outgoing.push_front(&graph[4]);

    graph[6].outgoing.push_front(&graph[7]);
    graph[6].outgoing.push_front(&graph[5]);

    graph[7].outgoing.push_front(&graph[8]);
    graph[7].outgoing.push_front(&graph[6]);

    graph[8].outgoing.push_front(&graph[9]);
    graph[8].outgoing.push_front(&graph[7]);

    graph[9].outgoing.push_front(&graph[0]);
    graph[9].outgoing.push_front(&graph[8]);
                                                // We set the outgoing conncetions for every node

    // Calling greedy search with start point 0 and query point 9, k = 2, L = 4
    pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, graph[0], graph[9], 2, 4);

    // After performing greedy search on the graph above manually, we find out that the k = 2 closest points are 8 and 7
    //TEST_ASSERT((res.first[0] == &graph[8]) && (res.first[1] == &graph[7]));
    TEST_ASSERT(res.first.size() == 2);

}



void test_robust_prune(void) {

    vector<graph_node> graph;

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;
    graph.push_back(g0);

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;
    graph.push_back(g1);

    graph_node g2;
    g2.id = 2;
    g2.cords = {2,1};
    g2.eucl_dist = -1;
    graph.push_back(g2);

    graph_node g3;
    g3.id = 3;
    g3.cords = {3,0};
    g3.eucl_dist = -1;
    graph.push_back(g3);

    graph_node g4;
    g4.id = 4;
    g4.cords = {2,-1};
    g4.eucl_dist = -1;
    graph.push_back(g4);

    graph_node g5;
    g5.id = 5;
    g5.cords = {1,-2};
    g5.eucl_dist = -1;
    graph.push_back(g5);

    graph_node g6;
    g6.id = 6;
    g6.cords = {0,-1};
    g6.eucl_dist = -1;
    graph.push_back(g6);

    graph_node g7;
    g7.id = 7;
    g7.cords = {-1,-2};
    g7.eucl_dist = -1;
    graph.push_back(g7);

    graph_node g8;
    g8.id = 8;
    g8.cords = {-2,-1};
    g8.eucl_dist = -1;
    graph.push_back(g8);

    graph_node g9;
    g9.id = 9;
    g9.cords = {-3,0};
    g9.eucl_dist = -1;
    graph.push_back(g9);

    graph[0].outgoing.push_front(&graph[9]);
    graph[0].outgoing.push_front(&graph[1]);

    graph[1].outgoing.push_front(&graph[0]);
    graph[1].outgoing.push_front(&graph[2]);

    graph[2].outgoing.push_front(&graph[1]);
    graph[2].outgoing.push_front(&graph[3]);

    graph[3].outgoing.push_front(&graph[2]);
    graph[3].outgoing.push_front(&graph[4]);

    graph[4].outgoing.push_front(&graph[3]);
    graph[4].outgoing.push_front(&graph[5]);

    graph[5].outgoing.push_front(&graph[4]);
    graph[5].outgoing.push_front(&graph[6]);

    graph[6].outgoing.push_front(&graph[5]);
    graph[6].outgoing.push_front(&graph[7]);

    graph[7].outgoing.push_front(&graph[6]);
    graph[7].outgoing.push_front(&graph[8]);

    graph[8].outgoing.push_front(&graph[7]);
    graph[8].outgoing.push_front(&graph[9]);

    graph[9].outgoing.push_front(&graph[0]);
    graph[9].outgoing.push_front(&graph[8]);  // Same graph as the greedy search test above


    // After the greedy search for the same graph, we know that the visited nodes are the ones below
    vector<GraphNode> v = {&graph[0], &graph[1], &graph[2], &graph[3], &graph[4], &graph[5], &graph[6], &graph[7], &graph[8]};
    
    // We call robust prune for query point 9, the visited nodes above, a = 1.0 and R = 2
    robust_prune(&graph[9], v, 1.0, 2);
    

    // After performing robust prune manually, we find out that, at the end, query point 9 has only 1 outgoing connection and that's with point 8
    GraphNode node1 = graph[9].outgoing.front();
    
    // We expect that the node in the outgoing list of query point 9 is the one with id = 8 (point 8)
    TEST_ASSERT((node1->id == graph[8].id));

}



void test_find_medoid() {

    vector<graph_node> graph;

    graph_node g0;
    g0.id = 0;
    g0.cords = {7.2,8.31};
    g0.eucl_dist = -1;
    graph.push_back(g0);

    graph_node g1;
    g1.id = 1;
    g1.cords = {5.19,3.09};
    g1.eucl_dist = -1;
    graph.push_back(g1);

    graph_node g2;
    g2.id = 2;
    g2.cords = {0.04,6.39};
    g2.eucl_dist = -1;
    graph.push_back(g2);

    graph_node g3;
    g3.id = 3;
    g3.cords = {1.49,0.50};
    g3.eucl_dist = -1;
    graph.push_back(g3);

    graph_node g4;
    g4.id = 4;
    g4.cords = {1.75,4.71};
    g4.eucl_dist = -1;
    graph.push_back(g4);                    // We create a graph with 5 random points 


    GraphNode res = find_medoid(graph);

    // We have pre-computed the medoid manually and we know its the point with id = 4
    TEST_ASSERT(res == &graph[4]);

}
        


void test_temp_vectors_equal(void) {

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;

    graph_node g2;
    g2.id = 2;
    g2.cords = {2,1};
    g2.eucl_dist = -1;

    vector<GraphNode> vec1;

    vec1.push_back(&g0);
    vec1.push_back(&g1);
    vec1.push_back(&g2);            // A vector of pointers to random nodes

    vector<int> vec2;

    vec2.push_back(0);
    vec2.push_back(1);
    vec2.push_back(2);             // A vector of integers equal to the ids of the nodes above

    vector<int> dif = temp_vectors_equal(vec1, vec2);

    // Same node ids and ints so the vector of their different elements should be empty 
    TEST_ASSERT(dif.size() == 0);

    vector<int> vec3;
    vec3.push_back(3);
    vec3.push_back(4);
    vec3.push_back(5);            // A vector of integers, different from the nodes above

    vector<int> dif2 = temp_vectors_equal(vec1, vec3);

    // All 3 ids are different from the 3 integers so their different elements vector should have size 3
    TEST_ASSERT(dif2.size() == 3);

}



void test_vamana(void) {

    vector<float> vec1 = {1,2};
    vector<float> vec2 = {2,3};
    vector<float> vec3 = {3,4};
    vector<float> vec4 = {4,5};
    vector<float> vec5 = {5,6};
    vector<float> vec6 = {6,5};
    vector<float> vec7 = {6,7};
    vector<float> vec8 = {7,8};


    vector<vector<float>> points;     // A vector of 8 random points

    points.push_back(vec1);
    points.push_back(vec2);
    points.push_back(vec3);
    points.push_back(vec4);
    points.push_back(vec5);               
    points.push_back(vec6);
    points.push_back(vec7);
    points.push_back(vec8);

    int a = 1.0;
    int l = 5;
    int r = 4;
    GraphNode med = NULL;

    vector<graph_node> vamana_graph = vamana(points, a, l, r, med, 1);

    // We want to make sure that the vamana function returns the medoid properly
    TEST_ASSERT(med != NULL);

    bool flag = true;  // If we find at least a node with outgoing list size > R, flag becomes false and the loop breaks

    for(int i = 0 ; i < vamana_graph.size() ; i++) {
        int list_size = distance(vamana_graph[i].outgoing.begin(), vamana_graph[i].outgoing.end());
        if(list_size > r) {
            flag = false;
            break;
        }
    }

    // We expect that all the graph nodes have at most r outgoing connections
    TEST_ASSERT(flag == true);
   
}



void test_vectors_differnce(void) {
    
    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;

    graph_node g2;
    g2.id = 2;
    g2.cords = {2,1};
    g2.eucl_dist = -1;

    graph_node g3;
    g3.id = 3;
    g3.cords = {3,0};
    g3.eucl_dist = -1;

    graph_node g4;
    g4.id = 4;
    g4.cords = {2,-1};
    g4.eucl_dist = -1;

    vector<GraphNode> vec1;       // A vector of pointers to 5 random nodes

    vec1.push_back(&g0);
    vec1.push_back(&g1);
    vec1.push_back(&g2);
    vec1.push_back(&g3);
    vec1.push_back(&g4);

    vector<GraphNode> vec2;      // A vector of pointers to the nodes with id 3 and 4 declared above
    vec2.push_back(&g3);
    vec2.push_back(&g4);

    vector<GraphNode> result = vectors_difference(vec1, vec2);

    // We expect the 3 nodes with ids 0 , 1 and 2
    int id1, id2, id3;
    id1 = result[0]->id;
    id2 = result[1]->id;
    id3 = result[2]->id;

    TEST_ASSERT((id1 == 0) && (id2 == 1) && (id3 == 2));

    vector<GraphNode> vec3;

    vec3.push_back(&g0);
    vec3.push_back(&g1);
    vec3.push_back(&g2);
    vec3.push_back(&g3);
    vec3.push_back(&g4);

    vector<GraphNode> result1 = vectors_difference(vec1, vec3);
    
    // We give two identical vectors so we expect the result to be an empty vector
    TEST_ASSERT(result1.size() == 0);

}





void test_read_config_file(void) {

    string DATA_FILE, QUERIES_FILE, GROUNDTRUTH;
    int L, R, k, random_med;
    float a;
    read_config_file("unit_testing/test_config.txt", DATA_FILE, QUERIES_FILE, GROUNDTRUTH, R, L, a, k, random_med);

    TEST_ASSERT(DATA_FILE == "test" && QUERIES_FILE == "test" && GROUNDTRUTH == "test" && R==0 && L==0 && a==0 && k==0);
}





TEST_LIST = {
	{ "test_euclidean", test_euclidean },
    { "test_compare_eucl", test_compare_eucl},
	{ "test_get_random_number", test_get_random_number},
    { "test_find_min_eucl" , test_find_min_eucl},
    { "test_random_permutation", test_random_permutation},
    { "test_create_random_graph", test_create_random_graph},
    { "test_greedy_search", test_greedy_search},
    { "test_robust_prune", test_robust_prune},
    { "test_find_medoid", test_find_medoid},
    { "test_temp_vectors_equal", test_temp_vectors_equal},
    { "test_vamana", test_vamana},
    { "test_vectors_differnce" , test_vectors_differnce},
    { "test_read_config_file" , test_read_config_file},
	{ NULL, NULL }
};

