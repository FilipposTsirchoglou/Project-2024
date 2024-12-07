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
#include "include/find_check.hpp" 
#include "include/file_operations.hpp"

using namespace std;

/*void ReadBin(const string &file_path, const int num_dimensions, vector<vector<float>> &data) {
  
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

}*/



int main(void) {

  //Read the configuration file and store its values
  string DATA_FILE, QUERIES_FILE, METHOD_TYPE;
  int L, R, k, th, L_small, R_small, R_stich;
  float a;

  read_config_file(DATA_FILE, QUERIES_FILE, R, L, a, k, th, L_small, R_small, R_stich, METHOD_TYPE);  //method type F0 or F1 means filtered_vamana type 0

  /*dummy node of id -1 (it is a placeholder)*/
  graph_node dummy_node;
  dummy_node.id = -1;
  dummy_node.cords = {};
  dummy_node.filters = {};
  dummy_node.outgoing = {};
  dummy_node.eucl_dist = -1;
  
  vector<vector<float>> nodes;    //all the points of the dataset
  ReadBin("dummy-data.bin", 102, nodes);

  vector<vector<float>> queries;  //all the queries
  ReadBin("dummy-queries.bin", 104, queries);

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

  //sort(filter_set.begin(), filter_set.end());  //now filters are in ascending order


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


  //call vamana to return the vamana indexing graph
  vector<graph_node> vamana_filtered = filteredVamana(graph, a, L, R, filter_set, th, med);

  //groundtruth reading
  vector<vector<float>> gt;
  ReadBin("GroundTruth.bin", 101, gt);    //gt has 101 dimensios. gt[0] is the query id and the rest are the 100 closest points

  float recall_sum = 0;
  int count = 0;
  cout <<"sdjcvsdbchdsc0" << endl;
  for (int i = 0 ; i < queries.size() ; i++) {    //for every query 
  
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
      //gq.filters.push_back(queries[i][1]);      //WHY?

      vamana_filtered.push_back(gq);        //push the graph_node in the graph vector

      vector<GraphNode> s1;
    
      s1.push_back(&vamana_filtered[med[queries[i][1]]]);

     /*for (auto it=med.begin() ; it!=med.end() ; ++it) {
        s1.push_back(&vamana_filtered[it->second]);
      }*/

      /*vector<GraphNode> k_closest_results;
      
      for (int j=0 ; j<filter_set.size() ; j++ ) {
        vector<GraphNode> s_temp;
        s_temp.push_back(&vamana_filtered[med[filter_set[j]]]);
        pair<vector<GraphNode>, vector<GraphNode>> result1 = filteredGreedySearch(vamana_filtered, s_temp, vamana_filtered[vamana_filtered.size() - 1], 1, L);
        k_closest_results.push_back(result1.first[0]);
      }*/
      
      
      pair<vector<GraphNode>, vector<GraphNode>> result = filteredGreedySearch(vamana_filtered, s1, vamana_filtered[vamana_filtered.size() - 1], k, 200);
      vector<GraphNode> k_closest = result.first;
      
      

      vamana_filtered.erase(vamana_filtered.end()-1);

      if (k_closest.size() < 100) {
        for (int j = k_closest.size() ; j < 100 ; j++) {
          k_closest.push_back(&dummy_node);
        }
      }
    
      cout << endl;
     
      int id=-10;
      
      for (int j=0 ; j<gt.size() ; j++) {
        if (gt[j][0] == i) {
          id = j;
          break;
        }
      }
      
      int same_points = vectors_equal(k_closest, gt[id]); 

      float recall = (float)same_points / 100.0;

      cout << "recall for query " << i << " = " << recall << endl;
      if (recall == 1) {
        cout << "FUCK YEA" << endl;
      }
      recall_sum += recall;

    }
  }
  float avg_recall =(float)(recall_sum / count);
  cout << "Average recall for Type 1 queries: " << avg_recall << endl;

  return 0;
}