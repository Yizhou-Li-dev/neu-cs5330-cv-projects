//
//  main.cpp
//  cam_features
//
//  Created by Yizhou Li on 3/14/23.
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

// this program tries to identify the corners using Harris corners detection algorithm.
int main(int argc, const char * argv[]) {
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
    
    Mat src, dst, gray, dst_norm;

    while (true) {
        cap >> src;
        if (src.empty()) {
            printf("src is empty\n");
            break;
        }
  
        // see if there is a waiting keystroke
        char key = waitKey(25);
        if (key == 'q') {
            break;
        }
        
        // Haris corners detector
        cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        int blockSize = 5;
        int apertureSize = 3;
        double k = 0.04;
        cornerHarris(gray, dst, blockSize, apertureSize, k);
        normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
        for (int i = 0; i < dst_norm.rows; i++) {
            for (int j = 0; j < dst_norm.cols; j++) {
                if ((int) dst_norm.at<float>(i, j) > 100) {
                    circle(src, cv::Point(j, i), 1, cv::Scalar(0, 0, 255), 1, 8, 0);
                }
            }
        }
        
        imshow("video", src);
    }
    return 0;
}
