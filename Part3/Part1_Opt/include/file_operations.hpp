/*file_operations.hpp*/
#pragma once 

#include <string>
#include <vector>



using namespace std;



/*Reads a .fvecs file and stores the data in the vector that gets passed as a parameter*/
int read_fvec_files(const string&, vector<vector<float>>&);



/*Reads a .ivecs file and stores the data in the vector that gets passed as a parameter*/
int read_ivec_files(const string&, vector<vector<int>>&);


/*Reads the config file and stores teh contents in the variables passed */
void read_config_file (string, string &, string &, string &, int &, int &, float &, int &, int &);

