/*file_operations.cpp*/

#include "../include/file_operations.hpp"
#include <bits/stdc++.h>
#include <iostream>

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
void read_config_file (string &DATASET, string &QUERIES, int &R, int &L, float &a, int &k, int &th, int &L_SMALL, int &R_SMALL ,int &R_STICH, string &method_type) {
    //open file
    ifstream config_file("config.txt");

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
}