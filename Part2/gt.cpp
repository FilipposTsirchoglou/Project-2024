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
  
  //cout << "Reading Data: " << file_path << endl;
  ifstream ifs;

  ifs.open(file_path, ios::binary);
  assert(ifs.is_open());
  
  uint32_t N;  // num of points
  
  ifs.read((char *)&N, sizeof(uint32_t));
  
  data.resize(N);
  
  //std::cout << "# of points: " << N << std::endl;
  
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
  //std::cout << "Finish Reading Data" << endl;

}

int main(void) {

    //read an store data in vector data
    vector<vector<float>> gt;

    vector<vector<float>> data;
    ReadBin("dummy-data.bin", 102, data);
    vector<graph_node> graph;

    //find all unique filters and save them in filter_set
    vector<int> filter_set = {};
    for(int i = 0 ; i < data.size() ; i++) {

        bool filter_found = false;
        for(int j = 0 ; j < filter_set.size() ; j++) {

            if (data[i][0] == filter_set[j]) {
                filter_found = true;
                break;
            }
        }
    
        if (filter_found == false) {
            filter_set.push_back(data[i][0]);
        }

        graph_node g;
        g.id = i;

        for(int j = 2 ; j < data[i].size() ; j++) {
            g.cords.push_back(data[i][j]);
        }

        g.eucl_dist = -1;
        g.outgoing = {};
        g.filters.push_back(data[i][0]);
        graph.push_back(g);

    }

    vector<GraphNode> graph_pointers;
    for (int j=0 ; j<graph.size() ; j++) {
        graph_pointers.push_back(&graph[j]);
    }

    //for each filter in filter set, find the data that match the filter


    vector<vector<GraphNode>> points_by_filter;     //vector of vectors of graph nodes with same filter
    //initilaize the vector to have filter_Set.zise() empty vectors
    for (int i=0 ; i < filter_set.size() ; i++) {
        points_by_filter.push_back({});
    }
   // cout << "sdcbscjhsnc";

    //for each filter in filter set, find the data that match the filter and store them in the vector points_by_filter
    for (int i=0 ; i<filter_set.size() ; i++) {

        for (int j=0 ; j<graph.size() ; j++) {

            if (graph[j].filters[0] == i) {

                points_by_filter[i].push_back(&graph[j]);
            }
        }
    }

    //read queries and store them in the vectro queries
    vector<vector<float>> queries;
    ReadBin("dummy-queries.bin", 104, queries);

    for (int i=0 ; i<queries.size() ; i++) {          //for each query
       // cout << "query: " << i << endl << endl;


        //keep the cords of the query in a vector
        vector<float> query_cords;
        for (int k=4 ; k<queries[i].size() ; k++) { 
            query_cords.push_back(queries[i][k]);
        }

        if ( (queries[i][0] == 2) || (queries[i][0] == 3)) continue;

        vector<float> closest;

        if (queries[i][0] == 0) {
            //cout << "type 0" << endl;
            closest.push_back(i);
            //type 0 queries -> find 100 closest without filters

           //calculate the distance between each point of the graph and the query and store it in a graph 
            for (int j=0 ; j<graph.size() ; j++) {
                graph[j].eucl_dist = euclidean(graph[j].cords, query_cords);
            }

            stable_sort(graph_pointers.begin(), graph_pointers.end(), compare_eucl);

            //keep the 100 closest points
            for (int j=0 ; j<graph_pointers.size() ; j++) {
                if (j == 100) break;
                closest.push_back(graph_pointers[j]->id);
            }

            //save closest vector to a binary file 

            gt.push_back(closest);



        } else if (queries[i][0] == 1) {
            //type 1 queries -> find 100 closest with filters
            //cout << "type 1" <<  endl;
            closest.push_back(i);


            int filter = queries[i][1];
            cout << "points by filter size: " << points_by_filter.size() << endl;
            if (filter > points_by_filter.size()) {
                cout << "aaaa filter: " << filter << endl;
                cout << "aiiiii: " << i << endl;
                //vector to of 100 -1 floats
                for (int j=0 ; j<100 ; j++) {
                    closest.push_back(-1);
                }
                gt.push_back(closest);

                continue;
            }
            for (int j=0 ; j < points_by_filter[filter].size() ; j++) {      //for each point that has the same filter as the query
                
                //calculate the euclidean distance between the point and the query and store it in the point's eucl_dist
                points_by_filter[filter][j]->eucl_dist = euclidean(points_by_filter[filter][j]->cords, query_cords);

            }
            stable_sort(points_by_filter[filter].begin(), points_by_filter[filter].end(), compare_eucl);

            //keep the 100 closest points
            int l;
            for (l = 0 ; l<points_by_filter[filter].size() ; l++) {
                if (l == 100) break;
                closest.push_back(points_by_filter[filter][l]->id);
            } 
            if (l<100) {
                for (int j=l ; j<100 ; j++) {
                    closest.push_back(-1);
                }
            }
            //save closest vector to a binary file
            gt.push_back(closest);

        } else {
            cout << "Invalid query type" << endl;
            exit(1);
        }
        //cout << "size: " << closest.size();
    }

    //save to binay file

    ofstream outFile("GroundTruth.bin", ios::binary);
    uint32_t N = gt.size();
    outFile.write(reinterpret_cast<const char*>(&N), sizeof(uint32_t));

   for (int i=0 ; i<gt.size() ; i++) {
        size_t size=gt[i].size();
        outFile.write(reinterpret_cast<const char*>(gt[i].data()), size * sizeof(float));
   }
    outFile.close();
    cout << "writen bin file" << endl;

    cout << "Attempting to read the file" << endl;
    vector<vector<float>> gt2;
    ReadBin("GroundTruth.bin", 101, gt2);
    cout << "gt2 size: " << gt2.size() << endl;

    for (int i=0 ; i<gt2.size() ; i++) {
        cout << "query: " << gt2[i][0] << endl;
        for (int j=1 ; j<gt2[i].size() ; j++) {
            cout << gt[i][j] << " ";
        }
        cout << endl << endl;
    }

    return 0;
}