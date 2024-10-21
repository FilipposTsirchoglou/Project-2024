#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <forward_list>
#include <ctime>
#include <utility>
#include <queue>
#include <algorithm>
#include <bits/stdc++.h>
#include <queue>
#include "temp_header.h"

using namespace std;

int main() {

    ifstream file("siftsmall/siftsmall_base.fvecs", ios::binary);

    if (!file) {
        cout << "Can't open file" << endl;
        return -1;
    }

    vector<vector<float>> all_points;

    while (!file.eof()){

        int dimension;

        file.read(reinterpret_cast<char*>(&dimension), sizeof(int));   //read size of int bytes and place them into dimensions

        if(file.eof()) break;

        vector<float> point(dimension);

        file.read(reinterpret_cast<char*>(point.data()), dimension * sizeof(float));

        all_points.push_back(point);
      
    }
    file.close();

    for (int i = 0 ; i<100 ; i++) {
            cout << i << "--> ("; 
        for (int j =0 ; j < all_points[i].size() ; j++) {
            cout << all_points[i][j] << ", "; 
        }
        cout << ")" << endl;
    }
}
