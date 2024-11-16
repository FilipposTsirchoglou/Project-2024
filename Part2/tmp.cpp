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


pair<vector<GraphNode>, vector<GraphNode>> filteredGreedySearch(vector<graph_node>& graph, vector<graph_node>& s, graph_node& xq, int k, int l) {
  vector<GraphNode> l_closest = {};
  vector<GraphNode> visited = {};
  
  for (int i = 0; i < s.size(); i++) {
    //if () {
    l_closest.push_back(&s[i]);
  }

  vector<GraphNode> p = vectors_difference(l_closest, visited);  // Computes L-V

  while (!p.empty()) {
    GraphNode p_star = find_min_eucl(p, xq);     // Finds the graph node p_star from all the p that has the min euclidian distance from xq
    visited.push_back(p_star);
    vector<GraphNode> temp_nout = {};
    
    if(!(p_star->outgoing.empty())) {

      for (forward_list<GraphNode>::iterator it = p_star->outgoing.begin(); it != p_star->outgoing.end(); ++it) {
        
        auto cond = find(visited.begin(), visited.end(), (*it));    // Search if the elements inside the outgoing list of p_star already exist in the visited vector
        
        if ((cond == visited.end()) && (*it)->id != xq.id) {      /* SOS CHECK it->id != xq.id */     // If the element is not already in the visited list && the element is not the same as the xq element(we dont want to compute the euclidian distance between a point and its self)
          
          (*it)->eucl_dist = euclidean((*it)->cords, xq.cords);       // Re-compute in case of eucl_dist = -1
          
          for (int i = 0 ; i < (*it)->filters.size() ; i++) {   /*    make function bool found_common_filter SOSOSOSOS                 */
            for (int j = 0; j < xq.filters.size(); j++) {
              
              if ((*it)->filters[i] == xq.filters[j]) {
                
                temp_nout.push_back((*it));
                break;

              }
            }                                                               
          }     
        }
      }
      for (int i = 0 ; i < temp_nout.size() ; i++) {
        bool found = false;
          for (int j=0 ; j<l_closest.size(); j++) {

            if(temp_nout[i] == l_closest[j]){
              found = true;
              break;
            }
          }
          if (!found) {
            l_closest.push_back(temp_nout[i]);
          }
      }
    }
    if (l_closest.size() > l) {
            
      sort(l_closest.begin(), l_closest.end(), compare_eucl);     // Sorts the l_closest vector using a custom comparator function 
      l_closest.resize(l);        // Keep the first L elements of the vector
    }
        
    p = vectors_difference(l_closest, visited);      // Recompute L-V before the condition is checked  in the while loop
  }

  l_closest.resize(k);        // Keeps the k closest elements of the vector that we want to return
  return {l_closest, visited};
}



int main(void) {
  vector<graph_node> s;


  vector<vector<float>> nodes;
  ReadBin("dummy-data.bin", 102, nodes);

  vector<graph_node> graph;

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

  s.push_back(graph[100]);
  pair<vector<GraphNode>, vector<GraphNode>> res = filteredGreedySearch(graph, s, graph[1000], 1, 50);

  cout << "visisted size" << res.second.size() << endl; 
  for (int i = 0 ; i < res.second.size() ; i++) {
    cout << res.second[i]->id << endl;
  }

   














 //cout << "Nodes vectro size: " << nodes.size() << endl;

 /*vector<int> filter_counter;
  for (int i=0 ; i<nodes.size() ; i++ ) {
    filter_counter.push_back(0);
  }



  for (int i=0 ; i<nodes.size(); i++) {
    filter_counter[nodes[i][0]]++;
  }

  int filters=0;

  for (int i=0 ; i<filter_counter.size(); i++) {
    if (filter_counter[i] == 0){
      continue;
    } else {
      filters++;
    }
    cout << "filter " << i << " exists " << filter_counter[i] << "times" << endl;
  }
  cout << "filters: " << filters << endl;*/

  
  return 0;
}
