/*file_operations.hpp*/
#pragma once 

#include <string>
#include <vector>
#include "util.hpp"

using namespace std;

/*Reads a binary file and stores the contents in the vector passed*/
void ReadBin(const string &file_path, const int num_dimensions, vector<vector<float>> &data);



/*Reads the config file ans stores teh contents in the variables passed */
void read_config_file (string CONFIG, string &DATASET, string &QUERIES, string &GRAPH, string &GRAPH_NAME, int &R, int &L, float &a, int &k, int &th, int &L_SMALL, int &R_SMALL ,int &R_STICH, string &method_type, int &ACTUAL_MEDOIDS, int &F0_TRICK);

/*Saves the graph to a .bin file*/
void save_graph(const string &file_path, const vector<graph_node> &graph, GraphNode &med);

void read_graph(const string &file_path, vector<graph_node> &graph, int &med_id);