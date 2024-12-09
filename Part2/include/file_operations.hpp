/*file_operations.hpp*/
#pragma once 

#include <string>
#include <vector>

using namespace std;

/*Reads a binary file and stores the contents in the vector passed*/
void ReadBin(const string &file_path, const int num_dimensions, vector<vector<float>> &data);



/*Reads the config file ans stores teh contents in the variables passed */
void read_config_file (string CONFIG, string &DATASET, string &QUERIES, int &R, int &L, float &a, int &k, int &th, int &L_SMALL, int &R_SMALL ,int &R_STICH, string &method_type);