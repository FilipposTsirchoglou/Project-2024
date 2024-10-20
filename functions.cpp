#include <vector>
#include <cmath>
#include <ctime>
#include "temp_header.h"

using namespace std;


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

            if (a * euclidean(p_star->cords, (*it)->cords) < euclidean((*it)->cords, p->cords)){     //if mistake, LOOK HERE
               
                v.erase(it);
                
            } else {
                ++it;
            }


        }
    }
}

GraphNode find_medoid(vector<graph_node>& graph){
    float min_eucl = 1000000;
    int min_pos;

    for(int i=0 ; i<graph.size() ; i++) {
        float eucl=0;
        for (int j=0 ; j<graph.size() ; j++) {
            if (i==j) continue;
            eucl += euclidean(graph[i].cords, graph[j].cords);
        }
        //cout << "sum_eucl: " << eucl << endl;
        if (eucl < min_eucl) {
            min_eucl = eucl;
            //cout << "new min_eucl: " << min_eucl << endl;
            min_pos = i;
        }
    }
    //cout << "pos: " << min_pos << endl;
   // cout << "graph[4] " << &graph[4] << endl;
    GraphNode medoid = &graph[min_pos];
    //cout << "medoid: " << medoid << endl;

    return medoid;
}