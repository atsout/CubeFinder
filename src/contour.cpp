#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include "filter.hpp"
#include "contour.hpp"
#include "parse.hpp"

#include <iostream>



std::vector<std::vector<cv::Point2f>> identifyRects(cv::Mat image){
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(image, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    char user1[4];
    readSetting(user1, 16);
    int contour_point_size_min = settingToDouble(user1);

    char user2[4];
    readSetting(user2, 17);
    int contour_area_min = settingToDouble(user2);

    char user6[4];
    readSetting(user6, 35);
    int contour_point_size_max = settingToDouble(user6);

    char user7[4];
    readSetting(user7, 36);
    int contour_area_max = settingToDouble(user7);

    // filter out based on min number of points per contour and min contour area
    std::vector<std::vector<cv::Point>> cubes;
    for(size_t i = 0; i < contours.size(); i++){
        if(contours[i].size() < contour_point_size_max && contours[i].size() > contour_point_size_min && contourArea(contours[i]) < contour_area_max && contourArea(contours[i]) > contour_area_min){
            cubes.push_back(contours[i]);
        }
    }

    // draw filtered contour lines
    cv::Mat contour_drawing = cv::Mat::zeros(image.size(), CV_8UC3);
    for(size_t i = 0; i < cubes.size(); i++){
        cv::Scalar color = cv::Scalar(255, 0, 0);
        cv::drawContours(contour_drawing, cubes, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
    }

    // find user input settings
    char user3[4];
    readSetting(user3, 20);
    double max_aspect_ratio = settingToDouble(user3);

    char user4[4];
    readSetting(user4, 21);
    int max_area = settingToDouble(user4);

    char user5[4];
    readSetting(user5, 22);
    int min_area = settingToDouble(user5);

    // create rotatedrects around each contour, get 4 points of each rect, then copy 
    // points into nested point vector for performing math on and drawing lines with
    std::vector<std::vector<cv::Point2f>> cubes_rect_points;
    for(size_t i = 0; i < cubes.size(); i++){
       
        // gets 4 points
        cv::RotatedRect rect_temp; 
        rect_temp = cv::minAreaRect(cubes[i]);
        cv::Point2f vtx[4];
        rect_temp.points(vtx);

        // removal conditions for rect aspect ratio and area
        if(rect_temp.size.width*max_aspect_ratio > rect_temp.size.height && rect_temp.size.height*max_aspect_ratio > rect_temp.size.width){
            if(rect_temp.size.width*rect_temp.size.height > min_area && rect_temp.size.width*rect_temp.size.height < max_area){
                
                // copy points into cubes_rect_points 
                std::vector<cv::Point2f> rect_vec_temp;
                for(int i2 = 0; i2 < 4; i2++){
                    rect_vec_temp.push_back(vtx[i2]);
                }
                cubes_rect_points.push_back(rect_vec_temp);
                
            }
        }
    }

    return cubes_rect_points;
    
}

cv::Mat drawRects(cv::Mat image, std::vector<std::vector<cv::Point2f>> cubes_rect_points){
    
    // draw the rectangles by connecting each point with a line
    cv::Mat rect_drawing = cv::Mat::zeros(image.size(), CV_8UC3);
    cv::Scalar color = cv::Scalar(0, 0, 255);
    for(size_t i = 0; i < cubes_rect_points.size(); i++){ 
        cv::line(rect_drawing, cubes_rect_points[i][0], cubes_rect_points[i][1], color, 4);
        rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][0])[0] = 0;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][0])[1] = 255;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][0])[2] = 255;

        cv::line(rect_drawing, cubes_rect_points[i][1], cubes_rect_points[i][2], color, 4);
        rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][1])[0] = 0;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][1])[1] = 255;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][1])[2] = 255;

        cv::line(rect_drawing, cubes_rect_points[i][2], cubes_rect_points[i][3], color, 4);
        rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][2])[0] = 0;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][2])[1] = 255;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][2])[2] = 255;

        cv::line(rect_drawing, cubes_rect_points[i][3], cubes_rect_points[i][0], color, 4);
        rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][3])[0] = 0;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][3])[1] = 255;  
		rect_drawing.at<cv::Vec3b>(cubes_rect_points[i][3])[2] = 255;

    }

    return rect_drawing;
    
}

// searches a cv::Rect2d for a point in std::vector<std::vector<cv::Point2f>> and erases it from the vector
std::vector<std::vector<cv::Point2f>> searchRects(std::vector<std::vector<cv::Point2f>> cubes_rect_points, cv::Rect2d ROI){

    for(int i = 0; i < cubes_rect_points.size(); i++){
        for(int i2 = 0; i2 < cubes_rect_points[i].size(); i2++){
            if(cubes_rect_points[i][i2].inside(ROI) == 1){

                cubes_rect_points.erase(cubes_rect_points.begin() + i);
                break;
            }
            
        }
    }
        
    return cubes_rect_points;

}