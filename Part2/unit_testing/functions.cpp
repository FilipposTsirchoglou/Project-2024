#include <vector>
#include <queue>
#include <cmath>
#include <ctime>
#include "temp_header.hpp"
#include <bits/stdc++.h>

using namespace std;


/*Takes a vector of graph node pointers and a vector of node ids and returns a vector of the ids that are in the same vector position but are different*/
vector<int> temp_vectors_equal(vector<GraphNode>& greedy_res, vector<int>& gt_res) {  
    
    bool are_same;
    vector<int> dif = {};

    for(int i = 0 ; i < greedy_res.size() ; i++) {
        are_same = false;
        for(int j = 0 ; j < greedy_res.size() ; j++) {
            if(greedy_res[i]->id == gt_res[j]) {     
                are_same = true;
                break;
            }
        }
        if (are_same == false) {
            dif.push_back(greedy_res[i]->id);
        }
    }
    
    return dif;

}



/*Takes the vamana graph, a query point , k and a search list of size L and returns a vector of the k closest neighbours of the query point*/
vector<GraphNode> find_k_closest_for_single_query_point(vector<graph_node>& vamana_graph, vector<float>& q_point, int k, int L, graph_node& s) {
    
    graph_node query;
    query.id = vamana_graph.size();
    query.cords = q_point;
    query.eucl_dist = -1;                 // Putting the query point in a graph node and inserting it temporarily in the vamana graph
    query.outgoing={};                    // to find its k closest
    vamana_graph.push_back(query);
    
    // Performing greedy search in the vamana graph for the specific query point
    pair<vector<GraphNode>, vector<GraphNode>> greedy_result2 = greedy_search(vamana_graph, s, vamana_graph[vamana_graph.size() - 1], k, L);

    // After finding the k closest of the query, we remove it from the graph
    vamana_graph.erase(vamana_graph.end()-1);
    
    return greedy_result2.first;

}



/*Returns a vector of vectors with the k closest neighbours of every single query point*/
vector<vector<GraphNode>> find_k_closest_for_all_query_points(vector<graph_node>& vamana_graph, vector<vector<float>>& q_points, int k, int L, graph_node& s) {

    graph_node query;
    query.id = vamana_graph.size();
    query.cords = {};
    query.eucl_dist = -1;
    query.outgoing = {};
    vamana_graph.push_back(query);

    
    pair<vector<GraphNode>, vector<GraphNode>> greedy_result2;
    vector<vector<GraphNode>> k_closest_vec;
    //vector<int> all_dif;
    for (int i = 0 ; i < q_points.size() ; i++) {

        vamana_graph[vamana_graph.size()-1].cords = q_points[i];
        vamana_graph[vamana_graph.size()-1].outgoing = {};
        vamana_graph[vamana_graph.size()-1].eucl_dist = -1;

        greedy_result2 = greedy_search(vamana_graph, s, vamana_graph[vamana_graph.size() - 1], k, L);
        
        k_closest_vec.push_back(greedy_result2.first);
    }  

    vamana_graph.erase(vamana_graph.end()-1);

    return k_closest_vec;

}



/*Compares the k closest neighbours of a single point we found using the vamana algorithm, with the ones of the groundtruth file and returs the recall*/
float check_groundtruth_for_single_query_point(vector<GraphNode>& my_k_closest, vector<int>& gt_k_closest) {

    vector<int> dif = temp_vectors_equal(my_k_closest, gt_k_closest);
    float recall = (float)( my_k_closest.size()-dif.size() ) / (float)(my_k_closest.size());
    
    return recall;

}



/*Compares the k closest neighbours of all the query points we we found using the vamana algorithm, with the ones of the groundtruth file and returs the average recall*/
float check_groundtruth_for_all_query_points(vector<vector<GraphNode>>& my_k_closest_vec, vector<vector<int>>& gt_k_closest_vec) {
    
    float recall_sum = 0;
    
    for(int i = 0 ; i < my_k_closest_vec.size() ; i++) {
        
        vector<int> dif = temp_vectors_equal(my_k_closest_vec[i], gt_k_closest_vec[i]);
        float recall = (float)( my_k_closest_vec[i].size()-dif.size() ) / (float)(my_k_closest_vec[i].size());
        //cout << "recall: " << recall << endl;
        recall_sum += recall;

    }
    
    return recall_sum / (float)(gt_k_closest_vec.size());

}





/*Returns a vector with the difference of the k-approx NNs and the visited nodes*/
vector<GraphNode> vectors_difference(vector<GraphNode>& l_closest, vector<GraphNode>& visited) {  
    
    bool are_same;
    vector<GraphNode> difference = {};

    for(int i = 0 ; i < l_closest.size() ; i++) {
        are_same = false;
        for(int j = 0 ; j < visited.size() ; j++) {
            if(l_closest[i] == visited[j]) {     
                are_same = true;
                break;
            }
        }
        if (are_same == false) {
            difference.push_back(l_closest[i]);
        }
    }
    
    return difference;

}



/*Takes 2 vectors and returns the euclidean distance*/
double euclidean(const vector<float>& p1, const vector<float>& p2) {    
    
    float dist = 0.0;

    for(int i = 0 ; i < p1.size() ; i++) {
        float temp = p1[i] - p2[i];
        dist += temp * temp;
    }

    return sqrt(dist);

}



/*Returns true if euclidean of first element < euclidean of second element*/
bool compare_eucl(GraphNode g1, GraphNode g2) {
    
    return g1->eucl_dist < g2->eucl_dist;

}



/*Returns a random integer in a certain range*/
int get_random_number(int max) {
    static bool seeded = false;

    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    return rand() % (max + 1);

}



/*Returns a pointer to the graph_node that has the min euclidean distance from xq*/
GraphNode find_min_eucl(vector<GraphNode>& p, graph_node& xq) {
    priority_queue<GraphNode, vector<GraphNode>, CompareDistance> pq;   // Priority queue using a min heap. Inserts the elements with priority the smallest euclidian distance
        
    for (int i=0 ; i<p.size() ; i++) {
        p[i]->eucl_dist = euclidean(p[i]->cords, xq.cords);
        pq.push(p[i]);          
    }

    GraphNode p_star = pq.top();
    
    return p_star;
    
}




/*Returns a vector l_closest that cointains the k closest NN's and a vector v cointaining the visited nodes*/    
pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>& graph, graph_node& s, graph_node& xq, int k, int l) {
    vector<GraphNode> l_closest = {&s};
    vector<GraphNode> visited = {};
    vector<GraphNode> p = vectors_difference(l_closest, visited);  // Computes L-V

    while(!p.empty()) {      // While L\V != empty set

       GraphNode p_star = find_min_eucl(p, xq);     // Finds the graph node p_star from all the p that has the min euclidian distance from xq
      
        // Update L ← L ∪ Nout(p_star) andV ← V ∪ {p_star} 
        for (forward_list<GraphNode>:: iterator it = p_star->outgoing.begin() ; it != p_star->outgoing.end() ; ++it) {
            auto cond = find(l_closest.begin(), l_closest.end(), (*it));    // Search if the elements inside the outgoing list of p_star already exist in the l_closest vector

            if ((cond == l_closest.end()) && (*it)->id != xq.id) {          // If the element is not already in the l_closest list && the element is not the same as the xq element(we dont want to compute the euclidian distance between a point and its self)
                (*it)->eucl_dist = euclidean((*it)->cords, xq.cords);       // Re-compute in case of eucl_dist = -1
                l_closest.push_back((*it));   
            }               
        }
        visited.push_back(p_star);

        // Update l_closest to retain closest L points to xq
        if (l_closest.size() > l) {
            
            sort(l_closest.begin(), l_closest.end(), compare_eucl);     // Sorts the l_closest vector using a custom comparator function
           
            l_closest.resize(l);        // Keep the first L elements of the vector
        }
        
        p = vectors_difference(l_closest, visited);      // Recompute L-V before the condition is checked  in the while loop
    }

    l_closest.resize(k);        // Keeps the k closest elements of the vector that we want to return

    return {l_closest, visited};
}



/*Graph gets modified by setting at most R new outgoing neigbors for p*/
void robust_prune(GraphNode p, vector<GraphNode>& v, float a, int r) {
    // If p is found inside vector v then remove it
    auto found_at_pos = find(v.begin(), v.end(), p);  //    
    if (found_at_pos != v.end()) {
        v.erase(found_at_pos);
    }
    
    // V U Nout(p)
    for (forward_list<GraphNode>:: iterator it = p->outgoing.begin() ; it != p->outgoing.end() ; ++it) {
        auto cond = find(v.begin(), v.end(), (*it));  // Search the v vector to find if the element at position it of the outgoing list exists
        
        if (cond == v.end()) {      // If you didnt find it         
            v.push_back((*it));  
        }
    }                   // V ← (V ∪ Nout(p)) \ {p}
    
    p->outgoing.clear();    // Nout(p) ← ∅
    
    while (!v.empty()) {

        GraphNode p_star = find_min_eucl(v, *p); 
        p->outgoing.push_front(p_star);        // Nout(p) ← Nout(p) ∪ {p_star}
       

        int list_size = distance(p->outgoing.begin(), p->outgoing.end()); 
        if (list_size == r) {
            break;
        }

        for (auto it = v.begin(); it != v.end();) {      // For (p' E V) do
        
            if (a * euclidean(p_star->cords, (*it)->cords) <= euclidean((*it)->cords, p->cords)) {    
    
                v.erase(it);
            
            } else {            // Only increase the iterator if erase didn't happen beacuse of segfault
                ++it;
            }
        }
    }
}



/*Returns a vamana indexing graph*/
vector<graph_node> vamana(vector<vector<float>>& all_points, float a, int L, int R, GraphNode& med) {

    // Creates a random graph using the all_points vector and stores it in the gaph vector
    vector<graph_node> graph;
    create_random_graph(graph,all_points, R);

    // Stores a random permutation atvector sigma
    vector<int> sigma = give_random_permutation(graph.size());

    GraphNode s = find_medoid(graph);   
    med = s;

    for (int i=0 ; i<sigma.size() ; i++) {       // Itterate all the points of the vector in random order
        //cout << "i: " << i <<endl;

        pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, *s, graph[sigma[i]], 1, L);
        vector<GraphNode> v = res.second;        
        robust_prune(&graph[sigma[i]], v, a, R);
        
        // For all points j in Nout(σ(i))
        for (auto it = graph[sigma[i]].outgoing.begin() ; it != graph[sigma[i]].outgoing.end() ; ++it) {
            
            vector<GraphNode> temp_vec;
            // temp_vec = Nout(j) U {σ(i)}
            for (auto it2 = (*it)->outgoing.begin() ; it2 != (*it)->outgoing.end() ; ++it2) {
                //cout << (*it2)->id << " , ";
                temp_vec.push_back((*it2));
            }
            temp_vec.push_back(&graph[sigma[i]]);
            
            if (temp_vec.size() > R) {
                robust_prune((*it), temp_vec, a, R);
            }
            else {
                (*it)->outgoing.push_front(&graph[sigma[i]]);
            }
        }
    }

    return graph;
}



/*Creates a random r-regular graph and returns it from the parameters*/
void create_random_graph(vector<graph_node>& graph, vector<vector<float>>& all_points, int r) {

    for(int i = 0 ; i < all_points.size() ; i++) {
        graph_node g;
        g.id = i;
        g.cords = all_points[i];
        graph.push_back(g);
    }
    
    for(int i = 0 ; i < graph.size() ; i++) {
        int counter=0;      // Counter for counting correct outgoing connections for every graph node

        do {
           int edge_to_id = get_random_number(graph.size() - 1);

           if(edge_to_id != graph[i].id) {             // If the id we want to connect is not the same as the source node id
                                                    
                if(graph[i].outgoing.empty()) {              // If the list is empty do the connection
                    GraphNode edge = &graph[edge_to_id];   
                    graph[i].outgoing.push_front(edge);
                    counter++;
                }
                else {
                    bool flag=true;     // Flag that shows the id of the node we want to connect does not already exist

                    for(forward_list<GraphNode>:: iterator it = graph[i].outgoing.begin(); it != graph[i].outgoing.end(); ++it) {

                        if((*it)->id == edge_to_id) { // The id already exists 
                            flag=false;
                            break;
                        }
                    }

                    if(flag==true) {
                        GraphNode edge = &graph[edge_to_id];   
                        graph[i].outgoing.push_front(edge);
                        counter++;
                    }

                }
           }
        } while(counter !=r);  // If we did r connections we stop
    }
}



/*Finds and returns the medoid of the graph*/
GraphNode find_medoid(vector<graph_node>& graph) {
    double min_eucl = INFINITY;
    int min_pos;

    for(int i = 0 ; i < graph.size() ; i++) {

        double eucl_sum=0;
        for (int j = 0 ; j < graph.size() ; j++) {
            if (i==j) continue;
            eucl_sum += euclidean(graph[i].cords, graph[j].cords);
        }
        if (eucl_sum < min_eucl) {
            min_eucl = eucl_sum;
            min_pos = i;
        }
    }
    GraphNode medoid = &graph[min_pos];

    return medoid;
}



/*Returns a random permutation of integers*/
vector<int> give_random_permutation(int size) {
    vector<int> sigma;

    for (int i = 0 ; i < size ; i++) {
        sigma.push_back(i);
    }
    random_shuffle(sigma.begin(), sigma.end());

    return sigma;
}