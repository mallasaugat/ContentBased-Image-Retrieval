/*
    Different features
*/

#include<iostream>
#include "opencv2/opencv.hpp"

//7x7 center square feature vector
cv::Mat baseline(cv::Mat image){

    cv::Rect roi(image.cols/4, image.rows/4, 7,7);
    cv::Mat featureVector = image(roi);

    return featureVector;

}