/*
   Compute the features of each image and get the 
*/

#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include "features.hpp"
#include<dirent.h>


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
    if(argc < 3){
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
    csvFile.open(csvfileName, fstream::in | fstream::out | fstream::app);
    // If file exists append if not then append to it
    if(!csvFile){
        cout<<"Cannot open file, file doesnot exits, creating new file";

        csvFile.open(csvfileName, fstream::in | fstream::out | fstream::trunc);
        csvFile.close();
    }
    else{ //user existing file
        cout<<"Success"<<csvfileName<<"found \n";
        cout<<"Writing on existing file"<<"\n";

        csvFile.close();
        cout<<"\n";
    }
 

    // Loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ){

        // Check if the file is an image
        if( strstr(dp->d_name, ".jpg") || strstr(dp->d_name, ".png") || strstr(dp->d_name, ".ppm") || strstr(dp->d_name, ".tif") ){

            cout<<"Processing image files: "<<dp->d_name<<"\n";

            // Build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            // cout<<"Full path name: "<<buffer<<"\n";

            cv::Mat image = cv::imread(buffer);
            if(image.empty()){
                cout<<"Error: Unable to read image:"<<buffer<<endl;
                continue;
            }


            // Computing the feature for target images
            switch(featureType){
                // case "Baseline": 
                // case "baseline":
                case 1:
                    cv::Mat featureVector;
                    featureVector = baseline(image);
                    break;
                // default:
                //     break;
            }

        }

    }

    cout<<"Terminating\n";

    return 0;
}