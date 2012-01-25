//
//  image.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include <png.h>
#include "image.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3

void Image::write_png_file(char* file_name,float image[1440][900]){
    
        /* create file */
        FILE *fp = fopen(file_name, "wb");
        if (!fp)
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        png_structp png_ptr;
        png_infop info_ptr;
        
        //Init image data.
        int width = 1440;int height = 900; int bit_depth = 8;int color_type = 2;
        
        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        
        if (!png_ptr)
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        if (setjmp(png_jmpbuf(png_ptr)))
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        png_init_io(png_ptr, fp);
        
        
        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        
        
        png_write_info(png_ptr, info_ptr);
        
        
        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        png_bytep row_pointers[height];
        png_bytep row;
        
        for (int y=0; y<height; y++) {
            row_pointers[y] = (png_bytep) malloc(3 * width * sizeof(png_byte));
            row = row_pointers[y];
            for (int x=0; x<width; x++) {
                png_byte* ptr = &(row[x*3]);

                float rgb = image[x][y];

                uint32_t rgbi = *(uint32_t*)&rgb; // magia de punteros. No preguntes...
                uint8_t r,g,b;
                
                r = (rgbi & 0xFF0000) >> 16;
                g = (rgbi & 0x00FF00) >> 8;
                b = (rgbi & 0x0000FF);
                ptr[0] = r;
                ptr[1] = g;
                ptr[2] = b;
                
            }
        }
                
        png_write_image(png_ptr, row_pointers);
        
        
        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        png_write_end(png_ptr, NULL);
        
        /* cleanup heap allocation */
        for (int y=0; y<height; y++)
            free(row_pointers[y]);
        
        fclose(fp);
    
}