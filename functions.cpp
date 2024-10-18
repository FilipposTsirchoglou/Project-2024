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
            cout << "IN RESIZE" << endl;
            sort(l_closest.begin(), l_closest.end(), compare_eucl);
            l_closest.resize(l);
        }
        cout << "l-closest print" << endl;
        for(int i = 0; i < l_closest.size(); i++){
            cout << l_closest[i]->id << endl;
        }

        p = vectors_equal(l_closest, visited);
        cout << " vectors equal print" << endl;
        for(int i = 0; i < p.size(); i++){
            cout << p[i]->id;
        }
        cout << endl;
    }

    /*cout << "After sorting: ";

    for(int i = 0; i < l_closest.size(); i++){
        cout << l_closest[i]->eucl_dist << ", " << l_closest[i]->id << "|| ";
    }

    cout << endl;*/
    l_closest.resize(k);

    return {l_closest, visited};
}