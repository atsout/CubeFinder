#pragma once
#include "contour.hpp"

std::vector<std::vector<cv::Point2f> > identifyRects(cv::Mat image);

cv::Mat drawRects(cv::Mat image, std::vector<std::vector<cv::Point2f> > cubes_rect_points);

std::vector<std::vector<cv::Point2f>> searchRects(std::vector<std::vector<cv::Point2f>> cubes_rect_points, cv::Rect2d ROI);