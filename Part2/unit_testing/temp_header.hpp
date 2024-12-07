#include <iostream>
#include <string>
#include <vector>
#include <forward_list>
#include <utility>

using namespace std;

// GraphNode is a pointer to a graph node
typedef struct graph_node* GraphNode;




// This is a graph node
typedef struct graph_node {
    int id;                                     // Node's unique id
    vector<float> cords;                        // Each node has a vector. These are the vector's cords
    forward_list<GraphNode> outgoing;           // List of pointers to graph_nodes
    double eucl_dist;                           // The euclidean distance between the nodes' vector to a query vector
}graph_node;


// This struct compares two graph node pointers based on their euclidean distance with the query point
struct CompareDistance {
    bool operator()(GraphNode p1, GraphNode p2) {
        // Return true if p1 has higher distance, so it has lower priority
        return p1->eucl_dist > p2->eucl_dist;
        
    }
};


/*Takes a vector of graph node pointers and a vector of node ids and returns a vector of the ids that are in the same vector position but are different*/
vector<int> temp_vectors_equal(vector<GraphNode>&, vector<int>&);



/*Takes the vamana graph, a query point , k and a search list of size L and returns a vector of the k closest neighbours of the query point*/
vector<GraphNode> find_k_closest_for_single_query_point(vector<graph_node>&, vector<float>&, int, int, graph_node&);



/*Returns a vector of vectors with the k closest neighbours of every single query point*/
vector<vector<GraphNode>> find_k_closest_for_all_query_points(vector<graph_node>&, vector<vector<float>>&, int, int, graph_node&);



/*Compares the k closest neighbours of a single point we found using the vamana algorithm, with the ones of the groundtruth file and returs the recall*/
float check_groundtruth_for_single_query_point(vector<GraphNode>&, vector<int>&);



/*Compares the k closest neighbours of all the query points we we found using the vamana algorithm, with the ones of the groundtruth file and returs the average recall*/
float check_groundtruth_for_all_query_points(vector<vector<GraphNode>>&, vector<vector<int>>&);




/*Returns a vector with the difference of the k-approx NNs and the visited nodes*/
vector<GraphNode> vectors_difference(vector<GraphNode>&, vector<GraphNode>&);



/*Takes 2 vectors and returns the euclidean distance*/
double euclidean(const vector<float>& , const vector<float>&);



/*Returns true if euclidean of first element < euclidean of second element*/
bool compare_eucl(GraphNode, GraphNode);



/*Returns a random integer in a certain range*/
int get_random_number(int);



/*Returns a pointer to the graph_node that has the min euclidean distance from xq*/ 
GraphNode find_min_eucl(vector<GraphNode>&, graph_node&);



/*Returns a vector l_closest that cointains the k closest NN's and a vector v cointaining the visited nodes*/
pair<vector<GraphNode>, vector<GraphNode>> greedy_search(vector<graph_node>&, graph_node&, graph_node&, int, int);



/*Graph gets modified by setting at most R new outgoing neigbors for p*/
void robust_prune(GraphNode, vector<GraphNode>&, float, int);



/*Returns a vamana indexing graph*/
vector<graph_node> vamana(vector<vector<float>>&, float, int, int, GraphNode&);



/*Creates a random r-regular graph and returns it from the parameters*/
void create_random_graph(vector<graph_node>&, vector<vector<float>>&, int);



/*Finds and returns the medoid of the graph*/
GraphNode find_medoid(vector<graph_node>&);



/*Returns a random permutation of integers*/
vector<int> give_random_permutation(int);