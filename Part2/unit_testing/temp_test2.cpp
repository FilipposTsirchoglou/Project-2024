#include "acutest.h"
#include "../include/util.hpp"
#include "../include/vamana.hpp"
#include "../include/find_check.hpp"
#include <iostream>
#include <map>

using namespace std;



void test_filteredFindMedoid(void) {

    vector<graph_node> graph;

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;
    g0.filters.push_back(0);
    graph.push_back(g0);

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;
    g1.filters.push_back(0);
    graph.push_back(g1);

    graph_node g2;
    g2.id = 2;
    g2.cords = {2,1};
    g2.eucl_dist = -1;
    g2.filters.push_back(1);
    graph.push_back(g2);

    graph_node g3;
    g3.id = 3;
    g3.cords = {3,0};
    g3.eucl_dist = -1;
    g3.filters.push_back(1);
    graph.push_back(g3);

    graph_node g4;
    g4.id = 4;
    g4.cords = {2,-1};
    g4.eucl_dist = -1;
    g4.filters.push_back(2);
    graph.push_back(g4);

    graph_node g5;
    g5.id = 5;
    g5.cords = {1,-2};
    g5.eucl_dist = -1;
    g5.filters.push_back(2);
    graph.push_back(g5);

    graph_node g6;
    g6.id = 6;
    g6.cords = {0,1};
    g6.eucl_dist = -1;
    g6.filters.push_back(3);
    graph.push_back(g6);

    graph_node g7;
    g7.id = 7;
    g7.cords = {-1,-2};
    g7.eucl_dist = -1;
    g7.filters.push_back(3);
    graph.push_back(g7);

    graph_node g8;
    g8.id = 8;
    g8.cords = {-2,-1};
    g8.eucl_dist = -1;
    g8.filters.push_back(4);
    graph.push_back(g8);

    graph_node g9;
    g9.id = 9;
    g9.cords = {-3,0};
    g9.eucl_dist = -1;
    g9.filters.push_back(4);
    graph.push_back(g9);

    vector<int> filter_set = {0, 1, 2, 3, 4};

    map<int,int> m = filteredFindMedoid(graph, 1, filter_set);

    TEST_ASSERT(m.size() == 5);

}




void test_filteredGreedySearch(void) {

    vector<graph_node> graph;

    graph_node g0;
    g0.id = 0;
    g0.cords = {0,1};
    g0.eucl_dist = -1;
    g0.filters.push_back(0);
    graph.push_back(g0);

    graph_node g1;
    g1.id = 1;
    g1.cords = {1,2};
    g1.eucl_dist = -1;
    g1.filters.push_back(1);
    graph.push_back(g1);

    graph_node g2;
    g2.id = 2;
    g2.cords = {2,1};
    g2.eucl_dist = -1;
    g2.filters.push_back(2);
    graph.push_back(g2);

    graph_node g3;
    g3.id = 3;
    g3.cords = {3,0};
    g3.eucl_dist = -1;
    g3.filters.push_back(3);
    graph.push_back(g3);

    graph_node g4;
    g4.id = 4;
    g4.cords = {2,-1};
    g4.eucl_dist = -1;
    g4.filters.push_back(4);
    graph.push_back(g4);

    vector<graph_node> s;
    s.push_back(graph[0]);

    pair<vector<GraphNode>, vector<GraphNode>> res = filteredGreedySearch(graph, s, graph[4], 1, 2);

    // In this case, start and query have different filter so we expect no neighbours or visited nodes
    TEST_ASSERT(res.first.size() == 0);
    TEST_ASSERT(res.second.size() == 0);

    graph[4].filters[0] = 0;

    res = filteredGreedySearch(graph, s, graph[4], 1, 2);

    // In this case, start and query have the same filter so the start node will be the only neighbour and the only visited node
    TEST_ASSERT(res.first.size() == 1);
    TEST_ASSERT(res.second.size() == 1);

}


TEST_LIST = {

    {"test_filteredFindMedoid", test_filteredFindMedoid},
	{"test_filteredGreedySearch", test_filteredGreedySearch},
	{ NULL, NULL }

};