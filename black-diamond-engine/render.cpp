//
//  render.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include "render.h"
#include "point.h"
#include "scene.h"
#include "image.h"
#include <math.h>
#include <iostream>

void Render::trans_world_to_cam() {
    
    for (int i = 0; i < s.cloud.size(); i++) {
        Point new_point = s.cam.get_camera_point(s.cloud[i]); 
        s.cloud[i] = new_point;
    }
    
}

void Render::filter_frustum() {
    
    std::vector<Point> frustum_cloud(0); 
    
    for (int i = 0; i < s.cloud.size(); i++) {
        if ((s.cloud[i].z >= s.cam.hither && s.cloud[i].z <= s.cam.yon) &&
            (fabsf(s.cloud[i].x) >=0 && fabsf(s.cloud[i].x) <= fabsf(tanf(s.cam.angle_x/2)*s.cloud[i].z)) && 
            (fabsf(s.cloud[i].y) >=0 && fabsf(s.cloud[i].y) <= fabsf(tanf(s.cam.angle_y/2)*s.cloud[i].z))) {
            frustum_cloud.push_back(s.cloud[i]);
        }
    }
    
    s.cloud = frustum_cloud;
    
}

void Render::get_pixel_info_ortho(){
    
    //std::cout << "Peta aqui?" << std::endl;
    
    Transform ortho_trans = ortho_trans.orthographic(s.cam.hither, s.cam.yon);
    float world_max_x = fabsf(tanf(s.cam.angle_x/2)*s.cam.yon);
    float world_max_y = fabsf(tanf(s.cam.angle_y/2)*s.cam.yon);
    float pixels[1440][900];//Recordar que hay que usar x_res y y_res.
    //pngwriter png(1440,900,0,"test.png");
    
    //int color = 50; 
    
    for (int i = 0; i < s.cloud.size(); i++) {
        
        Transform scales = scales.scale(30,30,30);
        Point point = scales(s.cloud[i]); 
        //std::cout << "x = " << point.x << " y = " << point.y << " z = " << point.z << std::endl;
        point = ortho_trans(point);
        //std::cout << "x = " << point.x << " y = " << point.y << " z = " << point.z << std::endl;
        //std::cout << "----------------------- " << std::endl;
        int screen_x = roundf((x_res/(world_max_x*2))*(point.x+world_max_x));
        int screen_y = roundf((y_res/(world_max_y*2))*(point.y+world_max_y));
        pixels[screen_x][screen_y] = 255;
        s.cloud[i] = point;
        
    }
    
    
    char f_name[200] = "/Users/osurfer3/Desktop/test.png";

    //Recordar que casi me he quedado sin stack. Y ademas tengo que devolver pixels con puntero.
    Image *im = new Image(f_name,1440,900,pixels);
    
    im->write_png_file(f_name, pixels);
    
}

