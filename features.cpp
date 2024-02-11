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

float cosineDistance(const vector<float>& feature1, const vector<float>& feature2){

    float cDist = 0;
    float dotProduct = 0;
    float norm1 = 0;
    float norm2 = 0;


    // Accumulate their products
    // for(size_t i=0;i<feature1.size();++i){

    //     cDist += (feature1[i]/f1_size) * (feature2[i]/f2_size);

    // }

    // Dot Product
    for(size_t i=0;i<feature1.size();++i){

        dotProduct += feature1[i] * feature2[i];
        norm1 += feature1[i] * feature1[i];
        norm2 += feature2[i] * feature2[i];

    }

    // Cosine distance
    cDist = dotProduct / (sqrt(norm1) * sqrt(norm2));

    
    return 1 - cDist;
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


// Function that applied sobelX filter
int sobelX3x3(cv::Mat &src, cv::Mat &dst){
  
  //src.copyTo(dst);
  
  dst.create(src.size(), CV_16SC3);

  for(int i=1;i<src.rows-1;i++){
    for(int j=1;j<src.cols-1;j++){
      for(int k=0;k<src.channels();k++){ 

        int sum = (-1)*src.at<cv::Vec3b>(i-1, j-1)[k] + \
          0*src.at<cv::Vec3b>(i-1, j)[k] \
          + src.at<cv::Vec3b>(i-1,j+1)[k] + (-2)*src.at<cv::Vec3b>(i, j-1)[k] + \
          0*src.at<cv::Vec3b>(i,j)[k] + 2*src.at<cv::Vec3b>(i, j+1)[k] + \
          (-1)*src.at<cv::Vec3b>(i+1, j-1)[k] + (0)* src.at<cv::Vec3b>(i+1, j)[k] + \
          src.at<cv::Vec3b>(i+1, j+1)[k];
        
        //sum /= 16;
        
        dst.at<cv::Vec3s>(i,j)[k] = sum;
        dst.convertTo(dst, CV_16SC3);

      }
    }
  }
   

  return 0;
}

// Function that applied sobelY filter 
int sobelY3x3(cv::Mat &src, cv::Mat &dst){
  
  //src.copyTo(dst);
  
  dst.create(src.size(), CV_16SC3);


  for (int i = 1; i < src.rows - 1; i++) {
    for (int j = 1; j < src.cols - 1; j++) {
      for (int k = 0; k < src.channels(); k++) {

        int sum = (-1) * src.at<cv::Vec3b>(i - 1, j - 1)[k] + \
                  (-2) * src.at<cv::Vec3b>(i - 1, j)[k] + \
                  (-1) * src.at<cv::Vec3b>(i - 1, j + 1)[k] + (0) * src.at<cv::Vec3b>(i, j - 1)[k] + \
                  0 * src.at<cv::Vec3b>(i, j)[k] + 0 * src.at<cv::Vec3b>(i, j + 1)[k] + \
                  (1) * src.at<cv::Vec3b>(i + 1, j - 1)[k] + (2) * src.at<cv::Vec3b>(i + 1, j)[k] + \
                  src.at<cv::Vec3b>(i + 1, j + 1)[k];
        
        //sum /= 16;
        
        dst.at<cv::Vec3s>(i, j)[k] = sum;
        dst.convertTo(dst,CV_16SC3);

      }
    }
  }

  return 0;
}


// Function that generates a gradient magnitude image from sobel X and Y filters
int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst){
  
  // Ensure sx, sy are of type CV_32SC3
  sx.convertTo(sx, CV_16SC3);
  sy.convertTo(sy, CV_16SC3);

  // Default CV_8UC3
  dst.create(sx.size(), CV_8UC3);
  
  for(int i=0;i<sx.rows;i++){
    for(int j=0;j<sx.cols;j++){
      for(int k=0;k<sx.channels();k++){
      
        float I = sqrt(static_cast<float>(sx.at<cv::Vec3s>(i,j)[k] * sx.at<cv::Vec3s>(i,j)[k] + \
                                          sy.at<cv::Vec3s>(i,j)[k] * sy.at<cv::Vec3s>(i,j)[k]));

        // To ensure magnitude is within th uchar range [0,255]
        I = std::min(255.0f, std::max(0.0f, I));

        dst.at<cv::Vec3b>(i,j)[k] = static_cast<uchar>(I);

      }
    }
  }

  return 0; 

}


vector<float> featureHM(cv::Mat &src){


    cv::Mat cHist, tHist;

    // Color histogram of Whole image
    cHist = computeHistogram(src);

    // Texture histogram of whole image
    cv::Mat sobelx;
    cv::Mat sobely;
    cv::Mat dst;
    sobelX3x3(src, sobelx);
    sobelY3x3(src, sobely);
    magnitude(sobelx, sobely, dst);
    tHist = computeHistogram(dst);

    

    vector<float> featureVector;
    for (int y = 0; y < cHist.rows; ++y) {
        for (int x = 0; x < cHist.cols; ++x) {
            featureVector.push_back(cHist.at<float>(y, x));
        }
    }
    for (int y = 0; y < tHist.rows; ++y) {
        for (int x = 0; x < tHist.cols; ++x) {
            featureVector.push_back(tHist.at<float>(y, x));
        }
    }


    return featureVector;

}

// Color histogram features
vector<float> computeColorHistogram(cv::Mat& image) {
    // Initialize histogram bins
    const int histSize = 256;
    const int histDim = 3;
    float range[] = {0, 256};
    const float* histRange = {range};

    // Split the image into channels
    std::vector<cv::Mat> bgr_planes;
    cv::split(image, bgr_planes);

    // Initialize histogram features vector
    std::vector<float> hist_features(histSize * histDim, 0.0f);

    // Compute histogram for each channel
    for (int ch = 0; ch < histDim; ++ch) {
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                int bin = static_cast<int>(bgr_planes[ch].at<uchar>(i, j));
                hist_features[ch * histSize + bin]++;
            }
        }
    }

    // Normalize histogram
    int totalPixels = image.rows * image.cols;
    for (int i = 0; i < histDim * histSize; ++i) {
        hist_features[i] /= totalPixels;
    }

    return hist_features;
}