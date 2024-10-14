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

using namespace  std;

/*STRUCTS*/
typedef struct graph_node* GraphNode; // GraphNode is a pointer to struct graph_node

typedef struct graph_node {
    int id;
    vector<float> cords;
    forward_list<GraphNode> outgoing = {};               // List of pointers to graph_nodes
    float eucl_dist = -1.0;
}graph_node;

struct CompareDistance {
    bool operator()(GraphNode p1, GraphNode p2) {
        // Return true if p1 has higher distance, so it has lower priority
        return p1->eucl_dist > p2->eucl_dist;
    }
};

/*FUNCTION PROTOTYPES*/
vector<GraphNode> vectors_equal(vector<GraphNode>&, vector<GraphNode>&);

float euclidean(const vector<int>& , const vector<int>& );

bool compare_eucl(GraphNode, GraphNode);

int get_random_number(int);

pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>&, graph_node&, graph_node&, int, int);


/*FUNCTION CODE*/
vector<GraphNode> vectors_equal(vector<GraphNode>& l_closest, vector<GraphNode>& visited){  
    bool are_same;
    vector<GraphNode> unvisited = {};

    for(int i = 0; i < l_closest.size(); i++){
        are_same = false;
        for(int j = 0; j < visited.size(); j++){
            if(l_closest[i] == visited[j]){     
                are_same = true;
                break;
            }
        }
        if (are_same == false){
            unvisited.push_back(l_closest[i]);
        }
    }
    return unvisited;
}


float euclidean(const vector<float>& p1, const vector<float>& p2){    
    float dist = 0.0;

    for(int i = 0; i < p1.size(); i++){
        int temp = p1[i] - p2[i];
        dist += temp * temp;
    }

    return sqrt(dist);
}


bool compare_eucl(GraphNode g1, GraphNode g2){
    return g1->eucl_dist < g2->eucl_dist;
}


int get_random_number(int max){
    static bool seeded = false;

    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    return rand() % (max + 1);
}


int create_random_graph(vector<graph_node>& graph, int r){
    /*ifstream file("inputfile");

    if(!file.is_open()){
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int c1, c2;
    int i = 0;

    while(file >> c1 >> c2){
        graph_node g;
        vector<int> cords;
        cords.push_back(c1);                  // Getting the vector cords
        cords.push_back(c2);                  // 
        g.id = i;                             //
        g.cords = cords;                      // Initializing a graph node
        g.outgoing = {};                      // 
        graph.push_back(g);                   //
        i++;
    }

    file.close();*/
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

   /* cout << "number of points: " << all_points.size()<< endl;
    cout << "1st vector data: ";
    vector<float> first = all_points[0];
    for (int i=0 ; i<all_points[0].size(); i++) {
        cout << first[i] << ", ";
    }
    cout << endl;*/

    for(int i = 0; i < all_points.size(); i++){
        graph_node g;
        g.id = i;
        g.cords = all_points[i];
        graph.push_back(g);
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < graph.size(); i++){
    int counter=0;      // Counter for counting correct outgoing connections for every graph node

        do{
           int edge_to_id = get_random_number(graph.size() - 1);

           if(edge_to_id != graph[i].id){             // If the id we want to connect is not the same as the source node id
                                                    
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
        } while(counter !=r);  // If we did r connections we stop
    }
    
    file.close();
    return 0;
}

pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>& graph, graph_node& s, graph_node& xq, int k, int l){
    vector<GraphNode> l_closest = {&s};
    vector<GraphNode> visited = {};
    vector<GraphNode> p = vectors_equal(l_closest, visited);

    while(!p.empty()){
        vector<float> distance = {};
        vector<GraphNode> pos = {};

        priority_queue<GraphNode, vector<GraphNode>, CompareDistance> pq;
        
        for (int i=0 ; i<p.size() ; i++) {
            p[i]->eucl_dist = euclidean(p[i]->cords, xq.cords);
            pq.push(p[i]);
            //distance.push_back(p[i]->eucl_dist);
            //pos.push_back(p[i]);
        }
        
        /*float min=10000;
        GraphNode min_id;

        for (int i=0 ; i<distance.size() ; i++) {
            if (distance[i] < min) {
                min = distance[i];
                min_id = pos[i];
            }
        }*/
       GraphNode p_star = pq.top();
        //cout << "MIN: " << p_star->eucl_dist << endl;
        //cout << "MIN ID: " << p_star->id << endl;

        for (forward_list<GraphNode>:: iterator it = p_star->outgoing.begin(); it != p_star->outgoing.end(); ++it) {
            auto cond = find(l_closest.begin(), l_closest.end(), (*it));

            if ((cond == l_closest.end()) && (*it)->id != xq.id) {
                l_closest.push_back((*it));   
            }               
        }

        visited.push_back(p_star);

        if (l_closest.size() >= l) {
            sort(l_closest.begin(), l_closest.end(), compare_eucl);
            l_closest.resize(l);
        }

        p = vectors_equal(l_closest, visited);
    }

    /*cout << "After sorting: ";

    for(int i = 0; i < l_closest.size(); i++){
        cout << l_closest[i]->eucl_dist << ", " << l_closest[i]->id << "|| ";
    }

    cout << endl;*/
    l_closest.resize(k);

    return {l_closest, visited};
}


/*MAIN FUNCTION*/
int main(void){

    vector<graph_node> graph;

    int flag = create_random_graph(graph, 13);
    if (flag) return 1;                             // Case of error in file opening

    /*temporary code for prinitng the graph*/
    /*for(int i = 0; i < graph.size(); i++){
        cout << "Id: " << graph[i].id << "-->";

        for(forward_list<GraphNode>:: iterator it = graph[i].outgoing.begin(); it != graph[i].outgoing.end(); ++it){
            cout << (*it)->id << " ";
        }

        cout << endl;
    }
    /*-------------------------------------*/

    pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, graph[0], graph[1000], 10, 100);

    /*temporary code for printing result*/
    cout << "closest k points from L: ";
    for(int i=0 ; i<res.first.size() ; i++){
        cout << res.first[i]->id << ", ";
    }

    cout << endl;
    cout << "size of visited: " << res.second.size();
    /*for (int i=0 ; i<res.second.size() ; i++){
        cout << res.second[i]->id << ", ";
    }*/
    cout << endl;
    /*-----------------------------------*/

    return 0;
}