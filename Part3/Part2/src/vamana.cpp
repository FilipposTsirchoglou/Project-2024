/*vamana.cpp*/

#include "../include/util.hpp"
#include "../include/vamana.hpp"
#include <bits/stdc++.h>

using namespace std;



/*Returns a vector l_closest that cointains the k closest NN's and a vector v cointaining the visited nodes*/    
pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>& graph, graph_node& s, graph_node& xq, int k, int l , int gogo) {

    /*if (gogo ==1) {
      cout << "Graph in greedy" << endl;
      for ( int i = 0 ; i < graph.size() ; i++) {
        cout << "id: " << graph[i].id << endl;
        cout << "cords: " << endl;
        for (int j = 0 ; j < graph[i].cords.size() ; j++) {
          cout << graph[i].cords[j] << " ";
        }
        cout << endl;
      }
      cout << endl;
    }*/
     vector<GraphNode> l_closest= {&s};
    vector<GraphNode> visited = {};
    vector<GraphNode> p = vectors_difference(l_closest, visited);  // Computes L-V

    /*if (gogo==1) {
      cout << "This is P: " << endl;
      for (int i=0 ; i<p.size() ; i++) {
        cout << "id: " << p[i]->id << endl;
        cout << "cords: " << endl;
        for (int j=0 ; j<p[i]->cords.size() ; j++) {
          cout << p[i]->cords[j] << " ";
        }
        cout << endl;
      }
      cout << endl;

    }*/


    //cout << "P size:" << p.size() << endl;
    /*for (int i = 0 ; i < p.size() ; i++) {
        cout << "id: " << p[i]->id << " ";
        cout << "cords size: " << p[i]->cords.size() << endl;
        cout << endl;
    }*/

    //cout << "medoid in greedy search" << endl;
    //cout << "medoid id: " << s.id << endl;
    //cout << "medoid cords: " << endl;
    /*for (int i = 0 ; i < s.cords.size() ; i++) {
        cout << s.cords[i] << " ";
    }
    cout << endl;*/
    /*cout << p[0]->id << endl;
    cout << "cords size: " << p[0]->cords.size() << endl;
    cout << "cords: " << endl;
    for (int i=0 ; i<p[0]->cords.size() ; i++) {
        cout << p[0]->cords[i] << " ";
    }
    cout << endl;

    cout << "DONE" << endl;*/

    while(!p.empty()) {      // While L\V != empty set
            //GraphNode tmp_zq = &xq;
        /*cout << p[0]->id << endl;
        cout << "cords size: " << p[0]->cords.size() << endl;
        cout << "cords: " << endl;
        for (int i=0 ; i<p[0]->cords.size() ; i++) {
            cout << p[0]->cords[i] << " ";
        }
        cout << endl;*/

        //cout << "DONE" << endl;
       GraphNode p_star = find_min_eucl(p, xq);     // Finds the graph node p_star from all the p that has the min euclidian distance from xq

       /*if (gogo ==1 ) {
        cout << "p_star id: " << p_star->id << endl;
        cout << "p_star cords: " << endl;
        for (int i=0 ; i<p_star->cords.size() ; i++) {
          cout << p_star->cords[i] << " ";
        }
        cout << endl;
       }*/
      
        // Update L ← L ∪ Nout(p_star) and V ← V ∪ {p_star} 
        for (forward_list<GraphNode>:: iterator it = p_star->outgoing.begin() ; it != p_star->outgoing.end() ; ++it) {

          /*if (gogo ==1) {
            cout << "outgoing id: " << (*it)->id << endl;
            cout << "outgoing cords: " << endl;
            for (int i=0 ; i<(*it)->cords.size() ; i++) {
              cout << (*it)->cords[i] << " ";
            }
            cout << endl;
          }*/
            
            auto cond = find(l_closest.begin(), l_closest.end(), (*it));    // Search if the elements inside the outgoing list of p_star already exist in the l_closest vector

            if ((cond == l_closest.end()) && (*it)->id != xq.id) {          // If the element is not already in the l_closest list && the element is not the same as the xq element(we dont want to compute the euclidian distance between a point and its self)

                (*it)->eucl_dist = euclidean((*it)->cords, xq.cords);       // Re-compute in case of eucl_dist = -1

                l_closest.push_back((*it));

            }               
        }

        visited.push_back(p_star);

        // Update l_closest to retain closest L points to xq
        if (l_closest.size() > l) {
          if (gogo ==1) {
            cout << "resozed" << endl;
          }
            sort(l_closest.begin(), l_closest.end(), compare_eucl);     // Sorts the l_closest vector using a custom comparator function 
            l_closest.resize(l);        // Keep the first L elements of the vector
        }
        
        p = vectors_difference(l_closest, visited);      // Recompute L-V before the condition is checked  in the while loop
        /*if (gogo==1) {
          cout << "This is P recompute: " << endl;
          for (int i=0 ; i<p.size() ; i++) {
            cout << "id: " << p[i]->id << endl;
            cout << "cords: " << endl;
            for (int j=0 ; j<p[i]->cords.size() ; j++) {
              cout << p[i]->cords[j] << " ";
            }
            cout << endl;
          }
          cout << endl;
        }*/

        //cout << "P RECOMPUTATION size:"<< p.size() << endl;
        /*for (int i = 0 ; i < p.size() ; i++) {
          cout << "id: " << p[i]->id << " ";
          cout << "cord size : "<< p[i]->cords.size() << endl;
          cout << endl;
        }*/
    }

    if( !(l_closest.empty()) && (l_closest.size() > k)){
      
      l_closest.resize(k);        // Keeps the k closest elements of the vector that we want to return

    }

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
vector<graph_node> vamana(vector<vector<float>>& all_points, float a, int L, int R, GraphNode& med, vector<int>& pos) {


    // Creates a random graph using the all_points vector and stores it in the gaph vector
    vector<graph_node> graph;
    create_random_graph(graph, all_points, R, pos);
  
    // Stores a random permutation atvector sigma
    vector<int> sigma = give_random_permutation(graph.size());

    GraphNode s = find_medoid(graph);
     
    med = s;
    cout << "Creating vamana indexing graph..." << endl;

    for (int i=0 ; i<sigma.size() ; i++) {       // Itterate all the points of the vector in random order
        
        if( i % 1000 == 0) {

            cout << (i / 100) << "%" << endl;

        }

        pair<vector<GraphNode>, vector<GraphNode>> res = greedy_search(graph, *s, graph[sigma[i]], 1, L, 0);

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
void create_random_graph(vector<graph_node>& graph, vector<vector<float>>& all_points, int r, vector<int>& pos) {
    
    for(int i = 0 ; i < all_points.size() ; i++) {

        graph_node g;
        //g.id = i;
        //g.cords = all_points[i];
        g.id=pos[i];
        g.cords = all_points[i];
        graph.push_back(g);

    }

    if (graph.size() >= r) {  //pissible change  

      for(int i = 0 ; i < graph.size() ; i++) {
          
          int counter=0;      // Counter for counting correct outgoing connections for every graph node

          do {
            int edge_to_id = get_random_number(graph.size() - 1);
            //int edge_to_id = pos[get_random_number(graph.size() - 1)];  // Get a random number in the range of the graph size
            if(graph[edge_to_id].id != graph[i].id) {             // If the id we want to connect is not the same as the source node id
                                                      
                  if(graph[i].outgoing.empty()) { 
                                 // If the list is empty do the connection
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
    } else {

      for (int i=0 ; i<graph.size() ; i++) {
        for (int j=0 ; j<graph.size() ; j++) {

          if (i !=j ) {

            graph[i].outgoing.push_front(&graph[j]);

          }
        }
      }
    }
}



/*Finds and returns the medoid of the graph*/
GraphNode find_medoid(vector<graph_node>& graph) {
    cout << " Finding medoid..." << endl;
    
    double min_eucl = INFINITY;
    int min_pos;

    for (int i = 0 ; i < graph.size() ; i++) {

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



/**/
map<int, int> filteredFindMedoid(vector<graph_node>& graph, int th, vector<int>& filter_set) {

  map<int, int> M;             // Initializing M to be an empty map

  for (int i = 0 ; i < filter_set.size() ; i++) {

    M[filter_set[i]] = -1;

  }
  
  map<int, int> T;

  for (int i = 0 ; i < graph.size() ; i++) {

    T[i] = 0;                                         // Initializing T to a zero map

  }


  for (int i = 0 ; i < filter_set.size() ; i++) {
    
    vector<int> pf = {};        //positions 

    for (int j = 0 ; j < graph.size() ; j++) {

      bool found = false;
      for (int k = 0 ; k < graph[j].filters.size() ; k++) {

        if (graph[j].filters[k] == filter_set[i]) {            // If the node has the same filter with the current filter put it in Pf

          found = true;
          break;

        }
      }

      if (found) {

        pf.push_back(j);

      }

    }

    vector<int> rf = {};

    do {

      int random_ind = get_random_number(pf.size() - 1);

      if (rf.size() == 0) {                   // If Rf is empty, we surely insert the node with id = random_id

        rf.push_back(pf[random_ind]);

      }
      else {                                  // If Rf is not empty, we have to check if the node with id = random_id already exists

        bool flag = true;                // Flag that shows that the node with id = random_id does not already exist in Rf

        for (int j = 0 ; j < rf.size() ; j++) {
          
          if (rf[j] == pf[random_ind]) {              // The node already exists

            flag = false;
            break;

          }
        }

        if (flag == true) {

          rf.push_back(pf[random_ind]);

        }
      }

    } while (rf.size() < th);     // We stop when Rf has th nodes

    int min = 100000;
   
    int p_star = -1;

    for (int i = 0 ; i < rf.size() ; i++) {
      
      if (T[rf[i]] < min) {

        min = T[rf[i]];
        p_star = rf[i];

      }
    }

    M[filter_set[i]] = p_star;
 
    T[p_star]++;

  }

  return M;

}


pair<vector<GraphNode>, vector<GraphNode>> filteredGreedySearch(vector<graph_node>& graph, vector<int>& s, graph_node& xq, int k, int l) {
  
  vector<GraphNode> l_closest = {};
  vector<GraphNode> visited = {};

  //find if current point inside the start points vector and query have at least one common filter
  for (int i = 0 ; i < s.size() ; i++) {      //for most times this loop will run only once

    bool same = false;
    for (int j = 0 ; j < xq.filters.size() ; j++){
      if (graph[s[i]].filters[0] == xq.filters[j]){     //check if they have at least one common filter
        same = true;
        break;
      }
    }

    if (same == true) {         //if they have at least one common filter
      l_closest.push_back(&graph[s[i]]);      //l_closest starts with the start point (that has at least one common filter with the query)
    }
  }
  //for the construction of the vamana index at this point the l_closest vector has only the start point l_closest.size =1
  

  //compute the L-V vector
  vector<GraphNode> p = vectors_difference(l_closest, visited);
  
  while (!p.empty()) {

    //find p_star inside p that has the min euclidian distance from xq
    GraphNode p_star = find_min_eucl(p, xq);   
   
    visited.push_back(p_star);
  
    vector<GraphNode> temp_nout = {};
    //in temp_nout store the outgoing points of p_sar that have at least one common filter with the query and dont exist in the visited vector
    
    if (!(p_star->outgoing.empty())) {

      for (forward_list<GraphNode>::iterator it = p_star->outgoing.begin(); it != p_star->outgoing.end(); ++it) {   //iterate over the outgoing list of p_star
        
        //auto cond1 = find(p_star->outgoing.begin(), p_star->outgoing.end(), p_star);
        bool exists_in_visited = false;
        for (int i=0 ; i<visited.size() ; i++) {

          if ((*it) == visited[i]) {
            exists_in_visited = true;
            break;
          }
        }
          
        if(!exists_in_visited) {

          for (int j=0 ; j<(*it)->filters.size() ; j++) {

            bool have_at_least_one_common_filter = false;
            for(int k=0 ; k<xq.filters.size() ; k++) {

              if ( (*it)->filters[j] == xq.filters[k]) {
                temp_nout.push_back((*it));
                break;
              }
            }
            break;
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
    }     //L U tempnout(p_star) with check that the elements of tempnout dont already exist in l_closest

    if (l_closest.size() > l) {    
      stable_sort(l_closest.begin(), l_closest.end(), compare_eucl);     // Sorts the l_closest vector using a custom comparator function 
      l_closest.resize(l);        // Keep the first L elements of the vector
    }
        
    p = vectors_difference(l_closest, visited);      // Recompute L-V before the condition is checked  in the while loop
  }
    
  if(  ( !(l_closest.empty()) ) && ( l_closest.size() > k ) ) {
    l_closest.resize(k);        // Keeps the k closest elements of the vector that we want to return
  }

  return {l_closest, visited};
}




/**/
void filteredRobustPrune(GraphNode p, vector<GraphNode>& v, float a, int r) {

    // If p is found inside vector v then remove it
    auto found_at_pos = find(v.begin(), v.end(), p);    
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
    p->outgoing = {};   // Nout(p) ← ∅

    while (!v.empty()) {

      GraphNode p_star = find_min_eucl(v, *p);
    
      auto cond5 = find(p->outgoing.begin(), p->outgoing.end(), p_star);
      if ( (p->id != p_star->id) && (cond5 == p->outgoing.end()) ) {              // MAYBE NOT NEEDED 
        p->outgoing.push_front(p_star);           // Nout(p) ← Nout(p) ∪ {p_star}
      }

      int list_size = distance(p->outgoing.begin(), p->outgoing.end()); 
      if (list_size == r) {
          break;
      }

      for (auto it = v.begin(); it != v.end();){      // For (p' E V) do
        
        if (((*it)->filters[0] == p->filters[0]) && (p->filters[0] != p_star->filters[0])) {
          
          ++it;
          continue;

        }
        if (a * euclidean(p_star->cords, (*it)->cords) <= euclidean((*it)->cords, p->cords)) {     
                
          v.erase(it);
            
        } else {            // Only increase the iterator if erase didn't happen beacuse of segfault

          ++it;

        }
      } 
    }
}




/**/
void filteredVamana(vector<graph_node>& graph, float a, int L, int R, vector<int>& filter_set, int th, map<int, int>& med) {

  vector<int> sigma = give_random_permutation(graph.size());

  //itterate through all the points of the vector in random order
  for (int i = 0 ; i < sigma.size() ; i++) {      
    cout << i << endl;
    int start_node_id = med[graph[sigma[i]].filters[0]];   //the pos of the medoid of the filter of the current node in the graph vector
   
    vector<int> s = {};  //Holds the ids of the medoids of the fitlers of the current node
    //vector<GraphNode> s = {};     // Holds the medoids of the filters of the current node. In our case it will hold only one element
    //s.push_back(&graph[start_node]);
    s.push_back(start_node_id);    

    // Call filtered greedy for start node s, xq = the current point from the random permutation, k=0 and L
    pair<vector<GraphNode>, vector<GraphNode>> res = filteredGreedySearch(graph, s, graph[sigma[i]], 0, L);
    res.first = {};

    filteredRobustPrune(&graph[sigma[i]], res.second, a, R);
  
    for (forward_list<GraphNode>::iterator it = graph[sigma[i]].outgoing.begin() ; it != graph[sigma[i]].outgoing.end() ; ++it) {
      auto cond = find((*it)->outgoing.begin(), (*it)->outgoing.end(), &graph[sigma[i]]);
      if ((*it)->id != graph[sigma[i]].id && cond == (*it)->outgoing.end()) {

        (*it)->outgoing.push_front(&graph[sigma[i]]);

      }

      int list_size = distance((*it)->outgoing.begin(), (*it)->outgoing.end());
      if (list_size > R) {

        vector<GraphNode> temp_vec = {};
        
        for (forward_list<GraphNode>::iterator it1 = graph[sigma[i]].outgoing.begin() ; it1 != graph[sigma[i]].outgoing.end() ; ++it1) {

          temp_vec.push_back((*it1));

        }

        filteredRobustPrune((*it), temp_vec, a, R);

      }
    }
  }
}