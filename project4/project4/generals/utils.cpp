//
//  utils.cpp
//  project4
//
//  Created by Yizhou Li on 3/9/23.
//

#include "utils.hpp"

void save_cam_caliberation_info(Mat camera_matrix, Mat dist_coefficients, Mat rvecs, Mat tvecs) {
    FileStorage fs("./caliberation_info.yaml", FileStorage::WRITE);
    fs << "camera_matrix" << camera_matrix;
    fs << "dist_coefficients" << dist_coefficients;
    fs << "rvecs" << rvecs;
    fs << "tvecs" << tvecs;
}

void read_cam_caliberation_info(Mat &camera_matrix, Mat &dist_coefficients) {
    FileStorage fs("./caliberation_info.yaml", FileStorage::READ);
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coefficients"] >> dist_coefficients;
//    fs["rvecs"] >> rvecs;
//    fs["tvecs"] >> tvecs;
}
