/*file_operations.hpp*/
#pragma once 

#include <string>
#include <vector>

using namespace std;

int read_fvec_files(const string&, vector<vector<float>>&);

int read_ivec_files(const string&, vector<vector<int>>&);