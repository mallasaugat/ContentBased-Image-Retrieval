/*
   Compute the features of each image and get the 
*/

#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<vector>
#include "features.hpp"
#include<dirent.h>
#include "csv_util.h"


using namespace std;

int main(int argc, char *argv[]){

    char dirname[256];
    char buffer[256];
    char csvfileName[256];
    int featureType;
    fstream csvFile;

    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;

    // Checking for sufficient arguments
    if(argc < 4){
        cout<<"Usage: "<<argv[1]<<"<directory path> <feature type> <csv file>\n";
        exit(-1);
    }

    // Get the directory path
    strcpy(dirname, argv[1]);
    cout<<"Processing directory: "<< dirname<<"\n";

       // Open the directory
    dirp = opendir(dirname);
    if(dirp==NULL){
        cout<<"Cannot open directory: "<< dirname<<"\n";
        exit(-1);
    }

    // Get feature type
    //strcpy(featureType, argv[2]);

    // Get the csv file name
    strcpy(csvfileName, argv[3]);
   
 
    vector<float> featureVector;
    // Loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ){

        // Check if the file is an image
        if( strstr(dp->d_name, ".jpg") || strstr(dp->d_name, ".png") || strstr(dp->d_name, ".ppm") || strstr(dp->d_name, ".tif") ){

            //cout<<"Processing image files: "<<dp->d_name<<"\n";

            // Build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            //cout<<"Full path name: "<<buffer<<"\n";

            cv::Mat hist;

            cv::Mat img = cv::imread(buffer);
            cv::Mat image = img.clone();
            // cv::Mat temp;
            // image.convertTo(temp,  CV_32F, 1.0/255);
            if(image.empty()){
                cout<<"Error: Unable to read image:"<<buffer<<endl;
                continue;
            }

            // Computing the feature for target images
            featureType = atoi(argv[2]);
            switch(featureType){
                // case "Baseline": 
                // case "baseline":
                case 1:
                    featureVector = baseline(image);
                    break;
                case 2:
                    featureVector = singleHM(image);
                    break;
                case 3:
                    featureVector = multiHM(image);
                    break;
                default:
                    cout<<"Invalid feature type"<<endl;
                    break;
            }
            

            // cout<<featureVector;
            // exit(-1);
            
            // Converting Mat to float vec
            // if (!featureVector.isContinuous()) {
            //     featureVector = featureVector.clone(); // Make a continuous copy
            // }
            
            // cv::Mat tmp;
            // featureVector.convertTo(tmp, CV_32F);
            // //cv::Mat reshapedFeatureVector = featureVector.reshape(1,1);
            // vector<float> featureVectorFloat;
            // featureVectorFloat.assign((float*)featureVector.datastart, (float*)featureVector.dataend);

            // Converting Mat to float vec
            // vector<float> featureVectorFloat;
            // for (int i = 0; i < featureVector.rows; ++i) {
            //     for (int j = 0; j < featureVector.cols; ++j) {
            //         featureVectorFloat.push_back(featureVector.at<float>(i, j));
            //     }
            // }

            // for(int i=0;i<featureVectorFloat.size();i++){
            //     cout<<featureVectorFloat[i]<<" ";
            // }
            // exit(-1);
            

            append_image_data_csv(csvfileName, dp->d_name, featureVector, 0);

        }



    }

    cout<<"Terminating\n";

    return 0;
}