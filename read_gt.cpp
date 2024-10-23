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

    ifstream file("siftsmall/siftsmall_groundtruth.ivecs", ios::binary);

    if (!file) {
        cout << "Can't open file" << endl;
        return -1;
    }

    vector<vector<int>> gt_points;
    

    while (!file.eof()){

        int dimension;
        file.read(reinterpret_cast<char*>(&dimension), sizeof(int));   //read size of int bytes and place them into dimensions
        cout << dimension << endl;
        if(file.eof()) break;

        
        vector<int> point(dimension);

        file.read(reinterpret_cast<char*>(point.data()), dimension * sizeof(int));

        gt_points.push_back(point);
      
    }
    file.close();

    
        
    for (int i=0 ; i<gt_points[0].size() ; i++) {
        cout << gt_points[55][i] << endl;;
    }
    cout << endl;
}

