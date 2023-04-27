#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "parse.hpp"

int readSetting(char* text, int line){
    
    FILE* fp = NULL;
    line -= 1;

    // open file
    fp = fopen("settings.txt", "r");
    if(fp == NULL){
        printf("Failed to open settings file.  \n");
        exit(1);
    }
    
    // find requested line
    for(int i = 0; i < line; ++i){
        fscanf(fp, "%*[^\n]\n");
    }
    
    // find text between <symbols> and copy into c-style string
    int c;
    text[0] = '\0';
    while((c = getc(fp)) != '<' && c != '\n'){
        ;
    }
    while((c = getc(fp)) != '>' && c != '\n'){
        char ch = c;
        strncat(text, &ch, 1);
    }

    fclose(fp);

    return 0;

}

// turn string from file into double that can be used for function parameters
double settingToDouble(char* text){
    double v;
    v = std::strtod(text, NULL);
    return v;
}



