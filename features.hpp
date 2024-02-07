#ifndef KMEANS_H
#define KMEANS_H

#define SSD(a,b) ( ((int)a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]) )

cv::Mat baseline(cv::Mat image);

#endif