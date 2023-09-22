//
//  my_csv_util.hpp
//  ImageDBGenerator
//
//  Created by Yizhou Li on 2/3/23.
//

#ifndef my_csv_util_hpp
#define my_csv_util_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

int write_csv(string filename, pair<string, vector<float>> dataset);
int read_csv_bypic(string filename, string picname, vector<float> &rst);
int read_csv_bylines(string filename, vector<pair<string, vector<float>>> &rst);

#endif /* my_csv_util_hpp */
