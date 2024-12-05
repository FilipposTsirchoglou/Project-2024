#include <iostream>
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

using namespace std;

void ReadBin(const string &file_path, const int num_dimensions, vector<vector<float>> &data) {
  
  cout << "Reading Data: " << file_path << endl;
  ifstream ifs;

  ifs.open(file_path, ios::binary);
  assert(ifs.is_open());
  
  uint32_t N;  // num of points
  
  ifs.read((char *)&N, sizeof(uint32_t));
  
  data.resize(N);
  
  std::cout << "# of points: " << N << std::endl;
  
  std::vector<float> buff(num_dimensions);
  
  int counter = 0;
  
  while (ifs.read((char *)buff.data(), num_dimensions * sizeof(float))) {
    std::vector<float> row(num_dimensions);
    
    for (int d = 0; d < num_dimensions; d++) {
      row[d] = static_cast<float>(buff[d]);
    }
    
    data[counter++] = std::move(row);
  }

  ifs.close();
  std::cout << "Finish Reading Data" << endl;

}



int main(void) {

  vector<vector<float>> nodes;
  ReadBin("dummy-data.bin", 102, nodes);
  vector<vector<float>> queries;
  ReadBin("dummy-queries.bin", 104, queries);

  vector<graph_node> graph;

  vector<int> filter_set = {};

  for(int i = 0 ; i < nodes.size() ; i++) {

    bool filter_found = false;
    for(int j = 0 ; j < filter_set.size() ; j++) {

      if (nodes[i][0] == filter_set[j]) {

        filter_found = true;
        break;

      }

    }
    
    if (filter_found == false) {

      filter_set.push_back(nodes[i][0]);

    }
    
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

  vector<graph_node> vamana_filtered = filteredVamana(graph, 1.0, 150, 12, filter_set, 1);
 
  vector<GraphNode> s;
  s.push_back(&graph[6326]);

  graph_node gq;
  gq.id = graph.size();
  for(int j = 4 ; j < queries[1].size() ; j++) {
    gq.cords.push_back(queries[1][j]);
  }
  gq.eucl_dist = -1;
  gq.outgoing = {};
  gq.filters.push_back(queries[1][1]);
  
  graph.push_back(gq);
  
  pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(vamana_filtered, s, graph[graph.size() - 1], 100, 150);
  
  cout << "Result size = ";
  cout << result.first.size() << endl;
  for(int i = 0 ; i < result.first.size() ; i++){
    //cout << i << endl;
    if (result.first[i] == NULL){
      cout << "error " << endl;
    }
    else {
      cout << result.first[i]->id << " , ";
    }  
  }
  cout << endl;

  return 0;
}