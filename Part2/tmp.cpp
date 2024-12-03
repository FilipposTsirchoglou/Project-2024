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



map<int, int> filteredFindMedoid(vector<graph_node>& graph, int th, vector<int>& filter_set) {

  map<int, int> M;             // Initializing M to be an empty map

  for (int i = 0 ; i < filter_set.size() ; i++) {

    M[filter_set[i]] = -1;

  }
  
  map<int, int> T;

  for (int i = 0 ; i < graph.size() ; i++) {

    T[i] = 0;                                         // Initializing T to a zero map

  }

  cout << "filter set size: " << filter_set.size() << endl;
  //for (int i = 0)

  for (int i = 0 ; i < filter_set.size() ; i++) {
    
    vector<int> pf = {};        //positions 

    for (int j = 0 ; j < graph.size() ; j++) {

      bool found = false;
      for (int k = 0 ; k < graph[j].filters.size() ; k++) {

        if (graph[j].filters[k] == filter_set[i]) {
          cout << graph[j].filters[k] << " == " << filter_set[i] << endl;

          found = true;
          break;

        }
      }

      if (found) {

        pf.push_back(j);

      }

    }

    vector<int> rf = {};

    do {

      int random_ind = get_random_number(pf.size() - 1);

      if (rf.size() == 0) {                   // If Rf is empty, we surely insert the node with id = random_id

        rf.push_back(pf[random_ind]);

      }
      else {                                  // If Rf is not empty, we have to check if the node with id = random_id already exists

        bool flag = true;                // Flag that shows that the node with id = random_id does not already exist in Rf

        for (int j = 0 ; j < rf.size() ; j++) {
          
          if (rf[j] == pf[random_ind]) {              // The node already exists

            flag = false;
            break;

          }
        }

        if (flag == true) {

          rf.push_back(pf[random_ind]);

        }
      }

    } while (rf.size() < th);     // We stop when Rf has th nodes

    cout << "rf: " << endl;

    int min = 100000;
   
    int p_star = -1;

    for (int i = 0 ; i < rf.size() ; i++) {
      
      if (T[rf[i]] < min) {

        min = T[rf[i]];
        p_star = rf[i];

      }
    }

    M[filter_set[i]] = p_star;
 
    T[p_star]++;

  }

  for (map<int, int>::iterator it = M.begin(); it != M.end(); ++it) {
    std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
  }

  return M;

}



pair<vector<GraphNode>, vector<GraphNode>> filteredGreedySearch(vector<graph_node>& graph, vector<graph_node>& s, graph_node& xq, int k, int l) {
  vector<GraphNode> l_closest = {};
  vector<GraphNode> visited = {};
  
  for (int i = 0 ; i < s.size() ; i++) {

    bool same = false;
    for (int j = 0 ; j < xq.filters.size() ; j++){

      cout << "filter s = " << s[i].filters[0] << " and filter q = " << xq.filters[j] << endl;
      if (s[i].filters[0] == xq.filters[j]){

        same = true;
        break;

      }

    }

    if (same) {

      l_closest.push_back(&s[i]);

    }
  }

  vector<GraphNode> p = vectors_difference(l_closest, visited);  // Computes L-V

  while (!p.empty()) {
    GraphNode p_star = find_min_eucl(p, xq);     // Finds the graph node p_star from all the p that has the min euclidian distance from xq
    visited.push_back(p_star);
    vector<GraphNode> temp_nout = {};
    
    if (!(p_star->outgoing.empty())) {

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
          for (int j=0 ; j < l_closest.size(); j++) {

            if (temp_nout[i] == l_closest[j]){
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



void filteredRobustPrune(GraphNode p, vector<GraphNode>& v, float a, int r){

    // If p is found inside vector v then remove it
    auto found_at_pos = find(v.begin(), v.end(), p);    
    if (found_at_pos != v.end()) {
        v.erase(found_at_pos);
    }
    
    // V U Nout(p)
    for (forward_list<GraphNode>:: iterator it = p->outgoing.begin() ; it != p->outgoing.end() ; ++it) {
        auto cond = find(v.begin(), v.end(), (*it));  // Search the v vector to find if the element at position it of the outgoing list exists
        
        if (cond == v.end()) {      // If you didnt find it         
            v.push_back((*it));     
        }
    }                   // V ← (V ∪ Nout(p)) \ {p}
    
    p->outgoing.clear();    // Nout(p) ← ∅

    while (!v.empty()) {

      GraphNode p_star = find_min_eucl(v, *p); 
      p->outgoing.push_front(p_star);           // Nout(p) ← Nout(p) ∪ {p_star}

      int list_size = distance(p->outgoing.begin(), p->outgoing.end()); 
      if (list_size == r) {
          break;
      }

      for (auto it = v.begin(); it != v.end();) {      // For (p' E V) do
        
        if (((*it)->filters[0] == p->filters[0]) && (p->filters[0] != p_star->filters[0])) {
          
          ++it;
          continue;

        }
        if (a * euclidean(p_star->cords, (*it)->cords) <= euclidean((*it)->cords, p->cords)) {     
                
          v.erase(it);
            
        } else {            // Only increase the iterator if erase didn't happen beacuse of segfault

          ++it;

        }
      }
    }

}


vector<graph_node> filteredVamana(vector<graph_node>& graph, float a, int L, int R, vector<int>& filter_set, int th) {
  
  map<int, int> medoid_pos = filteredFindMedoid(graph, th, filter_set);

  vector<int> sigma = give_random_permutation(graph.size());

  for (int i = 0 ; i < sigma.size() ; i++) {
    
    int start_node = medoid_pos[graph[sigma[i]].filters[0]];

    graph_node node;
   
    vector<graph_node> s = {};
  
    s.push_back(graph[start_node]);

    pair<vector<GraphNode>, vector<GraphNode>> res; 
    res = filteredGreedySearch(graph, s, graph[sigma[i]], 0, L);

    res.first = {};

    filteredRobustPrune(&graph[sigma[i]], res.second, a, R);

    for (forward_list<GraphNode>::iterator it = graph[sigma[i]].outgoing.begin() ; it != graph[sigma[i]].outgoing.end() ; ++it) {

      (*it)->outgoing.push_front(&graph[sigma[i]]);

      int list_size = distance((*it)->outgoing.begin(), (*it)->outgoing.end());
      if (list_size > R) {

        vector<GraphNode> temp_vec = {};
        for (int j = 0 ; j < list_size ; j++) {

          temp_vec.push_back((*it)->outgoing.front());
          (*it)->outgoing.front();
          

        }

        filteredRobustPrune((*it), temp_vec, a, R);

        for (int j = 0 ; j < temp_vec.size() ; j++) {

          (*it)->outgoing.push_front(temp_vec[i]);

        }

      }
    }

  }

  return graph;

}




int main(void) {
  vector<graph_node> s;


  vector<vector<float>> nodes;
  ReadBin("dummy-data.bin", 102, nodes);

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

  map<int,int> m = filteredFindMedoid(graph, 1, filter_set);

  s.push_back(graph[100]);
  pair<vector<GraphNode>, vector<GraphNode>> res = filteredGreedySearch(graph, s, graph[1000], 1, 50);
  //filteredRobustPrune(&graph[1000], res.second, 1.1, 2);

  cout << "visisted size" << res.second.size() << endl; 
  for(int i = 0 ; i < res.second.size() ; i++) {
    cout << res.second[i]->id << endl;
  }

   














 /*cout << "Nodes vectro size: " << nodes.size() << endl;

 vector<int> filter_counter;
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
  cout << "filters: " << filters << endl;
*/
  for(int i = 0; i < filter_set.size(); i++){
    cout << filter_set[i] << endl;
  }

  
  return 0;
}
