//
//  utils.hpp
//  project4
//
//  Created by Yizhou Li on 3/9/23.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include <opencv2/core.hpp>

using namespace cv;

/**
 @brief This function saves the camera's parameters.
 
 Intrinsic parameters such as camera matrix and distortion coefficients and extrinsic parameters such as rotation vectors and translation vectors are stored in a yaml file
 */
void save_cam_caliberation_info(Mat camera_matrix, Mat dist_coefficients, Mat rvecs, Mat tvecs);
/**
 @brief This function reads the camera's parameters.
 
 Intrinsic parameters such as camera matrix and distortion coefficients are read from a yaml file
 */
void read_cam_caliberation_info(Mat &camera_matrix, Mat &dist_coefficients);

#endif /* utils_hpp */
