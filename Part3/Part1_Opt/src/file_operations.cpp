/*file_operations.cpp*/

#include "../include/file_operations.hpp"
#include <bits/stdc++.h>
#include <iostream>

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
void read_config_file (string CONFIG, string &DATASET, string &QUERIES, string &GROUNDTRUTH, int &R, int &L, float &a, int &k, int &RANDOM_MEDOID) {
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
            } else if (name == "R") {
                Line >> R;
            } else if (name == "L") {
                Line >> L;
            } else if (name == "a") {
                Line >> a;
            } else if (name == "k") {
                Line >> k;
            } else if (name == "RANDOM_MEDOID") {
                Line >> RANDOM_MEDOID;
            }
        }
    }
    //close the file 
    config_file.close();

    return;
}
