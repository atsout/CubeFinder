#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include "parse.hpp"
#include "filter.hpp"
#include "main.hpp"
#include "contour.hpp"
#include "export.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]){

    if(argc == 1){ 
        std::cout << "No request image path as been entered." << '\n';
        exit(0);
    }

    std::string str(argv[1]);
    std::cout << "Requested image: " << str << '\n';

    std::cout << "Reading settings..." << '\n';
    
    char show_all_steps[2];
    readSetting(show_all_steps, 25);
    
    char export_to_xlsx[2];
    readSetting(export_to_xlsx, 26);

    char save_image[2];
    readSetting(save_image, 27);

    char run_gaussian[2];
    readSetting(run_gaussian, 28);

    char run_median[2];
    readSetting(run_median, 29);

    char run_denoise[2];
    readSetting(run_denoise, 30);

    // read requested image
    std::string image_path = cv::samples::findFile(str);
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
    
    if(img.empty()){
        std::cout << "Could not read the image. Did you enter the image path correctly?" << image_path << std::endl;
        return 1;
    }

    // clone image (to be processed) so original can be shown later
    cv::Mat cubes = img.clone();

    // convert image to grayscale
    cubes = grayscaleConvert(cubes);

    std::cout << "Denoising..." << '\n';

    // run filters on image if program option = 'y'
    // show each image instance if show_all_steps = 'y'
    if(run_denoise[0] == 'y'){
        cubes = denoise(cubes);
        if(show_all_steps[0] == 'y'){
            showImage(cubes, "denoise", 1000, 1000);
        }
    }

    std::cout << "Blurring..." << '\n';
    
    if(run_gaussian[0] == 'y'){
        cubes = gaussian(cubes);
        if(show_all_steps[0] == 'y'){
            showImage(cubes, "gaussian", 1000, 1000);
        }
    }

    if(run_median[0] == 'y'){
        cubes = median(cubes);
        if(show_all_steps[0] == 'y'){
            showImage(cubes, "median", 1000, 1000);
        }
    }

    std::cout << "Thresholding..." << '\n';
    
    // threshold the image
    cubes = adaptThresh(cubes);
    if(show_all_steps[0] == 'y'){
        showImage(cubes, "adaptThresh", 1000, 1000);
    }
    
    std::cout << "Searching for cubes..." << '\n';

    // find rectangles in the image and return them into a nested point vector
    std::vector<std::vector<cv::Point2f>> cubes_rect_points = identifyRects(cubes);

    std::cout << "Drawing cubes..." << '\n';

    // draw the rectangles on an image
    cubes = drawRects(img, cubes_rect_points);

    cv::Mat weighted;
    cv::addWeighted(img, 1, cubes, 0.5, 0.0, weighted);

    // show overlayed image
    // showImage(img, "identified_rects", 1000, 1000);
    showImage(weighted, "Output", 1000, 1000);

    std::cout << "ROI Loop starting." << '\n';

    // ROI removal loop
    int ROI_loop_check = 0;
    while(ROI_loop_check == 0){

        int k = cv::waitKey();
        if(k == 27){ // ESC key
            cv::destroyAllWindows;
            ROI_loop_check = -1;
        }
        if(k == 32){ // SPACE key
            
            cv::namedWindow("Manual Deletion Selection", cv::WINDOW_NORMAL);
            cv::resizeWindow("Manual Deletion Selection", 1000, 1000);
            cv::Rect2d ROI = cv::selectROI("Manual Deletion Selection", weighted, true, false);

            cv::destroyAllWindows;

            cubes_rect_points = searchRects(cubes_rect_points, ROI);
            
            cubes = drawRects(img, cubes_rect_points);
            cv::addWeighted(img, 1, cubes, 0.5, 0.0, weighted);
        }

    }

    std::cout << "ROI Loop Ended." << '\n';

    // export all data from rects_points to a .xlsx document
    if(export_to_xlsx[0] == 'y'){
        exportToXlsx(cubes_rect_points);
    }

    // save overlayed image
    if(save_image[0] == 'y'){
        cv::imwrite("rename me after writing.tiff", weighted);
    }

    std::cout << "Program End." << '\n';

    return 0;

}

