#include "acutest.h"
#include "temp_header.h"
#include <iostream>

using namespace std;


void test_euclidean(){

    vector<float> vec1 = {1,2};
    vector<float> vec2 = {4,2};

    float dist;
    dist = euclidean(vec1, vec2);
    TEST_ASSERT(dist == 3);

    dist = euclidean(vec1, vec1);
    TEST_ASSERT(dist == 0);
}


void test_get_random_number(){
    int max = 20;
    int num = get_random_number(max);
    TEST_ASSERT(num <= max);
    TEST_ASSERT(num >= 0);
}


void test_greedy_search(){

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
    graph.push_back(g9);

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


    pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, graph[0], graph[9], 2, 4);

    TEST_ASSERT((res.first[0] == &graph[8]) && (res.first[1] == &graph[7]));
    /*TEST_ASSERT((res.second[0] == &graph[0]) && 
                (res.second[1] == &graph[1]) && 
                (res.second[2] == &graph[2]) && 
                (res.second[3] == &graph[3]));*/

}


void test_robust_prune(){

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

    graph[9].outgoing.push_front(&graph[8]);
    graph[9].outgoing.push_front(&graph[0]);



    vector<GraphNode> v = {&graph[0], &graph[1], &graph[2], &graph[3], &graph[4], &graph[5], &graph[6], &graph[7], &graph[8]};

    robust_prune(&graph[9], v, 2, 2);
    

    GraphNode node1 = graph[9].outgoing.front();
   
    graph[9].outgoing.pop_front();
    
    GraphNode node2 = graph[9].outgoing.front();
    
    TEST_ASSERT((node1 == &graph[7]) && (node2 == &graph[8]));

}

void test_find_medoid(){

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
    graph.push_back(g4);

    //vector<graph_node> g = {graph[0], graph[1], graph[2], graph[3], graph[4]};


    GraphNode res = find_medoid(graph);
    //cout << "res: " << res << ", " << &g[4] <<endl;

    TEST_ASSERT(res == &graph[4]);
    //return medoid;
}
        

TEST_LIST = {
	{ "test_euclidean", test_euclidean },
	{ "test_get_random_number", test_get_random_number },
    { "test_greedy_search", test_greedy_search},
    { "test_robust_prune", test_robust_prune},
    { "test_find_medoid", test_find_medoid},
	{ NULL, NULL }
};

