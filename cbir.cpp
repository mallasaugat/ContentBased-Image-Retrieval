/*
    Code to identify files in a directory
*/

#include<iostream>
#include<opencv2/opencv.hpp>
#include<dirent.h>
#include "csv_util.h"

/*
    Given a directory on the command line, scans through the directory for image files.
    Prints out the full path name for each file. This can be used as an argument to
    fopen or to cv::imread.
*/

using namespace std;

int main(int argc, char *argv[]){

    char dirname[256];
    char buffer[256];
    char targetImage[256];
    char featureType[256];
    int outputImages; 
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;

    // Checking for sufficient arguments
    if(argc < 3){
        cout<<"Usage: <target image>:, "<<argv[1]<<" number of output images:"<<argv[3];
        exit(-1);
    }

    // Get the target image
    strcpy(targetImage, argv[1]);

    // Get Number of output images
    outputImages = atoi(argv[2]);


    


    return 0;
}