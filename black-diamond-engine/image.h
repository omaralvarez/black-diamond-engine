//
//  image.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_image_h
#define black_diamond_engine_image_h

#include <vector>

class Image {
    
    std::vector<std::vector<float> > pixels;
    char* im_name;
    int x_res;
    int y_res;
    
public:
    Image() {
        char blank[5] = ""; im_name = blank; x_res = 0; y_res = 0;
    }
    
    Image(char* file_name, int xres, int yres, std::vector<std::vector<float> > p){
        im_name = file_name; x_res = xres; y_res = yres; pixels = p;
    }
    
    void write_png_file(char* file_name);
    
};

#endif
