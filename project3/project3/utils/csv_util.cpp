//
//  my_csv_util.cpp
//  ImageDBGenerator
//
//  Created by Yizhou Li on 2/3/23.
//

#include "csv_util.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

int write_csv(string db_file_path, pair<string, vector<double>> dataset) {
    ofstream myFile;
    myFile.open(db_file_path, ios::out | ios::app);
    
    myFile << dataset.first << ",";
    for (int i = 0; i < dataset.second.size(); i++) {
        if (i != dataset.second.size() - 1) {
            myFile << dataset.second[i] << ",";
        } else {
            myFile << dataset.second[i] << "\n";
        }
    }
    myFile.close();
    return 0;
}

int write_csv_tasl_raw_pics(string db_file_path, string file_name, string obj_type, vector<int> data) {
    ofstream myFile;
    myFile.open(db_file_path, ios::out | ios::app);
    
    myFile << file_name << ",";
    myFile << obj_type << ",";
    for (int i = 0; i < data.size(); i++) {
        if (i != data.size() - 1) {
            myFile << data[i] << ",";
        } else {
            myFile << data[i] << "\n";
        }
    }
    myFile.close();
    return 0;
}

int read_csv_bylines(string dbfilePath, vector<pair<string, vector<double>>> &rst) {
    string line, word;
    fstream myFile(dbfilePath, ios::in);
    
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            stringstream str(line);
            
            // the first element will be its picture name
            getline(str, word, ',');
            auto tmpPair = pair<string, vector<double>>();
            tmpPair.first = word;
            
            vector<double> tmpVec;
            while(getline(str, word, ',')) {
                tmpVec.push_back(stof(word));
            }
            tmpPair.second = tmpVec;
            rst.push_back(tmpPair);
        }
    } else {
        printf("Could not open the file\n");
    }
    return 0;
}
