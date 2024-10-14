#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(){
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

    cout << "number of points: " << all_points.size()<< endl;
    cout << "1st vector data: ";
    vector<float> first = all_points[0];
    for (int i=0 ; i<all_points[0].size(); i++) {
        cout << first[i] << ", ";
    }
    cout << endl;
    return 0;
}