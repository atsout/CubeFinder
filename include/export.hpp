#pragma once
#include "export.hpp"

double distance(cv::Point2f a, cv::Point2f b);

class SquareData;

std::vector<SquareData> getFinalOutput(std::vector<std::vector<cv::Point2f>> pointsVector);

int exportToXlsx(std::vector<std::vector<cv::Point2f>> pointsVector);
