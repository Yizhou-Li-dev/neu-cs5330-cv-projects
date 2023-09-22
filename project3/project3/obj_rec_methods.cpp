//
//  obj_rec_methods.cpp
//  porject3
//
//  Created by Yizhou Li on 2/14/23.
//

#include "obj_rec_methods.hpp"
#include <map>
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace cv;

int threshold_image(Mat &src, Mat &dst) {
    cvtColor(src, dst, COLOR_BGR2GRAY);
    src = dst.clone();
    
    blur(src, dst, Size(5,5));
    src = dst.clone();
    blur(src, dst, Size(5,5));
    src = dst.clone();
    blur(src, dst, Size(5,5));
    src = dst.clone();
  
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if ((int) src.at<uchar>(i,j) <= 130) {
                dst.at<uchar>(i,j) = 255;
            } else {
                dst.at<uchar>(i,j) = 0;
            }
        }
    }
    
    return 0;
}

int custom_morphological_filtering(Mat &src, Mat &dst) {
    Mat mid = dst.clone();
    morphologyEx(src, mid, MORPH_OPEN, getStructuringElement(MorphShapes::MORPH_RECT, Size(4, 4)));
    morphologyEx(mid, dst, MORPH_CLOSE, getStructuringElement(MorphShapes::MORPH_RECT, Size(4, 4)));
    return 0;
}

int connected_components_analysis(Mat &dst, Mat &stats, int n) {
    Mat centroids;
    Mat lables;
    map<int, vector<int>> my_map;
    
    int labelSize = connectedComponentsWithStats(dst, lables, stats, centroids);
    // collect region info to a map
    for (int i = 0; i < labelSize; i++) {
        int x = stats.at<int>(Point(0, i));
        int y = stats.at<int>(Point(1, i));
        int w = stats.at<int>(Point(2, i));
        int h = stats.at<int>(Point(3, i));
        
        if (!(w < 20 && h < 20)) {
            my_map[w * h] = vector<int> {i, x, y, w, h};
        }
    }
    
    // print the region number and blocks
    if (n != 0) {
        labelSize = n;
    }
    int counter = 0;
    for (auto itr = my_map.end(); itr != my_map.begin(); itr--) {
        Scalar color(rand()%(255+1), rand()%(255+1), rand()%(255+1));
        Rect rect(itr->second[1],itr->second[2],itr->second[3],itr->second[4]);
        rectangle(dst, rect, color);
        putText(dst, to_string(itr->second[0]), Point(itr->second[1], itr->second[2]), FONT_HERSHEY_DUPLEX, 1, color, 2);
        
        counter++;
        if (counter == labelSize) {
            break;
        }
    }
    return labelSize;
}

int get_features_from_a_region(Mat &regionMap, int regionId, int label_size, Mat &stats, vector<double> &features) {
    // do image cropping according to regionID
    for (int i = 0; i < label_size; i++) {
        if (i == regionId) {
            int x = stats.at<int>(Point(0, i));
            int y = stats.at<int>(Point(1, i));
            int w = stats.at<int>(Point(2, i));
            int h = stats.at<int>(Point(3, i));
            
            // reconstruct the region map
            for (int r = 0; r < regionMap.rows; r++) {
                for (int c = 0; c < regionMap.cols; c++) {
                    if (r >= y + h || r <= y || c <= x || c >= x + w) {
                        regionMap.at<uchar>(r, c) = 0;
                    }
                }
            }
        }
    }
        
    Moments moments = cv::moments(regionMap, true);
    double hu[7];
    HuMoments(moments, hu);
    features.emplace_back(hu[0]);
    features.emplace_back(hu[1]);
    features.emplace_back(hu[2]);
    
    return 0;
}
