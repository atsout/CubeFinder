#include <iostream>
#include <string>
#include <vector>

float getConvertData(std::string convertIn){
    
    float convertOut;
    const int convertTableSize = 30;

    const std::string convertTableIn[convertTableSize] = {
    // T12 microscope
    "560", "1.65K", "2.1K", "2.7K", "3.2K", "4.4K", "6.5K", 
    "11K", "15K", "21K", "26K", "30K", "42K", "52K", "67K", 
    "110K", "150K", "220K", "265K", "350K", "400K", "570K",
    
    // 2200FS microscope
    "25K", "100K", "500K", "1M"
    
    };

    const float convertTableOut[convertTableSize] = {
    // T12 microscope
    0.0427, 0.1259, 0.1602, 0.2060, 0.2441, 0.3356, 0.4958, 
    0.8391, 1.1442, 1.6019, 1.9833, 2.2885, 3.2038, 3.9667, 
    5.1109, 8.3910, 11.4423, 16.7821, 20.2147, 26.6987,
    30.5128, 43.4808,
    
    // 2200FS microscope
    2.595, 8.6, 49.75, 89.6

    };

    for(int i = 0; i < convertTableSize; i++){
        if(convertTableIn[i] == convertIn){
            return convertTableOut[i];
        }
    }

    std::cout << "Image magnification entered not found in conversion table." << '\n';
    exit(0);

}


