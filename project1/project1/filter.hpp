//
//  filter.hpp
//  project1
//
//  Created by Yizhou Li on 1/2/23.
//

#ifndef filter_hpp
#define filter_hpp

#include <stdio.h>
#include <opencv2/core.hpp>

int greyscale( cv::Mat &src, cv::Mat &dst );
int blur5x5( cv::Mat &src, cv::Mat &dst );
int sobelX3x3( cv::Mat &src, cv::Mat &dst );
int sobelY3x3( cv::Mat &src, cv::Mat &dst );
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels );
int cartoon( cv::Mat &src, cv::Mat &dst, int levels, int magThreshold );
int changeBrightness(cv::Mat &src, cv::Mat&dst);
int trackBar(cv::Mat &src, cv::Mat&dst);

#endif /* filter_hpp */
