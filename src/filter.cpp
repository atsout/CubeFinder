#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/photo/cuda.hpp>

#include "filter.hpp"
#include "parse.hpp"

#include <iostream>

// show image in window
void showImage(cv::Mat image, std::string title, int width, int height){
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, width, height);
    cv::imshow(title, image);
}

// convert to grayscale
cv::Mat grayscaleConvert(cv::Mat image){
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    return image;
}

cv::Mat colorConvert(cv::Mat image){
    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    return image;
}

// histogram equalization
cv::Mat equalize(cv::Mat image){
    cv::Mat hist;
    cv::equalizeHist(image, hist);
    return hist;
}

// gaussian blur
cv::Mat gaussian(cv::Mat image){
    char text[4];
    readSetting(text, 6);
    int v = settingToDouble(text);
    
    cv::Mat gaus;
    GaussianBlur(image, gaus, cv::Size(v, v), 0);
    return gaus;
}

// median blur
cv::Mat median(cv::Mat image){
    char text[4];
    readSetting(text, 7);
    int v = settingToDouble(text);

    cv::Mat median;
    cv::medianBlur(image, median, v);
    return median;
}

// adaptive threshold
cv::Mat adaptThresh(cv::Mat image){
    char text[4];
    readSetting(text, 9);
    int v = settingToDouble(text);

    char text2[4];
    readSetting(text2, 10);
    int V = settingToDouble(text2);

    cv::Mat adaptThresh;
    cv::adaptiveThreshold(image, adaptThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, v, V);
    return adaptThresh;
}

// canny edge detection
cv::Mat canny(cv::Mat image){
    char text[4];
    readSetting(text, 11);
    int v = settingToDouble(text);

    char text2[4];
    readSetting(text2, 12);
    int V = settingToDouble(text2);

    char text3[4];
    readSetting(text3, 13);
    int u = settingToDouble(text3);

    cv::Mat canny;
    cv::Canny(image, canny, v, V, u);
    return canny;
}

cv::Mat denoise(cv::Mat image){
    char text[4];
    readSetting(text, 8);
    int v = settingToDouble(text);

    cv::Mat denoise;
    cv::fastNlMeansDenoising(image, denoise, v, 7, 21);
    return denoise;
}


