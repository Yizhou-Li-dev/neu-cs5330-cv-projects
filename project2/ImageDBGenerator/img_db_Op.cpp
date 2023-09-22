//
//  imageDatabaseOperation.cpp
//  project2
//
//  Created by Yizhou Li on 2/2/23.
//

#include "my_csv_util.hpp"
#include "img_db_Op.hpp"
#include "filter.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <cstdio>
#include <map>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <string>

using namespace std;
using namespace cv;

string dbFilePath = "/Users/zhou/Library/Mobile Documents/com~apple~CloudDocs/Yizhou/学校/研究生/NU课程/CS5330 CV/Projects/project2/";

int writeToDB(string imageDirPath, string featureType) {
    if (featureType == "baseline") {
        processBaseLine(imageDirPath);
    } else if (featureType == "rgChromaticity") {
        processRgChromaticity(imageDirPath);
    } else if (featureType == "mrgChromaticity") {
        processMultipleRgChromaticity(imageDirPath);
    } else if (featureType == "textureColor") {
        processTextureColor(imageDirPath);
    } else if (featureType == "custom") {
        processCustom(imageDirPath);
    }
    return 0;
}

int processBaseLine(string imageDirPath) {
    char dirname[256];
    char imagePath[256];
    DIR *dirp;
    struct dirent *dp;
    
    strcpy(dirname, imageDirPath.c_str());
    
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    
    dbFilePath += "baselineDB.csv";
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
           strstr(dp->d_name, ".png") ||
           strstr(dp->d_name, ".ppm") ||
           strstr(dp->d_name, ".tif")) {
            strcpy(imagePath, dirname);
            strcat(imagePath, dp->d_name);
            printf("processing image file: %s\n", imagePath);
            
            Mat img = imread(imagePath);
            
            vector<float> vec;
            for (int r = 0; r < img.rows; r++) {
                for (int c = 0; c < img.cols; c++) {
                    if (r >= img.rows/2 - 4 &&
                        r <= img.rows/2 + 4 &&
                        c >= img.cols/2 - 4 &&
                        c <= img.cols/2 + 4) {
                        for (int chl = 0; chl < img.channels(); chl++) {
                            vec.push_back(img.at<Vec3b>(r, c)[chl]);
                        }
                    }
                }
            }
            
            write_csv(dbFilePath, {imagePath, vec});
        }
    }
    return 0;
}

int processRgChromaticity(string imageDirPath) {
    char dirname[256];
    char imagePath[256];
    DIR *dirp;
    struct dirent *dp;
    
    strcpy(dirname, imageDirPath.c_str());
    
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    
    dbFilePath += "rgDB.csv";
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
           strstr(dp->d_name, ".png") ||
           strstr(dp->d_name, ".ppm") ||
           strstr(dp->d_name, ".tif")) {
            strcpy(imagePath, dirname);
            strcat(imagePath, dp->d_name);
            printf("processing image file: %s\n", imagePath);
            
            Mat img = imread(imagePath);
            
            auto hSize = 8; // we hard code bin number 8 for now
            auto binDivisor = 256 / hSize;
            int dim[3] = {hSize, hSize, hSize};
            Mat histogram = Mat::zeros(3, dim, CV_32S);

            for (int r = 0; r < img.rows; r++) { // build 3d histogram
                for (int c = 0; c < img.cols; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram.at<int>(re, gr, bl)++;
                }
            }
            
            // flattten the histogram into vector and record it into db
            vector<float> tmpVector;
            for (int r = 0; r < hSize; r++) {
                for (int g = 0; g < hSize; g++) {
                    for (int b = 0; b < hSize; b++) {
                        tmpVector.push_back(histogram.at<Vec3b>(r, g)[b]);
                    }
                }
            }
            
            write_csv(dbFilePath, {imagePath, tmpVector});
        }
    }
    return 0;
}

int processMultipleRgChromaticity(string imageDirPath) {
    char dirname[256];
    char imagePath[256];
    DIR *dirp;
    struct dirent *dp;
    
    strcpy(dirname, imageDirPath.c_str());
    
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    
    dbFilePath += "mrgDB.csv";
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
           strstr(dp->d_name, ".png") ||
           strstr(dp->d_name, ".ppm") ||
           strstr(dp->d_name, ".tif")) {
            strcpy(imagePath, dirname);
            strcat(imagePath, dp->d_name);
            printf("processing image file: %s\n", imagePath);
            
            Mat img = imread(imagePath);
            
            auto hSize = 8; // we hard code bin number 8 for now
            auto binDivisor = 256 / hSize;
            int dim[3] = {hSize, hSize, hSize};
            
            Mat histogram1 = Mat::zeros(3, dim, CV_32S);
            for (int r = 0; r < img.rows/2; r++) { // top half rgb histogram
                for (int c = 0; c < img.cols/2; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram1.at<int>(re, gr, bl)++;
                }
            }
            
            Mat histogram2 = Mat::zeros(3, dim, CV_32S);
            for (int r = img.rows/2; r < img.rows; r++) { // bottom half rgb histogram
                for (int c = img.cols/2; c < img.cols; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram2.at<int>(re, gr, bl)++;
                }
            }
            
            // flattten the histogram into vector and record it into db
            vector<float> tmpVector;
            for (int r = 0; r < hSize; r++) {
                for (int g = 0; g < hSize; g++) {
                    for (int b = 0; b < hSize; b++) {
                        auto n1 = histogram1.at<Vec3b>(r, g)[b];
                        auto n2 = histogram2.at<Vec3b>(r, g)[b];
                        tmpVector.push_back(n1 + n2);
                    }
                }
            }
            
            write_csv(dbFilePath, {imagePath, tmpVector});
        }
    }
    return 0;
}

int processTextureColor(string imageDirPath) {
    char dirname[256];
    char imagePath[256];
    DIR *dirp;
    struct dirent *dp;
    
    strcpy(dirname, imageDirPath.c_str());
    
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    
    dbFilePath += "textureColorDB.csv";
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
           strstr(dp->d_name, ".png") ||
           strstr(dp->d_name, ".ppm") ||
           strstr(dp->d_name, ".tif")) {
            strcpy(imagePath, dirname);
            strcat(imagePath, dp->d_name);
            printf("processing image file: %s\n", imagePath);
            
            Mat img = imread(imagePath);
            
            auto hSize = 8; // we hard code bin number 8 for now
            auto binDivisor = 256 / hSize;
            int dim[3] = {hSize, hSize, hSize};
            
            Mat histogram1 = Mat::zeros(3, dim, CV_32S);
            for (int r = 0; r < img.rows; r++) {
                for (int c = 0; c < img.cols; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram1.at<int>(re, gr, bl)++;
                }
            }
            
            Mat mid = Mat::zeros(img.rows, img.cols, CV_16SC3);
            Mat sobel_filtered_Ximage = Mat::zeros(img.rows, img.cols, CV_16SC3);
            Mat sobel_filtered_Yimage = Mat::zeros(img.rows, img.cols, CV_16SC3);
            sobelX3x3(img, sobel_filtered_Ximage);
            sobelY3x3(img, sobel_filtered_Yimage);
            magnitude(sobel_filtered_Ximage, sobel_filtered_Yimage, mid);
            
            Mat histogram2 = Mat::zeros(3, dim, CV_32S);
            for (int r = 0; r < img.rows; r++) {
                for (int c = 0; c < img.cols; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram2.at<int>(re, gr, bl)++;
                }
            }
            
            // flattten the histogram into vector and record it into db
            vector<float> tmpVector;
            for (int r = 0; r < hSize; r++) {
                for (int g = 0; g < hSize; g++) {
                    for (int b = 0; b < hSize; b++) {
                        auto n1 = histogram1.at<Vec3b>(r, g)[b];
                        auto n2 = histogram2.at<Vec3b>(r, g)[b];
                        tmpVector.push_back(n1 + n2);
                    }
                }
            }
            
            write_csv(dbFilePath, {imagePath, tmpVector});
        }
    }
    return 0;
}

int processCustom(string imageDirPath) {
    char dirname[256];
    char imagePath[256];
    DIR *dirp;
    struct dirent *dp;
    
    strcpy(dirname, imageDirPath.c_str());
    
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    
    dbFilePath += "customDB.csv";
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
           strstr(dp->d_name, ".png") ||
           strstr(dp->d_name, ".ppm") ||
           strstr(dp->d_name, ".tif") ||
           strstr(dp->d_name, ".jpeg")) {
            strcpy(imagePath, dirname);
            strcat(imagePath, dp->d_name);
            printf("processing image file: %s\n", imagePath);
            
            Mat img = imread(imagePath);
            
            auto hSize = 8; // we hard code bin number 8 for now
            auto binDivisor = 256 / hSize;
            int dim[3] = {hSize, hSize, hSize};
            
            Mat histogram1 = Mat::zeros(3, dim, CV_32S);
            for (int r = 0; r < img.rows; r++) {
                for (int c = 0; c < img.cols; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram1.at<int>(re, gr, bl)++;
                }
            }
            
            Mat mid = Mat::zeros(img.rows, img.cols, CV_16SC3);
            Mat sobel_filtered_Ximage = Mat::zeros(img.rows, img.cols, CV_16SC3);
            Mat sobel_filtered_Yimage = Mat::zeros(img.rows, img.cols, CV_16SC3);
            sobelX3x3(img, sobel_filtered_Ximage);
            sobelY3x3(img, sobel_filtered_Yimage);
            magnitude(sobel_filtered_Ximage, sobel_filtered_Yimage, mid);
            
            Mat histogram2 = Mat::zeros(3, dim, CV_32S);
            for (int r = 0; r < img.rows; r++) {
                for (int c = 0; c < img.cols; c++) {
                    auto bl = img.at<Vec3b>(r, c)[0] / binDivisor;
                    auto gr = img.at<Vec3b>(r, c)[1] / binDivisor;
                    auto re = img.at<Vec3b>(r, c)[2] / binDivisor;
                    histogram2.at<int>(re, gr, bl)++;
                }
            }
            
            // flattten the histogram into vector and record it into db
            vector<float> tmpVector;
            for (int r = 0; r < hSize; r++) {
                for (int g = 0; g < hSize; g++) {
                    for (int b = 0; b < hSize; b++) {
                        auto n1 = histogram1.at<Vec3b>(r, g)[b];
                        auto n2 = histogram2.at<Vec3b>(r, g)[b];
                        tmpVector.push_back(n1 + n2);
                    }
                }
            }
            
            write_csv(dbFilePath, {imagePath, tmpVector});
        }
    }
    return 0;
}
