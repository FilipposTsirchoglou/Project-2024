/*file_operations.hpp*/
#pragma once 

#include <string>
#include <vector>



using namespace std;



/*Reads a .fvecs file and stores the data in the vector that gets passed as a parameter*/
int read_fvec_files(const string&, vector<vector<float>>&);



/*Reads a .ivecs file and stores the data in the vector that gets passed as a parameter*/
int read_ivec_files(const string&, vector<vector<int>>&);



void read_config_file (string &DATASET, string &QUERIES, string &GROUNDTRUTH, int &R, int &L, float &a, int &k);