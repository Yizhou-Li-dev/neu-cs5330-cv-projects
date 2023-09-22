//
//  general_utils.cpp
//  project2
//
//  Created by Yizhou Li on 2/7/23.
//

#include "general_utils.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;
 
// Comparator function to sort pairs
// according to second value
bool cmp(pair<string, float>& a,
        pair<string, float>& b)
{
    return a.second < b.second;
}
 
// Function to sort the map according
// to value in a (key-value) pairs
void sort(map<string, float>& M, vector<pair<string, float>>& A)
{ 
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
 
    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);
}
