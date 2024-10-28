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

    while (!file1.eof()){

        int dimension;

        file1.read(reinterpret_cast<char*>(&dimension), sizeof(int));   //read size of int bytes and place them into dimensions

        if(file1.eof()) break;

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

    while (!file1.eof()){

        int dimension;

        file1.read(reinterpret_cast<char*>(&dimension), sizeof(int));   //read size of int bytes and place them into dimensions

        if(file1.eof()) break;

        vector<int> point(dimension);

        file1.read(reinterpret_cast<char*>(point.data()), dimension * sizeof(float));

        all_points.push_back(point);
      
    }
    file1.close();
    return 0;
}
