#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <forward_list>
#include <ctime>

using namespace  std;

typedef struct graph_node* GraphNode; // GraphNode is a pointer to struct graph_node

typedef struct graph_node {
    int id;
    vector<int> cords;
    forward_list<GraphNode> outgoing;               // List of pointers to graph_nodes
}graph_node;



float euclidean(const vector<int>& p1, const vector<int>& p2){
    
    float dist = 0.0;

    for(int i = 0; i < p1.size(); i++){

        int temp = p1[i] - p2[i];
        dist += temp * temp;
    }

    return sqrt(dist);
}

int main(void){

    vector<graph_node> graph;

    ifstream file("inputfile");

    if(!file.is_open()){
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int c1, c2;
    int i = 0;

    while(file >> c1 >> c2){
        graph_node g;
        vector<int> cords;
        cords.push_back(c1);
        cords.push_back(c2);                  // Getting the vector cords
        g.id = i;
        g.cords = cords;
        g.outgoing = {};                      // Initializing a graph node
        graph.push_back(g);
        i++;
    }
    file.close();
    
    return 0;
}
