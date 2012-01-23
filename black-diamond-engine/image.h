//
//  image.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_image_h
#define black_diamond_engine_image_h

class Image {
    
    float pixels[1440][900];
    char* im_name;
    int x_res;
    int y_res;
    
public:
    Image() {
        char blank[5] = ""; im_name = blank; x_res = 0; y_res = 0;
    }
    
    Image(char* file_name, int xres, int yres, float p[1440][900]){
        im_name = file_name; x_res = xres; y_res = yres; 
        for (int i = 0; i < x_res; i++) 
            for (int j = 0; j < y_res; j++) 
                    pixels[i][j] = p[i][j];  
    }
    
    void write_png_file(char* file_name,float image[1440][900]);
    
};

#endif
