//
//  main.cpp
//  project4
//
//  Created by Yizhou Li on 3/9/23.
//

#include <iostream>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "utils.hpp"
using namespace std;
using namespace cv;

// this program tries to caliberate the camera. Press 2 to record previous caliberation image. Press 3 to caliberate images.
int main(int argc, const char * argv[]) {
    std::filesystem::create_directory("./caliberation_images/");
    
    VideoCapture cap(1);
    if(!cap.isOpened()) {
        printf("Unable to open video device");
        return 0;
    }
    // get some properties of the image
    cv::Size refS((int) cap.get(cv::CAP_PROP_FRAME_WIDTH),
                  (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);
    
    namedWindow("video");
    Mat src, dst, gray;
    
    // initialize some variables for chessboard pattern
    vector<Point2f> corners;
    vector<vector<Point2f>> img_corners_list;
    vector<Point3f> points;
    vector<vector<Point3f>> obj_corners_list;
    Size patternsize(9, 6); // hard coded interior width=9, height=6
    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
    
    while (true) {
        cap >> src;
        dst = src.clone();
        if(src.empty()) {
            printf("src is empty\n");
            break;
        }
        cvtColor(src, gray, COLOR_BGR2GRAY);
        corners.clear();
        bool patternfound = findChessboardCorners(gray, patternsize, corners,            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
        if (patternfound) {
            cornerSubPix(gray, corners, Size(5, 5), Size(-1, -1), termcrit);
            drawChessboardCorners(dst, patternsize, Mat(corners), patternfound);
            printf("the total number of corners: %lu\n", corners.size());
            printf("the first corner: (%f, %f)\n", corners[0].x, corners[0].y);
        }
        
        // see if there is a waiting keystroke
        char key = waitKey(25);
        
        if (key == 'q') {
            break;
        } else if (key == '2') {
            if (corners.size() != 0) {
                points.clear();
                for (int i = 0; i < patternsize.height; i++) {
                    for (int j = 0; j < patternsize.width; j++) {
                        points.push_back(Point3f(j, -i, 0));
                    }
                }
                
                img_corners_list.push_back(corners);
                obj_corners_list.push_back(points);
                cout << "print corners' image coords:" << endl;
                cout << corners << endl;
                cout << "print coeners' object coords:" << endl;
                cout << points << endl;
                imwrite("./caliberation_images/" + to_string(img_corners_list.size()) + ".jpg", dst);
            }
            printf("currently has %lu pre-trained images\n", img_corners_list.size());
        } else if (key == '3') {
            if (img_corners_list.size() >= 5) {
                Mat camera_matrix = Mat::eye(3, 3, CV_64FC1);
                camera_matrix.at<double>(0, 2) = src.cols/2;
                camera_matrix.at<double>(1, 2) = src.rows/2;
                
                Mat dist_coefficients, rvecs, tvecs, std_deviations_intrinsic, std_deviations_extrinsic, per_view_errors;
                // print some variables before the camera caliberation
                printf("Initial camera matrix is:\n");
                cout << camera_matrix << endl;
                printf("Initial distortion coefficients  is:\n");
                cout << dist_coefficients << endl;
                // do camera caliberation
                double reprojection_error = calibrateCamera(obj_corners_list, img_corners_list, src.size(), camera_matrix, dist_coefficients, rvecs, tvecs, std_deviations_intrinsic, std_deviations_extrinsic, per_view_errors, CALIB_FIX_ASPECT_RATIO, termcrit);
                // print some variables after the camera caliberation
                printf("Caliberated camera matrix is:\n");
                cout << camera_matrix << endl;
                printf("Caliberated distortion coefficients is:\n");
                cout << dist_coefficients << endl;
                cout << "the reprojection error is: " << reprojection_error << endl;
                save_cam_caliberation_info(camera_matrix, dist_coefficients, rvecs, tvecs);
            } else {
                printf("You have to specifiy atleast 5 images for camera caliberation.\n");
                printf("currently has %lu pre-trained images\n", img_corners_list.size());
            }
        }
        
        imshow("video", dst);
    }
    
    // todo: potentially destroy data created in program running
    //    std::filesystem::remove_all("./caliberation_images/");
    return 0;
}
