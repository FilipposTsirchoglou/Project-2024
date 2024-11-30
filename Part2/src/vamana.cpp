/*vamana.cpp*/

#include "../include/util.hpp"
#include "../include/vamana.hpp"
#include <bits/stdc++.h>

using namespace std;



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
    cout << "Creating vamana indexing graph..." << endl;

    for (int i=0 ; i<sigma.size() ; i++) {       // Itterate all the points of the vector in random order
        
        if( i % 1000 == 0) {
            cout << (i / 100) << "%" << endl;
        }

        pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, *s, graph[sigma[i]], 1, L);
        vector<GraphNode> v = res.second;        
        robust_prune(&graph[sigma[i]], v, a, R);
        
        // For all points j in Nout(σ(i))
        for (auto it = graph[sigma[i]].outgoing.begin() ; it != graph[sigma[i]].outgoing.end() ; ++it) {
            
            vector<GraphNode> temp_vec;
            // temp_vec = Nout(j) U {σ(i)}
            for (auto it2 = (*it)->outgoing.begin() ; it2 != (*it)->outgoing.end() ; ++it2) {
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
    cout << "100%" << endl;
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
    cout << " Finding medoid..." << endl;
    
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
    cout << " Medoid found !" << endl;

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





map<int, int> filteredFindMedoid(vector<graph_node>& graph, int th, vector<int>& filter_set) {

  map<int, int> M;             // Initializing M to be an empty map
  
  map<int, int> T;

  for (int i = 0 ; i < graph.size() ; i++) {

    T[i] = 0;                                         // Initializing T to a zero map

  }

  for (int i = 0 ; i < filter_set.size() ; i++) {
    
    vector<graph_node> pf = {};

    for (int j = 0 ; j < graph.size() ; j++) {

      bool found = false;
      for (int k = 0 ; k < graph[j].filters.size() ; k++) {

        if (graph[j].filters[k] == filter_set[i]) {

          found = true;
          break;

        }
      }

      if (found) {

        pf.push_back(graph[j]);

      }

    }

    vector<graph_node> rf = {};

    do {

      int random_id = get_random_number(th - 1);

      if (rf.size() == 0) {                   // If Rf is empty, we surely insert the node with id = random_id

        rf.push_back(graph[random_id]);

      }
      else {                                  // If Rf is not empty, we have to check if the node with id = random_id already exists

        bool flag = true;                // Flag that shows that the node with id = random_id does not already exist in Rf

        for (int j = 0 ; j < rf.size() ; j++) {
          
          if (rf[j].id == random_id) {              // The node already exists

            flag = false;
            break;

          }
        }

        if (flag == true) {

          rf.push_back(graph[random_id]);

        }
      }

    } while (rf.size() < th);     // We stop when Rf has th nodes

    int min = 100000;
    int p_star;

    for (int i = 0 ; i < rf.size() ; i++) {

      if (T[rf[i].id] < min) {

        min = T[rf[i].id];
        p_star = rf[i].id;

      }

    }

    M[i] = p_star;
    T[p_star]++;

  }

  return M;

}




pair<vector<GraphNode>, vector<GraphNode>> filteredGreedySearch(vector<graph_node>& graph, vector<graph_node>& s, graph_node& xq, int k, int l) {
  vector<GraphNode> l_closest = {};
  vector<GraphNode> visited = {};

  for (int i = 0 ; i < s.size() ; i++) {

    bool same = false;
    for (int j = 0 ; j < xq.filters.size() ; j++){

      if (s[i].filters[0] == xq.filters[j]){

        same = true;
        break;

      }

    }

    if (same) {
        
        l_closest.push_back(&s[i]);

    }
  }

  vector<GraphNode> p = vectors_difference(l_closest, visited);  // Computes L-V

  while (!p.empty()) {
    GraphNode p_star = find_min_eucl(p, xq);     // Finds the graph node p_star from all the p that has the min euclidian distance from xq
    visited.push_back(p_star);
    vector<GraphNode> temp_nout = {};
    
    if (!(p_star->outgoing.empty())) {

      for (forward_list<GraphNode>::iterator it = p_star->outgoing.begin(); it != p_star->outgoing.end(); ++it) {
        
        auto cond = find(visited.begin(), visited.end(), (*it));    // Search if the elements inside the outgoing list of p_star already exist in the visited vector
        
        if ((cond == visited.end()) && (*it)->id != xq.id) {      /* SOS CHECK it->id != xq.id */     // If the element is not already in the visited list && the element is not the same as the xq element(we dont want to compute the euclidian distance between a point and its self)
          
          (*it)->eucl_dist = euclidean((*it)->cords, xq.cords);       // Re-compute in case of eucl_dist = -1
          
          for (int i = 0 ; i < (*it)->filters.size() ; i++) {   /*    make function bool found_common_filter SOSOSOSOS                 */
            for (int j = 0; j < xq.filters.size(); j++) {
              
              if ((*it)->filters[i] == xq.filters[j]) {
                
                temp_nout.push_back((*it));
                break;

              }
            }                                                               
          }     
        }
      }
      for (int i = 0 ; i < temp_nout.size() ; i++) {
        bool found = false;
          for (int j=0 ; j < l_closest.size(); j++) {

            if (temp_nout[i] == l_closest[j]){
              found = true;
              break;
            }
          }

          if (!found) {
            
            l_closest.push_back(temp_nout[i]);
            
          }
      }
    }
    if (l_closest.size() > l) {
            
      sort(l_closest.begin(), l_closest.end(), compare_eucl);     // Sorts the l_closest vector using a custom comparator function 
      l_closest.resize(l);        // Keep the first L elements of the vector
    }
        
    p = vectors_difference(l_closest, visited);      // Recompute L-V before the condition is checked  in the while loop
  }

  if(l_closest.size() > 0) {

    l_closest.resize(k);        // Keeps the k closest elements of the vector that we want to return

  } 
   
  return {l_closest, visited};
}