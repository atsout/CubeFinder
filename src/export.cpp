#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include "xlsxwriter.h"
#include "parse.hpp"
#include "convert.hpp"

#include <iostream>

// euclidean distance
double distance(cv::Point2f a, cv::Point2f b){
    cv::Point2f diff = a - b;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

// class containing data for output for each identified square
class SquareData{
    public:
        double longSide, shortSide, aspectRatio, area;
        SquareData(std::vector<cv::Point2f>, float conversionValue);
};

// constructor for getting output data from square point data
SquareData::SquareData(std::vector<cv::Point2f> square, float conversionValue){
    // get longSide and shortSide
    double sideOne = distance(square[0], square[1]);
    double sideTwo = distance(square[1], square[2]);
    if(sideOne > sideTwo || sideOne == sideTwo){
        longSide = sideOne / conversionValue;
        shortSide = sideTwo / conversionValue;
    } else if(sideTwo > sideOne){
        longSide = sideTwo / conversionValue;
        shortSide = sideOne / conversionValue;
    }
    // calculate aspectRatio and area
    aspectRatio = longSide / shortSide;
    area = longSide * shortSide;
}

// final output vector of squares
std::vector<SquareData> getFinalOutput(std::vector<std::vector<cv::Point2f>> pointsVector){
    
    std::vector<SquareData> output;

    char magX[10];
    readSetting(magX, 33);
    std::string strMagX(magX);
    float conversionValue = getConvertData(strMagX);

    for(size_t i = 0; i < pointsVector.size(); i++){
        SquareData temp (pointsVector[i], conversionValue);
        output.push_back(temp);
    }

    return output;

}

// create xlsx workbook, worksheet, create table labels, then put data for each square into the table
int exportToXlsx(std::vector<std::vector<cv::Point2f>> pointsVector){

    std::vector<SquareData> output = getFinalOutput(pointsVector);

    lxw_workbook* workbook = workbook_new("rename me after writing.xlsx");
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format* format = workbook_add_format(workbook);
    format_set_bold(format);

    // table labels
    worksheet_write_string(worksheet, 0, 0, "Square #", format);
    worksheet_write_string(worksheet, 0, 1, "Long side (nm)", format);
    worksheet_write_string(worksheet, 0, 2, "Short side (nm)", format);
    worksheet_write_string(worksheet, 0, 3, "Aspect ratio (long side:short side)", format);
    worksheet_write_string(worksheet, 0, 4, "Area (nm^2)", format);
    worksheet_write_string(worksheet, 0, 6, "Average long side (nm)", format);
    worksheet_write_string(worksheet, 1, 6, "Average short side (nm)", format);
    worksheet_write_string(worksheet, 2, 6, "Average aspect ratio", format);
    worksheet_write_string(worksheet, 3, 6, "Average area (nm^2)", format);
    worksheet_write_string(worksheet, 5, 6, "Standard Dev long side (nm)", format);
    worksheet_write_string(worksheet, 6, 6, "Standard Dev short side (nm)", format);
    worksheet_write_string(worksheet, 7, 6, "Standard Dev aspect ratio (nm)", format);
    worksheet_write_string(worksheet, 8, 6, "Standard Dev area (nm^2)", format);
    
    size_t i;

    // input data into table for each square
    for(i = 0; i < output.size(); i++){
        worksheet_write_number(worksheet, i + 1, 0, i + 1, NULL);
        worksheet_write_number(worksheet, i + 1, 1, output[i].longSide, NULL);
        worksheet_write_number(worksheet, i + 1, 2, output[i].shortSide, NULL);
        worksheet_write_number(worksheet, i + 1, 3, output[i].aspectRatio, NULL);
        worksheet_write_number(worksheet, i + 1, 4, output[i].area, NULL);
    }

    // write averages
    std::string avg1("=AVERAGE(B2:B" + std::to_string(i) + ")");
    std::string avg2("=AVERAGE(C2:C" + std::to_string(i) + ")");
    std::string avg3("=AVERAGE(D2:D" + std::to_string(i) + ")");
    std::string avg4("=AVERAGE(E2:E" + std::to_string(i) + ")");
    const char* avg1c = avg1.c_str();
    const char* avg2c = avg2.c_str();
    const char* avg3c = avg3.c_str();
    const char* avg4c = avg4.c_str();

    // write std dev
    std::string std1("=STDEV(B2:B" + std::to_string(i) + ")");
    std::string std2("=STDEV(C2:C" + std::to_string(i) + ")");
    std::string std3("=STDEV(D2:D" + std::to_string(i) + ")");
    std::string std4("=STDEV(E2:E" + std::to_string(i) + ")");
    const char* std1c = std1.c_str();
    const char* std2c = std2.c_str();
    const char* std3c = std3.c_str();
    const char* std4c = std4.c_str();

    worksheet_write_formula(worksheet, 0, 7, avg1c, NULL);
    worksheet_write_formula(worksheet, 1, 7, avg2c, NULL);
    worksheet_write_formula(worksheet, 2, 7, avg3c, NULL);
    worksheet_write_formula(worksheet, 3, 7, avg4c, NULL);

    worksheet_write_formula(worksheet, 5, 7, std1c, NULL);
    worksheet_write_formula(worksheet, 6, 7, std2c, NULL);
    worksheet_write_formula(worksheet, 7, 7, std3c, NULL);
    worksheet_write_formula(worksheet, 8, 7, std4c, NULL);

    // make columns larger
    worksheet_set_column(worksheet, 0, 7, 20, NULL);

    // close workbook
    workbook_close(workbook);
    return 0;
}
