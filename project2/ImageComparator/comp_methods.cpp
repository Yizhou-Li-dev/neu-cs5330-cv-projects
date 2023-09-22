//
//  comparationMethods.cpp
//  ImageComparator
//
//  Created by Yizhou Li on 2/6/23.
//

#include "comp_methods.hpp"
#include "my_csv_util.hpp"
#include "general_utils.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;

int compare(string dbFilePath, string picFilePath, string matchingMethod, int numOfOutputs, string outputs[]) {
    // get individual image vector
    vector<float> picImgVec;
    read_csv_bypic(dbFilePath, picFilePath, picImgVec);
    
    // get all image vectors
    vector<pair<string, vector<float>>> imgVecs;
    read_csv_bylines(dbFilePath, imgVecs);
    
    // do distance metric calculation and record the results into a map
    map<string, float> tmpMap;
    for (auto aImgVec : imgVecs) {
        string name = aImgVec.first;
        float val = 0;
        
        if (matchingMethod == "sum_of_squared_diff") {
            sum_of_squared_diff(picImgVec, aImgVec.second, val);
        } else if (matchingMethod == "histogram_intersection") {
            histogram_intersection(picImgVec, aImgVec.second, val);
        }
        
        if (name != picFilePath) { // dont add the compare image itself
            tmpMap.insert({name, val});
        }
    }
    
    // sort the map according to the value
    vector<pair<string, float>> A;
    sort(tmpMap, A);
    
    // iterate first n results of the map and put it in outpus array
    int count = 0;
    for (auto itr = A.begin(); itr != A.end(); itr++) {
        outputs[count] = itr->first;
        if (count == numOfOutputs) {
            break;
        }
        count += 1;
    }
    
    return 0;
}

// *** distance metrics *** //
int sum_of_squared_diff(vector<float> f1, vector<float> f2, float &rst) {
    auto val = 0;
    
    for(int i = 0; i < f1.size(); i++) {
        val = f1[i] - f2[i];
        val *= val;
        rst += val;
    }

    return 0;
}

int histogram_intersection(vector<float> f1, vector<float> f2, float &rst) {
    float val = 0, sumA = 0, sumB = 0;
    
    for(int i = 0; i < f1.size(); i++) {
        sumA += f1[i];
        sumB += f2[i];
    }
    
    for(int i = 0; i < f1.size(); i++) {
        auto af = f1[i]/sumA;
        auto bf = f2[i]/sumB;
        val = af < bf ? af : bf;
        rst += val;
    }
    
    rst = 1.0 - rst;
    return 0;
}
