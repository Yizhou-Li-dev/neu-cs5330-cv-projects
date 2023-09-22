//
//  main.cpp
//  ImageDBGenerator
//
//  Created by Yizhou Li on 2/3/23.
//

#include <iostream>
#include "img_db_Op.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    if (argc < 3) {
      printf("not enough arguments. You have to provide target filename, image directory, feature type, matching method, and number of output images.\n");
      exit(-1);
    }
    string imageDirPath = argv[1],
           featureType = argv[2];
    printf("Note you provided following: arguments:\n\
        image directory path: %s\n\
        feature type: %s\n",
           imageDirPath.c_str(),
            featureType.c_str());
    writeToDB(imageDirPath, featureType);
    return 0;
}
