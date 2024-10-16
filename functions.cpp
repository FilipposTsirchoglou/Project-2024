#include <vector>
#include <cmath>
#include <ctime>

using namespace std;


float euclidean(const vector<float>& p1, const vector<float>& p2){    
    float dist = 0.0;

    for(int i = 0; i < p1.size(); i++){
        int temp = p1[i] - p2[i];
        dist += temp * temp;
    }

    return sqrt(dist);
}

int get_random_number(int max){
    static bool seeded = false;

    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    return rand() % (max + 1);
}