/*file_operations.cpp*/

#include "../include/file_operations.hpp"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;


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
