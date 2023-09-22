//
//  general_utils.hpp
//  porject3
//
//  Created by Yizhou Li on 2/28/23.
//

#ifndef general_utils_hpp
#define general_utils_hpp

#include <stdio.h>
#include <vector>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#define BACK_GROUND 0
#define FRONT_GROUND 255
#define THRESHOLD 100

double StandardDeviation(std::vector<double> samples);
double Variance(std::vector<double> samples);
double scaled_euclidian_distance(std::vector<double> f1, std::vector<double> f2, std::vector<double> stds);
void mark_object(cv::Mat &segmented_img, std::vector<cv::Point> draw_vertices);
int features(cv::Mat &src, std::vector<double> &feature_vector, std::vector<cv::Point> &draw_vertices);

#endif /* general_utils_hpp */
