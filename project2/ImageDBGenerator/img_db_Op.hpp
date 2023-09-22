//
//  imageDatabaseOperation.hpp
//  project2
//
//  Created by Yizhou Li on 2/2/23.
//

#ifndef imageDatabaseOperation_hpp
#define imageDatabaseOperation_hpp

#include <stdio.h>
#include <string>
using namespace std;

int writeToDB(string imageDirPath, string featureType);
int processBaseLine(string imageDirPath);
int processRgChromaticity(string imageDirPath);
int processMultipleRgChromaticity(string imageDirPath);
int processTextureColor(string imageDirPath);
int processCustom(string imageDirPath);

#endif /* imageDatabaseOperation_hpp */
