//
//  imgDisplay.cpp
//  project1
//
//  Created by Yizhou Li on 12/25/22.
//

#include "imgDisplay.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

void startImg() {
    string imagePath = "/Users/zhou/Library/Mobile Documents/com~apple~CloudDocs/Yizhou/学校/研究生/NU课程/CS5330 CV/project1/project1/resources/3ED9589F-629E-4319-833C-D8C968CD1EA0_1_105_c.jpeg";
    Mat img = imread(imagePath);

    if (img.empty()) {
        cout << "Could not find " << imagePath << endl;
        return;
    }

    imshow("The Display Window", img);

    for (;;) {
        int k = waitKey(0);
        if (k == 's') {
            imwrite("/Users/zhou/Library/Mobile Documents/com~apple~CloudDocs/Yizhou/学校/研究生/NU课程/CS5330 CV/project1/project1/resources/part1_generated_image.png", img);
        } else if (k == 'q') {
            break;
        }
    }
}
