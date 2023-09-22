//
//  obj_rec_methods.hpp
//  porject3
//
//  Created by Yizhou Li on 2/14/23.
//

#ifndef obj_rec_methods_hpp
#define obj_rec_methods_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int threshold_image(Mat &src, Mat &dst);
int custom_morphological_filtering(Mat &src, Mat &dst);
int connected_components_analysis(Mat &dst, Mat &stats, int n);
int get_features_from_a_region(Mat &regionMap, int regionId, int label_size, Mat &stats, vector<double> &features);

#endif /* obj_rec_methods_hpp */
