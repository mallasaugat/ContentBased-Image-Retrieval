#ifndef FEATURES_H
#define FEATURES_H

// #define SSD(a,b) ( ((int)a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]) )
float SSD(const std::vector<float>& feature1, const std::vector<float>& feature2);

float histIntersection(const std::vector<float>& feature1, const std::vector<float>& feature2);

std::vector<float> baseline(cv::Mat &image);

std::vector<float> singleHM(cv::Mat &src);

std::vector<float> multiHM(cv::Mat &src);

#endif