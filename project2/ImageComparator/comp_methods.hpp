//
//  comparationMethods.hpp
//  ImageComparator
//
//  Created by Yizhou Li on 2/6/23.
//

#ifndef comparationMethods_hpp
#define comparationMethods_hpp

#include <stdio.h>
#include <string>

using namespace std;

int compare(string dbFilePath, string picFilePath, string matchingMethod, int numOfOutputs, string outputs[]);
int sum_of_squared_diff(vector<float> f1, vector<float> f2, float &rst);
int histogram_intersection(vector<float> f1, vector<float> f2, float &rst);

#endif /* comparationMethods_hpp */
