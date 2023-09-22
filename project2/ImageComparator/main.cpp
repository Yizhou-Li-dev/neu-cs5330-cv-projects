//
//  main.cpp
//  project2
//
//  Created by Yizhou Li on 2/2/23.
//

#include <iostream>
#include "comp_methods.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

int main(int argc, const char * argv[]) {
    string dbFilePath = "/Users/zhou/Library/Mobile Documents/com~apple~CloudDocs/Yizhou/学校/研究生/NU课程/CS5330 CV/Projects/project2/";
    
    if (argc < 5) {
      printf("not enough arguments. You have to provide target filename, feature type, matching method, and number of output images.\n");
      exit(-1);
    }
    string targetFilePath = argv[1],
           featureType = argv[2],
           matchingMethod = argv[3],
           numOfOutputs = argv[4];
    printf("Note you provided following arguments:\n\
        target file path: %s\n\
        feature type: %s\n\
        matching method: %s\n\
        number of output images: %s\n", targetFilePath.c_str(), featureType.c_str(), matchingMethod.c_str(), numOfOutputs.c_str());
    
    string outputs[stoi(numOfOutputs)];
    if (featureType == "baseline") {
        compare(dbFilePath + "baselineDB.csv", targetFilePath, matchingMethod, stoi(numOfOutputs), outputs);
    } else if (featureType == "rgChromaticity") {
        compare(dbFilePath + "rgDB.csv", targetFilePath,  matchingMethod, stoi(numOfOutputs), outputs);
    } else if (featureType == "mrgChromaticity") {
        compare(dbFilePath + "mrgDB.csv", targetFilePath,  matchingMethod, stoi(numOfOutputs), outputs);
    } else if (featureType == "textureColor") {
        compare(dbFilePath + "textureColorDB.csv", targetFilePath,  matchingMethod, stoi(numOfOutputs), outputs);
    } else if (featureType == "custom") {
        compare(dbFilePath + "customDB.csv", targetFilePath,  matchingMethod, stoi(numOfOutputs), outputs);
    }
   
    for (auto it : outputs) {
        cout << "image: " << it << endl;
    }
    
    return 0;
}
