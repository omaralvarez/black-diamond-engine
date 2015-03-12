/*
 *	image.h
 *	black-diamond-engine
 *
 *	Created by Luis Omar Alvarez Mures on 2/13/12.
 *	Copyright (c) 2012
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef black_diamond_engine_image_h
#define black_diamond_engine_image_h

#include <vector>

class Image {
    
    std::vector<std::vector<std::vector<short> > > pixels;
    char* im_name;
    int x_res;
    int y_res;
    
public:
    //Creates a png image.
    Image() {
        char blank[5] = ""; im_name = blank; x_res = 0; y_res = 0;
    }
    
    Image(char* file_name, int xres, int yres, std::vector<std::vector<std::vector<short> > > p){
        im_name = file_name; x_res = xres; y_res = yres; pixels = p;
    }
    
    void write_png_file(char* file_name);
    
};

#endif
