#pragma once 
#include "filter.hpp"

void showImage(cv::Mat image, std::string title, int width, int height);

cv::Mat grayscaleConvert(cv::Mat image);

cv::Mat colorConvert(cv::Mat image);

cv::Mat equalize(cv::Mat image);

cv::Mat gaussian(cv::Mat image);

cv::Mat median(cv::Mat image);

cv::Mat adaptThresh(cv::Mat image);

cv::Mat canny(cv::Mat image);

cv::Mat denoise(cv::Mat image);