//
//  my_csv_util.cpp
//  ImageDBGenerator
//
//  Created by Yizhou Li on 2/3/23.
//

#include "my_csv_util.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

int write_csv(string filename, pair<string, vector<float>> dataset) {
    ofstream myFile;
    myFile.open(filename, ios::out | ios::app);
    
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

//int write_csv_byrg(string filename, map<int, int> map, string imagePath) {
//    ofstream myFile;
//    myFile.open(filename, ios::out | ios::app);
//    
//    myFile << imagePath << ",";
//    for (int i = 0; i < 256; i++) {
//        if (i == 255) {
//            myFile << map[i] << "\n";
//        } else {
//            myFile << map[i] << ",";
//        }
//    }
//    
//    myFile.close();
//    return 0;
//}

int read_csv_bypic(string filename, string picname, vector<float> &rst) {
    string line, word;
    fstream myFile (filename, ios::in);
    
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            stringstream str(line);
            
            // check the picture name
            getline(str, word, ',');
            if (word == picname) {
                while(getline(str, word, ',')) {
                    rst.push_back(stof(word));
                }
            }
        }
    } else {
        printf("Could not open the file\n");
    }
    return 0;
}

int read_csv_bylines(string filename, vector<pair<string, vector<float>>> &rst) {
    string line, word;
    fstream myFile(filename, ios::in);
    
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            stringstream str(line);
            
            // the first element will be its picture name
            getline(str, word, ',');
            auto tmpPair = pair<string, vector<float>>();
            tmpPair.first = word;
            
            vector<float> tmpVec;
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
