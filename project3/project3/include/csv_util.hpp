//
//  my_csv_util.hpp
//  ImageDBGenerator
//
//  Created by Yizhou Li on 2/3/23.
//

#ifndef csv_util_hpp
#define csv_util_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

int write_csv(string db_file_path, pair<string, vector<double>> dataset);
int read_csv_bylines(string dbfilePath, vector<pair<string, vector<double>>> &rst);
int write_csv_tasl_raw_pics(string db_file_path, string file_name, string obj_type, vector<int> data);

#endif /* csv_util_hpp */
