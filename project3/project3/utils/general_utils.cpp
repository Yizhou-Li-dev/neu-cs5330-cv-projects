//
//  general_utils.cpp
//  porject3
//
//  Created by Yizhou Li on 2/28/23.
//

#include "general_utils.hpp"

double StandardDeviation(std::vector<double> samples)
{
     return sqrt(Variance(samples));
}

double Variance(std::vector<double> samples)
{
     int size = samples.size();

     double variance = 0;
     double t = samples[0];
     for (int i = 1; i < size; i++)
     {
          t += samples[i];
          double diff = ((i + 1) * samples[i]) - t;
          variance += (diff * diff) / ((i + 1.0) *i);
     }

     return variance / (size - 1);
}

double scaled_euclidian_distance(std::vector<double> f1, std::vector<double> f2, std::vector<double> stds) {
    double rst = 0;
    double tmp_val = 0;
    for (int i = 0; i < f1.size(); i++) {
        tmp_val = (f1[i] - f2[i]) / stds[i];
        tmp_val *= tmp_val;
        rst += tmp_val;
    }
    return rst;
}

void mark_object(cv::Mat &segmented_img, std::vector<cv::Point> draw_vertices) {
  cv::circle(segmented_img, draw_vertices[0], 2, cv::Scalar(0, 0, 255), 2);
  cv::line(segmented_img, draw_vertices[1], draw_vertices[2], cv::Scalar(0, 255, 0), 2);
  cv::line(segmented_img, draw_vertices[3], draw_vertices[4], cv::Scalar(0, 255, 0), 2);
  cv::line(segmented_img, draw_vertices[4], draw_vertices[6], cv::Scalar(0, 255, 0), 2);
  cv::line(segmented_img, draw_vertices[6], draw_vertices[5], cv::Scalar(0, 255, 0), 2);
  cv::line(segmented_img, draw_vertices[5], draw_vertices[3], cv::Scalar(0, 255, 0), 2);
}

int features(cv::Mat &src, std::vector<double> &feature_vector, std::vector<cv::Point> &draw_vertices) {
  cv::Moments moments = cv::moments(src, true);

  // centroid
  std::pair<double, double> centroid;
  centroid.first = moments.m10 / moments.m00;
  centroid.second = moments.m01 / moments.m00;
  draw_vertices.emplace_back(cv::Point((int)centroid.first, (int)centroid.second));

  // angle
  double angle = 0.5 * std::atan2(2.0 * moments.mu11, moments.mu20 - moments.mu02);

  // coordinate system transformation
  std::vector<std::pair<double, double>> transformed_vertices;
  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.cols; j++) {
      if (src.at<uchar>(i, j) == FRONT_GROUND) {
        // when transforming, the x is j and y is i
        int x = j, y = i;
        transformed_vertices.emplace_back(std::make_pair(
            std::cos(angle) * (x - centroid.first) + std::sin(angle) * (y - centroid.second),
            std::cos(angle) * (y - centroid.second) - std::sin(angle) * (x - centroid.first)
        ));
      }
    }
  }

  // get bounding boxes axis
  double quad_axis[4] = {0., 0., 0., 0.};
  for (std::pair<double, double> p: transformed_vertices) {
    // leftest x-axis
    quad_axis[0] = std::min(p.first, quad_axis[0]);
    // rightest x-axis
    quad_axis[1] = std::max(p.first, quad_axis[1]);
    // top y-axis
    quad_axis[2] = std::max(p.second, quad_axis[2]);
    // bottom y-axis
    quad_axis[3] = std::min(p.second, quad_axis[3]);
  }

  // get the width and height feature
  double height = quad_axis[2] - quad_axis[3];
  double width = quad_axis[1] - quad_axis[0];
  feature_vector.emplace_back(height / width);


  // get filled ratio feature
  feature_vector.emplace_back(moments.m00 / (height * width));

  // get second moment about the central axis feature
  double beta = angle + CV_PI / 2.;
  double summation = 0.;
  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.cols; j++) {
      if (src.at<uchar>(i, j) == FRONT_GROUND) {
        int x = j, y = i;
        double temp = (y - centroid.second) * std::cos(beta) + (x - centroid.first) * std::sin(beta);
        summation += temp * temp;
      }
    }
  }
  double mu_22_angle = summation / moments.m00;
  feature_vector.emplace_back(mu_22_angle);

  double hu[7];
  HuMoments(moments, hu);
  feature_vector.emplace_back(hu[0]);
  feature_vector.emplace_back(hu[1]);
  feature_vector.emplace_back(hu[2]);

  // find the coordinates in the original coordinate system, then draw the bounding box
  std::vector<std::pair<double, double>> temp_vertices;
  // central axis vertices
  temp_vertices.emplace_back(std::make_pair(quad_axis[0], 0));
  temp_vertices.emplace_back(std::make_pair(quad_axis[1], 0));
  // top left vertices
  temp_vertices.emplace_back(std::make_pair(quad_axis[0], quad_axis[2]));
  // top right vertices
  temp_vertices.emplace_back(std::make_pair(quad_axis[1], quad_axis[2]));
  // bottom left vertices
  temp_vertices.emplace_back(std::make_pair(quad_axis[0], quad_axis[3]));
  // bottom right vertices
  temp_vertices.emplace_back(std::make_pair(quad_axis[1], quad_axis[3]));

  for (std::pair<int, int> p: temp_vertices) {
    draw_vertices.emplace_back(cv::Point(
        (int)(std::cos(angle) * p.first - std::sin(angle) * p.second + centroid.first),
        (int)(std::sin(angle) * p.first + std::cos(angle) * p.second + centroid.second)
    ));
  }
  return 0;
}
