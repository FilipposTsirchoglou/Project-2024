#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <forward_list>
#include <ctime>
#include <utility>
#include <queue>


using namespace  std;


typedef struct graph_node* GraphNode; // GraphNode is a pointer to struct graph_node

typedef struct graph_node {
    int id;
    vector<int> cords;
    forward_list<GraphNode> outgoing;               // List of pointers to graph_nodes
}graph_node;


vector<int> vectors_equal(vector<graph_node>& l_closest, vector<graph_node>& visited){
    bool are_same;
    vector<int> unvisited = {};
    for(int i = 0; i < l_closest.size(); i++){
        are_same = false;
        for(int j = 0; j < visited.size(); j++){
            if(l_closest[i].id == visited[j].id){
                are_same = true;
                break;
            }
        }
        if (are_same == false){
            unvisited.push_back(l_closest[i].id);
        }
    }
    return unvisited;
}




pair<vector<graph_node>, vector<graph_node>> greedy_search(vector<graph_node>& graph, graph_node& s, graph_node& xq, int k, int l){
    vector<graph_node> l_closest = {s};
    vector<graph_node> visited = {};
    vector<int> p = vectors_equal(l_closest, visited);
    //priority_queue<GraphNode, vector<GraphNode>, compare> 

    while(!p.empty()){

        vector<float> distance = {};

        for (int i=0 ; i<p.size() ; i++) {
            distance.push_back(euclidean(graph[p[i]].cords, xq.cords));
        }
        

    }
}


int get_random_number(int max){
    static bool seeded = false;

    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    return rand() % (max + 1);
}

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

    /*for(int i = 0; i < graph.size(); i++){
        cout << graph[i].cords[0] <<  ' ' << graph[i].cords[1] << endl;
    }*/

    for(int i = 0; i < graph.size(); i++){
        int counter=0;      // Counter for counting correct outgoing connections for every graph node 
        do{
           int edge_to_id = get_random_number(graph.size() - 1);
           if(edge_to_id != graph[i].id){                   // If the id we want to connect is not the same as the source node id
                if(graph[i].outgoing.empty()){              // If the list is empty do the connection
                    GraphNode edge = &graph[edge_to_id];   
                    graph[i].outgoing.push_front(edge);
                    counter++;
                }
                else{
                    bool flag=true; //Flag that shows the id of the node we want to connect does not already exist
                    for(forward_list<GraphNode>:: iterator it = graph[i].outgoing.begin(); it != graph[i].outgoing.end(); ++it){

                        if((*it)->id == edge_to_id){ // The id already exists 
                            flag=false;
                            break;
                        }
                    }
                    if(flag==true){
                        GraphNode edge = &graph[edge_to_id];   
                        graph[i].outgoing.push_front(edge);
                        counter++;
                    }

                }
           }
        } while(counter !=3);  // 3 is the parameter R. If we did R connections we stop
    }
    for(int i = 0; i < graph.size(); i++){
        cout << "Id: " << graph[i].id << "-->";
        for(forward_list<GraphNode>:: iterator it = graph[i].outgoing.begin(); it != graph[i].outgoing.end(); ++it){
            cout << (*it)->id << " ";
        }
        cout << endl;
    }
    

    return 0;
}