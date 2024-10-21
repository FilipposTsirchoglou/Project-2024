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

bool equal(vector<float>& v1,vector<float>& v2 ) {
    //cout << "eq" << endl;
    for (int i=0 ; i<v1.size() ; i++) {
        cout << v1[i] << " " << v2[i] << endl;
        if (v1[i] != v2[i]) return false;
    }
 
    return true;
}

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



    ifstream file1("siftsmall/siftsmall_query.fvecs", ios::binary);

    if (!file1) {
        cout << "Can't open file" << endl;
        return -1;
    }

    vector<vector<float>> q_points;

    while (!file1.eof()){

        int dimension;

        file1.read(reinterpret_cast<char*>(&dimension), sizeof(int));   //read size of int bytes and place them into dimensions

        if(file1.eof()) break;

        vector<float> point(dimension);

        file1.read(reinterpret_cast<char*>(point.data()), dimension * sizeof(float));

        q_points.push_back(point);
      
    }
    file1.close();
    cout  << all_points.size();

    
    int count = 0;
    for (int i = 0 ; i<q_points.size() ; i++) {
        bool flag = false;
        for (int j=0 ; j<all_points.size() ; j++) {
            if (equal(q_points[i], all_points[j])) {
                cout << "kk"<<endl;
                flag = true;
                count++;
                break;
            }          
        }
    }

    cout << count << endl;




}