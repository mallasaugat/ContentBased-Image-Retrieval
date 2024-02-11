/*
    Code to identify files in a directory
*/

#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<vector>
#include "features.hpp"
#include<dirent.h>
#include "csv_util.h"
#include<cstring>

/*
    Given a directory on the command line, scans through the directory for image files.
    Prints out the full path name for each file. This can be used as an argument to
    fopen or to cv::imread.
*/

using namespace std;

int main(int argc, char *argv[]){

    char targetImage[256];
    char featureVectorFile[256];
    int featureType;
    int outputImages;
    //auto a=outputImages;
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;

    // Checking for sufficient arguments
    if(argc < 5){
        cout << "Usage: " << argv[0] << " <target_image> <feature_type> <feature_vector_file> <output_images>" << endl;
        exit(-1);
    }

    // Get the target image
    strcpy(targetImage, argv[1]);

    // Get the feature type
    featureType = atoi(argv[2]);

    strcpy(featureVectorFile, argv[3]);

    // Get Number of output images
    outputImages = atoi(argv[4]);


    
    // Below is done so that one variable can be used to call different functions based on user input
    // Declaring a type alias for the function pointer type
    using DistanceFunction = float (*)(const vector<float>&, const vector<float>&);
    // Declaring a variable of this type.
    DistanceFunction distanceM;
    

    // Read feature vectors from file
    vector<char *> filenames;
    vector<vector<float>> data;
    if(read_image_data_csv(featureVectorFile, filenames, data)!=0){
        cout<<"Error: Unable to read feature vector file"<<endl;
        exit(-1);
    }

    
    // Reading the target image
    cv::Mat tImage = cv::imread(targetImage);
    if(tImage.empty()){
            cout<<"Error: Unable to read image:"<<targetImage<<endl;
    }


    // Computing the features for the target image
    char completeFN[256];
    vector<float> targetFeatures;
    vector<float> targetFeatures_2;
    switch(featureType){
        case 1:
            targetFeatures = baseline(tImage);
            distanceM = &SSD;
            break;
        case 2:
            targetFeatures = singleHM(tImage);
            distanceM = &histIntersection;
            break;
        case 3:
            targetFeatures = multiHM(tImage);
            distanceM = &histIntersection;
            break;
        case 4:
            targetFeatures = featureHM(tImage);
            distanceM = &histIntersection;
            break;
        case 5:
            for(size_t i=0; i<filenames.size(); ++i){
                // Memory address
                // if(filenames[i]==targetImage)
                strcpy(completeFN, "olympus");
                strcat(completeFN, "/");
                strcat(completeFN, filenames[i]);

                if(strlen(filenames[i])==0){
                    cout<<"Error: Unable to read image:"<<completeFN<<endl;
                    continue;
                }

                if(strcmp(completeFN,targetImage) == 0){
                    targetFeatures = data[i];
                }
            }
            
            distanceM = &cosineDistance;
            
            break;

        case 7:

            targetFeatures = computeColorHistogram(tImage);

            distanceM = &histIntersection;
            
            break;

        case 8:
            for(size_t i=0; i<filenames.size(); ++i){
                // Memory address
                // if(filenames[i]==targetImage)
                strcpy(completeFN, "olympus");
                strcat(completeFN, "/");
                strcat(completeFN, filenames[i]);

                if(strlen(filenames[i])==0){
                    cout<<"Error: Unable to read image:"<<completeFN<<endl;
                    continue;
                }

                if(strcmp(completeFN,targetImage) == 0){
                    targetFeatures = data[i];
                }
            }
            
            distanceM = &histIntersection;
            
            break;

        

        case 9:    
            for(size_t i=0; i<filenames.size(); ++i){
                // Memory address
                // if(filenames[i]==targetImage)
                strcpy(completeFN, "olympus");
                strcat(completeFN, "/");
                strcat(completeFN, filenames[i]);

                if(strlen(filenames[i])==0){
                    cout<<"Error: Unable to read image:"<<completeFN<<endl;
                    continue;
                }

                if(strcmp(completeFN,targetImage) == 0){
                    targetFeatures = data[i];
                }
            }
            
            distanceM = &SSD;
            
            break;

        
    
        default:
            cout<<"Invalid feature type"<<endl;
            exit(-1);
    }
    

    // Calculating distance and sort
    vector<pair<string, float>> distances;
    for(size_t i=0; i<filenames.size(); ++i){
        //cout<<targetFeatures;
        float distance = distanceM(targetFeatures, data[i]);
        distances.emplace_back(filenames[i], distance);
    }

    // Sort using in-build vector sort function, sorts distances in range, lambda function is used as comparision function 
    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });


    // Display Top N Images
    char buffer[256];
    cv::Mat src;
    
    cout<<"Top "<<outputImages<<" matches: "<<endl;
    for(int i=0;i<outputImages && i<distances.size(); i++){
        cout<<distances[i].first<<" (Distance: "<< distances[i].second << ")"<<endl;

        // strcpy(buffer, "olympus");
        strcpy(buffer, "olympus");
        strcat(buffer, "/");
        const char *str = distances[i].first.c_str();
        strcat(buffer, str);
        

        src = cv::imread(buffer);
        cv::imshow(buffer,src);
        cv::waitKey(0);
        cv::destroyWindow(buffer);
    }

    return 0;
}