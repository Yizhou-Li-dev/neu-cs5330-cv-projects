//
//  filter.cpp
//  project1
//
//  Created by Yizhou Li on 1/2/23.
//

#include <iostream>
#include "filter.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int greyscale(Mat &src, Mat &dst) {
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            cv::Vec3b& srcPixel = src.at<cv::Vec3b>(i, j);
            cv::Vec3b& dstPixel = dst.at<cv::Vec3b>(i, j);
            auto greyPixel = srcPixel[0] * 0.144 + srcPixel[1] * 0.587 + srcPixel[2] * 0.299;

            dstPixel[0] = greyPixel;
            dstPixel[1] = greyPixel;
            dstPixel[2] = greyPixel;
        }
    }
    return 0;
}

int blur5x5(Mat &src, Mat &dst) {
    int filter[] = {1, 2, 4, 2, 1};

    // first pass, filter the imaage horizontally
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            cv::Vec3b dstPixel = dst.at<Vec3b>(i, j);
            auto bVal = 0, gVal = 0, rVal = 0, fNum = 10;
            for (int k = -2; k < 3; k++) {
                if (i + k == -2 || i + k == src.rows + 1) {
                    fNum = 7;
                } else if (i + k == -1 || i + k == src.rows) {
                    fNum = 9;
                } else {
                    bVal += src.at<Vec3b>(i + k, j)[0] * filter[k + 2];
                    gVal += src.at<Vec3b>(i + k, j)[1] * filter[k + 2];
                    rVal += src.at<Vec3b>(i + k, j)[2] * filter[k + 2];
                }
            }
            dstPixel[0] = bVal / fNum;
            dstPixel[1] = gVal / fNum;
            dstPixel[2] = rVal / fNum;
        }
    }

    // second pass, filter the imaage vertically
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            Vec3b dstPixel = dst.at<Vec3b>(i, j);
            auto bVal = 0, gVal = 0, rVal = 0, fNum = 10;
            for (int k = -2; k < 3; k++) {
                if (j + k == -2 || j + k == src.cols + 1) {
                    fNum = 7;
                } else if (j + k == -1 || j + k == src.cols) {
                    fNum = 9;
                } else {
                    bVal += src.at<Vec3b>(i, j + k)[0] * filter[k + 2];
                    gVal += src.at<Vec3b>(i, j + k)[1] * filter[k + 2];
                    rVal += src.at<Vec3b>(i, j + k)[2] * filter[k + 2];
                }
            }
            dstPixel[0] = bVal / fNum;
            dstPixel[1] = gVal / fNum;
            dstPixel[2] = rVal / fNum;
        }
    }

    return 0;
}

void sobelHelper(Mat &src, Mat &dst, int H[3], int V[3], int norms[2]) {
    Mat mid = Mat::zeros(src.rows, src.cols, CV_16SC3);

    // horizontol
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols; j++){
            if(i < 1 || i > src.rows - 2 || j < 1 || j > src.cols - 2){
                mid.at<Vec3s>(i, j) = src.at<Vec3b>(i, j);
            } else {
                for(int chl = 0; chl < 3; chl++){
                    int temp = 0;
                    for(int k = 0; k < 3; k++){
                        temp += src.at<Vec3b>(i, j + k - 1)[chl] * H[k];
                    }
                    mid.at<Vec3s>(i, j)[chl] = temp / norms[0];
                }
            }
        }
    }

    // vertical
    for(int j = 0; j < src.cols; j++){
        for(int i = 0; i < src.rows; i++){
            if(i < 1 || i > src.rows - 2 || j < 1 || j > src.cols - 2){
                dst.at<Vec3s>(i, j) = mid.at<Vec3b>(i, j);
            } else {
                for(int chl = 0; chl < 3; chl++){
                    int temp = 0;
                    for(int k = 0; k < 3; k++){
                        temp += mid.at<Vec3s>(i + k - 1, j)[chl] * V[k];
                    }
                    dst.at<Vec3s>(i, j)[chl] = temp / norms[1];
                }
            }
        }
    }
}

int sobelX3x3(Mat &src, Mat &dst) {
    int H[3] = {1, 2, 1};
    int V[3] = {1, 0, -1};
    int norms[2] = {4, 1};
    sobelHelper(src, dst, H, V, norms);
    return 0;
}

int sobelY3x3(Mat &src, Mat &dst) {
    int H[3] = {1, 0, -1};
    int V[3] = {1, 2, 1};
    int norms[2] = {1, 4};
    sobelHelper(src, dst, H, V, norms);
    return 0;
}

int magnitude(Mat &sx, Mat &sy, Mat &dst) {
    for (int i = 0; i < sx.rows; i++) {
        for (int j = 0; j < sx.cols; j++) {
            Vec3s sxPixel = sx.at<Vec3s>(i, j);
            Vec3s syPixel = sy.at<Vec3s>(i, j);
            dst.at<Vec3b>(i, j) = Vec3b(
                                        sqrt(sxPixel[0] * sxPixel[0] + syPixel[0] * syPixel[0]),
                                        sqrt(sxPixel[1] * sxPixel[1] + syPixel[1] * syPixel[1]),
                                        sqrt(sxPixel[2] * sxPixel[2] + syPixel[2] * syPixel[2]));
        }
    }
    return 0;
}

int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels) {
    blur5x5(src, dst);
    int b = 255/levels;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            for (int chl = 0; chl < src.channels(); chl++) {
                auto xt = dst.at<Vec3b>(i, j)[chl] / b;
                auto xf = xt * b;
                dst.at<Vec3b>(i, j)[chl] = xf;
            }
        }
    }
    return 0;
}

int cartoon(cv::Mat &src, cv::Mat &dst, int levels, int magThreshold) {
    Mat gradient = Mat::zeros(src.rows, src.cols, CV_8UC3);
    Mat sobel_filtered_Ximage = Mat::zeros(src.rows, src.cols, CV_16SC3);
    Mat sobel_filtered_Yimage = Mat::zeros(src.rows, src.cols, CV_16SC3);

    sobelX3x3(src, sobel_filtered_Ximage);
    sobelY3x3(src, sobel_filtered_Yimage);

    magnitude(sobel_filtered_Ximage, sobel_filtered_Yimage, gradient);
    blurQuantize(src, dst, levels);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            for (int chl = 0; chl < src.channels(); chl++) {
                if (gradient.at<Vec3b>(i, j)[chl] > magThreshold) {
                    dst.at<Vec3b>(i, j)[chl] = 0;
                }
            }
        }
    }
    return 0;
}

int changeBrightness(Mat &src, Mat &dst) {
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, .4) * 255.0);
    cv::Mat res = src.clone();
    LUT(src, lookUpTable, res);
    dst = res;
    return 0;
}

int alpha_slider = 0;
int alpha_slider_max = 100;
Mat trackBarSrc, trackBarDst;

void on_trackbar(int value, void *userdata) {
    Mat dst = *((Mat*)&userdata);
    Canny(trackBarSrc, trackBarDst, 0, alpha_slider);
    dst = trackBarDst;
}

int trackBar(Mat &src, Mat &dst) {
    trackBarSrc = src.clone();
    trackBarDst = dst.clone();

    char TrackbarName[50];
    sprintf(TrackbarName, "Alpha x %d", alpha_slider_max);
    cv::createTrackbar(TrackbarName, "Video", &alpha_slider, alpha_slider_max, on_trackbar, &dst);
//    dst = trackBarDst;
    return 0;
}


