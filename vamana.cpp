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

using namespace  std;

/*STRUCTS*/
/*typedef struct graph_node* GraphNode; // GraphNode is a pointer to struct graph_node

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
/*vector<GraphNode> vectors_equal(vector<GraphNode>&, vector<GraphNode>&);

float euclidean(const vector<int>& , const vector<int>& );

bool compare_eucl(GraphNode, GraphNode);

int get_random_number(int);

pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>&, graph_node&, graph_node&, int, int);

void robust_prune(GraphNode, vector<GraphNode>&, int, int);*/


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
           
            p[i]->eucl_dist = euclidean(p[i]->cords, xq.cords);             // MAKE IT A FUNTION FIND_MIN_EUCL
            pq.push(p[i]);
           
        }
        
      
       GraphNode p_star = pq.top();
      

        for (forward_list<GraphNode>:: iterator it = p_star->outgoing.begin(); it != p_star->outgoing.end(); ++it) {
            auto cond = find(l_closest.begin(), l_closest.end(), (*it));

            if ((cond == l_closest.end()) && (*it)->id != xq.id) {
                (*it)->eucl_dist = euclidean((*it)->cords, xq.cords); // re-compute in case of eucl_dist = -1
                l_closest.push_back((*it));   
            }               
        }

        visited.push_back(p_star);

        if (l_closest.size() >= l) {
            
            sort(l_closest.begin(), l_closest.end(), compare_eucl);
            l_closest.resize(l);

        }
        
        for(int i = 0; i < l_closest.size(); i++){
            cout << l_closest[i]->id << endl;
        }

        p = vectors_equal(l_closest, visited);
        
        for(int i = 0; i < p.size(); i++){
            cout << p[i]->id  << endl;
        }
        cout << endl;
    }

   
    l_closest.resize(k);

    return {l_closest, visited};
}

void robust_prune(GraphNode p, vector<GraphNode>& v, int a, int r) {
    auto found_at_pos = find(v.begin(), v.end(), p);
    
    if (found_at_pos != v.end()) v.erase(found_at_pos);
   
    for (forward_list<GraphNode>:: iterator it = p->outgoing.begin() ; it != p->outgoing.end() ; ++it) {
        auto cond = find(v.begin(), v.end(), (*it));
        
        if (cond == v.end()) {           
            v.push_back((*it));
            
        }
    }                   //V ← (V ∪ Nout(p)) \ {p}
    
    p->outgoing.clear();    //Nout(p) ← ∅
    

    while (!v.empty()) {
        
        priority_queue<GraphNode, vector<GraphNode>, CompareDistance> pq;
        
        for (int i=0 ; i<v.size() ; i++) {
            
            v[i]->eucl_dist = euclidean(v[i]->cords, p->cords);             // MAKE IT A FUNTION FIND_MIN_EUCL
            pq.push(v[i]);
            
        }
        
        GraphNode p_star = pq.top();           // p_star <- min apostasi

        p->outgoing.push_front(p_star);        // Nout(p) ← Nout(p) ∪ {p_star}



        int list_size = distance(p->outgoing.begin(), p->outgoing.end()); 
        if (list_size == r){
           
            break;
        }

        for (auto it = v.begin(); it != v.end();){
            
            if (a * euclidean(p_star->cords, (*it)->cords) <= euclidean((*it)->cords, p->cords)){     //SHOULD BE <=
                
                v.erase(it);
            
            } else {
                ++it;
            }
        }
    }
}



/*MAIN FUNCTION*/
int main(void){

    vector<graph_node> graph;

    int flag = create_random_graph(graph, 20);
    if (flag) return 1;                             // Case of error in file opening

    

    pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, graph[100], graph[1000], 10, 100);

    robust_prune(&graph[1000], res.second, 10, 30);

    cout << "v size: " << res.second.size() << endl;

    

    cout << endl; 
  

    return 0;
}