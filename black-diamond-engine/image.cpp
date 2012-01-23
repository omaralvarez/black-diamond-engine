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
        
        /*png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)user_error_ptr,user_error_fn, user_warning_fn); 
         if (!png_ptr)
         return (ERROR);
         png_infop info_ptr = png_create_info_struct(png_ptr); 
         if (!info_ptr)
         {
         png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
         return (ERROR); 
         }*/
        
        png_structp png_ptr;
        png_infop info_ptr;
        
        //std::cout << "**x: " << 711 << " y: " << 455 <<  " value: "  << image[711][455] << std::endl;
        
        int width = 1440;int height = 900; int bit_depth = 8;int color_type = 2;
        
        /*png_ptr = malloc(sizeof (png_struct));
         if (!png_ptr)
         {
         fclose(fp);
         return;
         }
         
         info_ptr = malloc(sizeof (png_info));
         if (!info_ptr)
         {
         fclose(fp);
         free(png_ptr);
         return;
         }*/
        
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
        png_bytep row;// = (png_bytep) malloc(3 * width * sizeof(png_byte));
        
        for (int y=0; y<height; y++) {
            row_pointers[y] = (png_bytep) malloc(3 * width * sizeof(png_byte));
            row = row_pointers[y];
            for (int x=0; x<width; x++) {
                png_byte* ptr = &(row[x*3]);
                //printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                //x, y, ptr[0], ptr[1], ptr[2], ptr[3]);
                
                /* set red value to 0 and green value to the blue one */
                ptr[0] = image[x][y];
                ptr[1] = image[x][y];
                ptr[2] = image[x][y];
                
                //std::cout << image[x][y] << std::endl;
                
            }
        }
        
        //png_bytep row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        
        png_write_image(png_ptr, row_pointers);
        
        
        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
            std::cout << "[write_png_file] File %s could not be opened for writing" << std::endl;
        
        png_write_end(png_ptr, NULL);
        
        /* cleanup heap allocation */
        for (int y=0; y<height; y++)
            free(row_pointers[y]);
        //free(row);
        
        fclose(fp);
    
}