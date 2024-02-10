/*
    Different features
*/

#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<algorithm>

using namespace std;


// Function to compute sum of squared differences between two feature vectors
float SSD(const vector<float>& feature1, const vector<float>& feature2) {
    float ssd = 0;
    for (size_t i = 0; i < feature1.size(); ++i) {
        ssd += pow(feature1[i] - feature2[i], 2);
    }
    return ssd;
}


float histIntersection(const vector<float>& feature1, const vector<float>& feature2){

    float histDist=0;

    for(size_t i=0;i<feature1.size();++i){
        histDist += std::min(feature1[i], feature2[i]);
    }

    return 1-histDist;
}

//7x7 middle of the image as feature vector
vector<float> baseline(cv::Mat &image){

    int x = (image.cols -3) / 2;
    int y = (image.rows -3) / 2;

    cv::Rect roi(x, y, 7,7);
    cv::Mat featureRegion = image(roi);

    vector<float> featureVector;
    for (int y = 0; y < featureRegion.rows; ++y) {
        for (int x = 0; x < featureRegion.cols; ++x) {
            featureVector.push_back(featureRegion.at<uchar>(y, x));
        }
    }

    return featureVector;

}

cv::Mat computeHistogram(cv::Mat src){

    cv::Mat hist;

    const int histsize = 16;

    // Building the histogram 
    hist = cv::Mat::zeros(cv::Size(histsize, histsize), CV_32FC1);


    for(int i=0;i<src.rows;i++){
        cv::Vec3b *ptr = src.ptr<cv::Vec3b>(i); // Pointer to row i

        for(int j=0;j<src.cols;j++){

            float B = ptr[j][0];
            float G = ptr[j][1];
            float R = ptr[j][2];

            float divisor = R+G+B;

            divisor = divisor > 0.0 ? divisor : 1.0; // Check for all zeros

            float r = R / divisor;
            float g = G / divisor;

            // Compute indexes r,g are in [0,1]
            int rindex = (int)( r * (histsize -1) + 0.5);
            int gindex = (int)( g* (histsize -1) + 0.5);

            // Increment the histogram
            hist.at<float>(rindex*histsize+gindex)++;
        }
    }

    // Normalize the histogram by the number of pixels
    // Divides all the elements of cv::Mat by scalar
    // Whole histogram is in the range [0,1]
    hist /= (src.rows * src.cols);
    

    return hist;

}


vector<float> singleHM(cv::Mat &src){
    

    cv::Mat hist;

    hist  = computeHistogram(src);

    vector<float> featureVector;
    for (int y = 0; y < hist.rows; ++y) {
        for (int x = 0; x < hist.cols; ++x) {
            featureVector.push_back(hist.at<float>(y, x));
        }
    }


    return featureVector;
}


vector<float> multiHM(cv::Mat &src){


    cv::Mat hist1, hist2;

    // First histogram
    // Whole image
    hist1 = computeHistogram(src);

    // Second histogram
    // Center 7x7 
    int x = (src.cols -7) / 2;
    int y = (src.rows -7) / 2;

    cv::Rect roi(x, y, 7,7);
    cv::Mat centerRegion = src(roi);
    hist2 = computeHistogram(centerRegion);

    vector<float> featureVector;
    for (int y = 0; y < hist1.rows; ++y) {
        for (int x = 0; x < hist1.cols; ++x) {
            featureVector.push_back(hist1.at<float>(y, x));
        }
    }
    for (int y = 0; y < hist2.rows; ++y) {
        for (int x = 0; x < hist2.cols; ++x) {
            featureVector.push_back(hist2.at<float>(y, x));
        }
    }


    return featureVector;

}