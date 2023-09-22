//
//  vidDisplay.cpp
//  project1
//
//  Created by Yizhou Li on 12/25/22.
//

#include "vidDisplay.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void startVid() {
    VideoCapture *capdev;

    // open the video device
    capdev = new VideoCapture(0);
    if( !capdev->isOpened() ) {
            printf("Unable to open video device\n");
            return;
    }

    // get some properties of the image
    Size refS( (int) capdev->get(CAP_PROP_FRAME_WIDTH),
                   (int) capdev->get(CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    namedWindow("Video", 1); // identifies a window
    Mat src;
    Mat dst;
    char lastKey = 's';

    for(;;) {
        *capdev >> src; // get a new src from the camera, treat as a stream
        if(src.empty()) {
          printf("src is empty\n");
          break;
        }

        // see if there is a waiting keystroke
        char key = waitKey(10);
        if (key == -1) {
            key = lastKey;
        }
        
        string filePrefix = "/Users/zhou/Library/Mobile Documents/com~apple~CloudDocs/Yizhou/学校/研究生/NU课程/CS5330 CV/project1/project1/resources/";
        
        dst = src.clone();
        if (key == 'q') {
            break;
        } else if (key == 's') {
            imwrite(filePrefix + "part2_generated_image.png", dst);
            setWindowTitle("Video", "Normal Mode");
            lastKey = 's';
        } else if (key == 'g') {
            cvtColor(src, dst, COLOR_RGB2GRAY);
            imwrite(filePrefix + "part3_generated_image.png", dst);
            setWindowTitle("Video", "Grayscale Mode by Opencv");
            lastKey = 'g';
        } else if (key == 'h') {
            greyscale(src, dst);
            imwrite(filePrefix + "part4_generated_image.png", dst);
            setWindowTitle("Video", "Grayscale Mode by Me");
            lastKey = 'h';
        } else if (key == 'b') {
            blur5x5(src, dst);
            imwrite(filePrefix + "part5_generated_image.png", dst);
            setWindowTitle("Video", "Blurry Mode by Me");
            lastKey = 'b';
        } else if (key == 'x') {
            dst = Mat::zeros(src.rows, src.cols, CV_16SC3);
            sobelX3x3(src, dst);
            convertScaleAbs(dst, dst);
            imwrite(filePrefix + "part6_generated_imageX.png", dst);
            setWindowTitle("Video", "Sobel-X Filter Mode by Me");
            lastKey = 'x';
        } else if (key == 'y') {
            dst = Mat::zeros(src.rows, src.cols, CV_16SC3);
            sobelY3x3(src, dst);
            convertScaleAbs(dst, dst);
            imwrite(filePrefix + "part6_generated_imageY.png", dst);
            setWindowTitle("Video", "Sobel-Y Filter Mode by Me");
            lastKey = 'y';
        } else if (key == 'm') {
            Mat sobel_filtered_Ximage = Mat::zeros(src.rows, src.cols, CV_16SC3);
            Mat sobel_filtered_Yimage = Mat::zeros(src.rows, src.cols, CV_16SC3);
            
            sobelX3x3(src, sobel_filtered_Ximage);
            sobelY3x3(src, sobel_filtered_Yimage);
            
            magnitude(sobel_filtered_Ximage, sobel_filtered_Yimage, dst);
            imwrite(filePrefix + "part7_generated_image.png", dst);
            setWindowTitle("Video", "Magnitude Filter Mode by Me");
            lastKey = 'm';
        } else if (key == 'i') {
            blurQuantize(src, dst, 15);
            imwrite(filePrefix + "part8_generated_image.png", dst);
            setWindowTitle("Video", "Blur and Quantinized Filter Mode by Me");
            lastKey = 'i';
        } else if (key == 'c') {
            cartoon(src, dst, 15, 15);
            imwrite(filePrefix + "part9_generated_image.png", dst);
            setWindowTitle("Video", "Cartoonized Filter Mode by Me");
            lastKey = 'c';
        } else if(key == 'p') {
            changeBrightness(src, dst);
            imwrite(filePrefix + "part10_generated_image.png", dst);
            setWindowTitle("Video", "Extra brightness Mode by Me");
            lastKey = 'p';
        }  else if(key == 'e') {
            trackBar(src, dst);
            imwrite(filePrefix + "part11_generated_image.png", dst);
            setWindowTitle("Video", "Extension Adding Trackbar to Video");
            lastKey = 'e';
        }
        
        imshow("Video", dst);
    }
    delete capdev;
}
