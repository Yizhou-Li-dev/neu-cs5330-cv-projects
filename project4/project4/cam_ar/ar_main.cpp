//
//  main.cpp
//  cam_extra
//
//  Created by Yizhou Li on 3/11/23.
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

// this program tries to build AR system on top of the chessboard plane. Press 5 to show the 3D axes. Press 6 to show the pre-build 3D object phantom axe.
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
    
    Size patternsize(9, 6); //interior number of corners
    Mat src, dst, gray, camera_matrix, dist_coefficients, rvecs, tvecs;
    read_cam_caliberation_info(camera_matrix, dist_coefficients);
    char lastkey = 'n';
    while (true) {
        cap >> src;
        dst = src.clone();
        if (src.empty()) {
            printf("src is empty\n");
            break;
        }
  
        cvtColor(src, gray, COLOR_BGR2GRAY);
 
        vector<Point2f> corners;
        vector<Point3f> points;
        for (int i = 0; i < patternsize.height; i++) {
            for (int j = 0; j < patternsize.width; j++) {
                points.push_back(Point3f(j, -i, 0));
            }
        }
        
        bool patternfound = findChessboardCorners(gray, patternsize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                                                  + CALIB_CB_FAST_CHECK);
        if (patternfound) {
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1));
            drawChessboardCorners(dst, patternsize, Mat(corners), patternfound);
            printf("the total number of corners: %lu\n", corners.size());
            printf("the first corner: (%f, %f)\n", corners[0].x, corners[0].y);
            
            solvePnP(points, corners, camera_matrix, dist_coefficients, rvecs, tvecs);
            
            // print rvecs and tvecs in realtime
            cout << "rvecs:" << endl;
            cout << rvecs << endl;
            cout << "tvecs:" << endl;
            cout << tvecs << endl;
        }
        
        // see if there is a waiting keystroke
        char key = waitKey(25);
        if(key == -1){
            key = lastkey;
        }
        vector<Point2f> img_points;
        vector<Point3f> obj_points;
        if (key == 'q') {
            break;
        } else if (key == '5') {
            if (patternfound) {
                obj_points.push_back(Point3f(0, 0, 0));
                obj_points.push_back(Point3f(10, 0, 0));
                obj_points.push_back(Point3f(0, -10, 0));
                obj_points.push_back(Point3f(0, 0, 10));
                projectPoints(obj_points, rvecs, tvecs, camera_matrix, dist_coefficients, img_points);
                line(dst, Point(img_points[0].x, img_points[0].y), Point(img_points[1].x, img_points[1].y), Scalar(255, 0, 0), 5);
                line(dst, Point(img_points[0].x, img_points[0].y), Point(img_points[2].x, img_points[2].y), Scalar(255, 0, 0), 5);
                line(dst, Point(img_points[0].x, img_points[0].y), Point(img_points[3].x, img_points[3].y), Scalar(255, 0, 0), 5);
                lastkey = '5';
            }
        } else if (key == '6') {
            // we are going to construct an axe.
            float center_x = 2;
            float center_y = -2;
            float center_z = 2;
            float axe_height = 10;
            float axe_body_width = .5;
            float axe_bar_width = 8;
            float axe_bar_height = 3;
            
            // axe's body
            obj_points.push_back(Point3f(center_x - axe_body_width, center_y + axe_body_width, center_z));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y - axe_body_width, center_z));
            obj_points.push_back(Point3f(center_x - axe_body_width, center_y + axe_body_width, center_z + axe_height));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y - axe_body_width, center_z + axe_height));
            
            obj_points.push_back(Point3f(center_x - axe_body_width, center_y + axe_body_width, center_z));
            obj_points.push_back(Point3f(center_x - axe_body_width, center_y + axe_body_width, center_z + axe_height));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y + axe_body_width, center_z));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y + axe_body_width, center_z + axe_height));
            obj_points.push_back(Point3f(center_x - axe_body_width, center_y - axe_body_width, center_z));
            obj_points.push_back(Point3f(center_x - axe_body_width, center_y - axe_body_width, center_z + axe_height));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y - axe_body_width, center_z));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y - axe_body_width, center_z + axe_height));
            
            // axe's head
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y, center_z + axe_height));
            obj_points.push_back(Point3f(center_x + axe_body_width + axe_bar_width, center_y, center_z + axe_height));
            obj_points.push_back(Point3f(center_x + axe_body_width + axe_bar_width, center_y, center_z + axe_height));
            obj_points.push_back(Point3f(center_x + axe_body_width + axe_bar_width, center_y, center_z + axe_bar_height));
            obj_points.push_back(Point3f(center_x + axe_body_width + axe_bar_width, center_y, center_z + axe_bar_height));
            obj_points.push_back(Point3f(center_x + axe_body_width, center_y, center_z + axe_bar_height));
            
            projectPoints(obj_points, rvecs, tvecs, camera_matrix, dist_coefficients, img_points);
            
            int random1, random2, random3;
            random1 = rand() % 255;
            random2 = rand() % 255;
            random3 = rand() % 255;
            rectangle(dst, Point(img_points[0].x, img_points[0].y), Point(img_points[1].x, img_points[1].y), Scalar(random1, random2, random3), 10);
            random1 = rand() % 255;
            random2 = rand() % 255;
            random3 = rand() % 255;
            rectangle(dst, Point(img_points[2].x, img_points[2].y), Point(img_points[3].x, img_points[3].y), Scalar(random1, random2, random3), 10);
            
            for (int i = 4; i < obj_points.size(); i += 2) {
                random1 = rand() % 255;
                random2 = rand() % 255;
                random3 = rand() % 255;

                line(dst, Point(img_points[i].x, img_points[i].y), Point(img_points[i + 1].x, img_points[i + 1].y), Scalar(random1, random2, random3), 20);
            }
            lastkey = '6';
        }
        
        imshow("video", dst);
    }
    return 0;
}
