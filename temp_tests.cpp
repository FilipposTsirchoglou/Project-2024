#include "acutest.h"
#include "temp_header.h"
#include <iostream>

using namespace std;


void test_euclidean(){

    vector<float> vec1 = {1,2};
    vector<float> vec2 = {4,2};

    float dist;
    dist = euclidean(vec1, vec2);
    TEST_ASSERT(dist == 3);

    dist = euclidean(vec1, vec1);
    TEST_ASSERT(dist == 0);
}


void test_get_random_number(){
    int max = 20;
    int num = get_random_number(max);
    TEST_ASSERT(num < max);
    TEST_ASSERT(num >= 0);
}


TEST_LIST = {
	{ "test_euclidean", test_euclidean },
	{ "test_get_random_number", test_get_random_number },

	{ NULL, NULL }
};