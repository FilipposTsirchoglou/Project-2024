/*file_operations.cpp*/

#include "../include/file_operations.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include "../include/util.hpp"

using namespace std;

/*Reads a binary file and stores the contents in the vector passed*/
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


/*Reads the config file and stores teh contents in the variables passed */
void read_config_file (string CONFIG, string &DATASET, string &QUERIES, string &GRAPH, string &GRAPH_NAME, int &R, int &L, float &a, int &k, int &th, int &L_SMALL, int &R_SMALL ,int &R_STICH, string &method_type) {
    //open file
    ifstream config_file(CONFIG);

    // Read the file line by line
    string line;
    while (getline(config_file, line)) {
        istringstream Line(line);
        string name, equal;
        //parse each line and store the contents
        if (Line >> name >> equal) {

            if (name == "DATASET") {
                Line >> DATASET;
            } else if (name == "QUERIES") {
                Line >> QUERIES;
            } else if (name == "GRAPH") {
                Line >> GRAPH;
            } else if (name == "GRAPH_NAME") {
                Line >> GRAPH_NAME;
            } else if (name == "R") {
                Line >> R;
            } else if (name == "L") {
                Line >> L;
            } else if (name == "a") {
                Line >> a;
            } else if (name == "k") {
                Line >> k;
            } else if (name == "th") {
                Line >> th;
            } else if (name == "L-SMALL") {
                Line >> L_SMALL;
            } else if (name == "R-SMALL") {
                Line >> R_SMALL;
            } else if (name == "R-STICH") {
                Line >> R_STICH;
            } else if (name == "METHOD-TYPE") {
                Line >> method_type;
            }
        }
    }
    //close the file 
    config_file.close();

    return;
};


/*Saves the graph to a .bin file*/
void save_graph(const string &file_path, const vector<graph_node> &graph, GraphNode &med) {

    ofstream outFile(file_path, ios::binary);
    //write the medoid id 
    int med_id = med->id;
    outFile.write(reinterpret_cast<const char*>(&med_id), sizeof(int));
    // Write the number of nodes in the graph   
    size_t graph_size = graph.size(); 
    cout << "Graph size save: " << graph_size << endl;
    outFile.write(reinterpret_cast<const char*>(&graph_size), sizeof(size_t));

    for (int i=0 ; i<graph_size ; i++) {    //for each node

        // Write the id of the node
        int id = graph[i].id;
        outFile.write(reinterpret_cast<const char*>(&id), sizeof(int));

        //write number of coords
        int num_coords = graph[i].cords.size();
        outFile.write(reinterpret_cast<const char*>(&num_coords), sizeof(int));

        //write the coords
        for (int j=0 ; j<num_coords ; j++) {
            float coord = graph[i].cords[j];
            outFile.write(reinterpret_cast<const char*>(&coord), sizeof(float));
        }

        //write the number of edges 
        int num_edges = distance(graph[i].outgoing.begin(), graph[i].outgoing.end());
        outFile.write(reinterpret_cast<const char*>(&num_edges), sizeof(int));

        //write each edge
        for (auto it = graph[i].outgoing.begin() ; it != graph[i].outgoing.end() ; ++it) {
            int pos = (*it)->id;
            outFile.write(reinterpret_cast<const char*>(&pos), sizeof(int));
        }

        //write the euclidian distance
        double eucl_dist = graph[i].eucl_dist;
        outFile.write(reinterpret_cast<const char*>(&eucl_dist), sizeof(double));

        //write the number of filters 
        int num_filters = graph[i].filters.size();
        outFile.write(reinterpret_cast<const char*>(&num_filters), sizeof(int));

        //write each filter
        for (int j=0 ; j<num_filters ; j++) {
            int filter = graph[i].filters[j];
            outFile.write(reinterpret_cast<const char*>(&filter), sizeof(filter));
        }
    }
};

/* 
How the graph is stored in the .bin file:

[(int)med_id (size_t)graph_size, (int)id, (int)num_cords, num_cords x (float)coord, (int)num_edges, num_edges x (int)pos, (double)eucl_dist, (int)num_filters, num_filters x (int)filter ]

*/

/*Reads a vamana_graph*/
void read_graph(const string &file_path, vector<graph_node> &graph, int &med_id) {

    ifstream inputFile(file_path, ios::binary);

    //read the medoid id
    inputFile.read((char*)&med_id, sizeof(int));

    size_t graph_size;
    inputFile.read((char*)&graph_size, sizeof(size_t));

    cout << "Graph size: " << graph_size << endl;
    for (int i=0 ; i<graph_size ; i++) {
        graph_node g;
        g.id=i;
        graph.push_back(g);
    }

    for (int i=0 ; i<graph.size() ; i++) {

        int id;
        inputFile.read((char*)&id, sizeof(int));
        graph[i].id = id;

        int num_coords;
        inputFile.read((char*)&num_coords, sizeof(int));
        for (int j=0 ; j<num_coords ; j++) {
            float coord;
            inputFile.read((char*)&coord, sizeof(float));
            graph[i].cords.push_back(coord);
        }

        int num_edges;
        inputFile.read((char*)&num_edges, sizeof(int));
        for (int j=0 ; j<num_edges ; j++) {
            int pos;
            inputFile.read((char*)&pos, sizeof(int));
            graph[i].outgoing.push_front(&graph[pos]);
        }

        double eucl_dist;
        inputFile.read((char*)&eucl_dist, sizeof(double));
        graph[i].eucl_dist = eucl_dist;

        int num_filters;
        inputFile.read((char*)&num_filters, sizeof(int));
        for (int j=0 ; j<num_filters ; j++) {
            int filter;
            inputFile.read((char*)&filter, sizeof(int));
            graph[i].filters.push_back(filter);
        }
    }

    /*print the graph
    for (int i=0 ; i<graph.size() ; i++) {
        cout << "Node id: " << graph[i].id << endl;
        cout << "Coords: ";
        for (int j=0 ; j<graph[i].cords.size() ; j++) {
            cout << graph[i].cords[j] << " ";
        }
        cout << endl;

        cout << "Edges: ";
        for (auto it = graph[i].outgoing.begin() ; it != graph[i].outgoing.end() ; ++it) {
            cout << (*it)->id << " ";
        }
        cout << endl;

        cout << "Euclidian distance: " << graph[i].eucl_dist << endl;

        cout << "Filters: ";
        for (int j=0 ; j<graph[i].filters.size() ; j++) {
            cout << graph[i].filters[j] << " ";
        }
        cout << endl;
    }*/
};