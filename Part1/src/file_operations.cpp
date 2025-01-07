/*file_operations.cpp*/

#include "../include/file_operations.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include "../include/util.hpp"

using namespace std;



/*Reads a .fvecs file and stores the data in the vector that gets passed as a parameter*/
int read_fvec_files(const string& file_path, vector<vector<float>>& all_points) {
    
    ifstream file1(file_path, ios::binary);
    if (!file1) {
        cout << "Can't open file" << endl;
        return -1;
    }

    while (!file1.eof()) {

        int dimension;

        file1.read(reinterpret_cast<char*>(&dimension), sizeof(int));   //read size of int bytes and place them into dimension

        if(file1.eof()){ 
            break;
        }

        vector<float> point(dimension);

        file1.read(reinterpret_cast<char*>(point.data()), dimension * sizeof(float));

        all_points.push_back(point);
      
    }

    file1.close();
    return 0;

}



/*Reads a .ivecs file and stores the data in the vector that gets passed as a parameter*/
int read_ivec_files(const string& file_path, vector<vector<int>>& all_points) {
    
    ifstream file1(file_path, ios::binary);
    if (!file1) {
        cout << "Can't open file" << endl;
        return -1;
    }

    while (!file1.eof()) {

        int dimension;

        file1.read(reinterpret_cast<char*>(&dimension), sizeof(int));   

        if(file1.eof()) { 
            break;
        }

        vector<int> point(dimension);

        file1.read(reinterpret_cast<char*>(point.data()), dimension * sizeof(float));

        all_points.push_back(point);
      
    }

    file1.close();
    return 0;
    
}

/*Reads the config file and stores teh contents in the variables passed */
void read_config_file (string CONFIG, string &DATASET, string &QUERIES, string &GROUNDTRUTH, string &GRAPH, string &GRAPH_NAME, int &R, int &L, float &a, int &k) {
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
            } else if (name == "GROUNDTRUTH") {
                Line >> GROUNDTRUTH;
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
            }
        }
    }
    //close the file 
    config_file.close();

    return;
}


/*Saves the graph to a .bin file*/
void save_graph(const string &file_path, const vector<graph_node> &graph, GraphNode &medoid) {

    ofstream outFile(file_path, ios::binary);
    // Write the medoid id
    int medoid_id = medoid->id;
    outFile.write(reinterpret_cast<const char*>(&medoid_id), sizeof(int));
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
    }
};

/* 
How the graph is stored in the .bin file:

[(int)medoid_id, (size_t)graph_size, (int)id, (int)num_cords, num_cords x (float)coord, (int)num_edges, num_edges x (int)pos, (double)eucl_dist, (int)num_filters, num_filters x (int)filter ]

*/

/*Reads a vamana_graph*/
void read_graph(const string &file_path, vector<graph_node> &graph, int &medoid_id) {

    ifstream inputFile(file_path, ios::binary);
    inputFile.read((char*)&medoid_id, sizeof(int));
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
    }
};