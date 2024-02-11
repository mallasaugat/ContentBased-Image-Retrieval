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
    featureType = atoi(argv[2]);

    // Get the csv file name
    strcpy(csvfileName, argv[3]);
   

    // Read feature vectors from file
    vector<char *> filenames;
    vector<vector<float>> data;
    char resNet[256];
    strcpy(resNet, "ResNet18_olym.csv");
    if(read_image_data_csv(resNet, filenames, data)!=0){
        cout<<"Error: Unable to read feature vector file"<<endl;
        exit(-1);
    }

    char completeFN[256];
    vector<float> featureVector;
    vector<float> featureVector_2;
    vector<float> featureVector_3;
    vector<float> featureVector_4;
    // Loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ){

        // Check if the file is an image
        if( strstr(dp->d_name, ".jpg") || strstr(dp->d_name, ".png") || strstr(dp->d_name, ".ppm") || strstr(dp->d_name, ".tif") || strstr(dp->d_name, ".jpeg")){

            //cout<<"Processing image files: "<<dp->d_name<<"\n";

            // Build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            //cout<<"Full path name: "<<buffer<<"\n";

            cv::Mat hist;

            cv::Mat img = cv::imread(buffer);
            cv::Mat image = img.clone();
    
            if(image.empty()){
                cout<<"Error: Unable to read image:"<<buffer<<endl;
                continue;
            }

            // Computing the feature for target images
            switch(featureType){
                case 1:
                    featureVector = baseline(image);
                    break;
                case 2:
                    featureVector = singleHM(image);
                    break;
                case 3:
                    featureVector = multiHM(image);
                    break;
                case 4:
                    featureVector = featureHM(image);
                    break;
                case 7:
                    featureVector = computeColorHistogram(image);
                    break;

                case 8:
                    for(size_t i=0; i<filenames.size(); ++i){
                        strcpy(completeFN, "olympus");
                        strcat(completeFN, "/");
                        strcat(completeFN, filenames[i]);

                        if(strlen(filenames[i])==0){
                            cout<<"Error: Unable to read image:"<<completeFN<<endl;
                            continue;
                        }

                        if(strcmp(completeFN,buffer) == 0){
                            featureVector_2 = data[i];
                        }
                    }
                    featureVector = computeColorHistogram(image);
                    featureVector.insert(featureVector.end(), featureVector_2.begin(), featureVector_2.end());
                    break;

                case 9:
                    for(size_t i=0; i<filenames.size(); ++i){
                        strcpy(completeFN, "olympus");
                        strcat(completeFN, "/");
                        strcat(completeFN, filenames[i]);

                        if(strlen(filenames[i])==0){
                            cout<<"Error: Unable to read image:"<<completeFN<<endl;
                            continue;
                        }

                        if(strcmp(completeFN,buffer) == 0){
                            featureVector_2 = data[i];
                        }
                    }

                    featureVector = featureHM(image);
                    featureVector.insert(featureVector.end(), featureVector_2.begin(), featureVector_2.end());
                    break;

                default:
                    cout<<"Invalid feature type"<<endl;
                    break;
            }
            

            // Converting Mat to float vec
            // vector<float> featureVectorFloat;
            // for (int i = 0; i < featureVector.rows; ++i) {
            //     for (int j = 0; j < featureVector.cols; ++j) {
            //         featureVectorFloat.push_back(featureVector.at<float>(i, j));
            //     }
            // }

 
            append_image_data_csv(csvfileName, dp->d_name, featureVector, 0);

        }

    }

    cout<<"Terminating\n";

    return 0;
}