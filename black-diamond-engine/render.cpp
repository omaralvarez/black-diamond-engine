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
#include <vector>

void Render::trans_world_to_cam() {
    
    for (int i = 0; i < s.cloud.size(); i++) {
                
        bdm::Point new_point = s.cam.get_camera_point(s.cloud[i]); 
        
        s.cloud[i].x = new_point.x;
        s.cloud[i].y = new_point.y;
        s.cloud[i].z = new_point.z;
        
    }
    
}

void Render::filter_frustum() {
    
    std::vector<Surfel> frustum_cloud(0); 
    
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
        
    bdm::Transform ortho_trans = ortho_trans.orthographic(s.cam.hither, s.cam.yon);
    float world_max_x = fabsf(tanf(s.cam.angle_x/2)*s.cam.yon);
    float world_max_y = fabsf(tanf(s.cam.angle_y/2)*s.cam.yon);
    //std::vector<std::vector<std::vector<short> > > pix_vec(x_res,std::vector< std::vector<short> > (y_res, std::vector<short>(3,1337)));
    std::vector<std::vector<std::vector<short> > > pix_vec;
    std::vector<Surfel> clones(0);
    
    // Set up sizes. (HEIGHT x WIDTH x CHANNELS)
    pix_vec.resize(x_res);                     //Fors are faster than the direct declaration of the array with constructor.
    for (int i = 0; i < x_res; ++i)
        pix_vec[i].resize(y_res);
    for (int i = 0; i < x_res; ++i)
        for (int j = 0; j < y_res; j++)
            pix_vec[i][j].resize(3);
        
    
    for (int i = 0; i < x_res; i++) 
        for (int j = 0; j < y_res; j++) 
            pix_vec[i][j][0] = 1337;
        
    for (int i = 0; i < s.cloud.size(); i++) {
        
        bdm::Transform scales = scales.scale(80,80,80);
        bdm::Point point = scales(s.cloud[i]); 
        point = ortho_trans(point);
        
        int screen_x = roundf((x_res/(world_max_x*2))*(point.x+world_max_x));
        int screen_y = roundf((y_res/(world_max_y*2))*(point.y+world_max_y));
        
        if (pix_vec[x_res-screen_x][y_res-screen_y][0] == 1337) {
            
            //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
            pix_vec[x_res-screen_x][y_res-screen_y][0] = s.cloud[i].r;
            pix_vec[x_res-screen_x][y_res-screen_y][1] = s.cloud[i].g;
            pix_vec[x_res-screen_x][y_res-screen_y][2] = s.cloud[i].b;
            
        } else {
            
            int j=0;
            while (j < clones.size() && clones[j].x != point.x && clones[j].y != point.y) j++; //Search for point in clones.
            
            //If there is no point (x,y) in clones yet, we look for the z and add it.
            if (clones.size() == 0 || (clones[j].x != point.x && clones[j].y != point.y)) { 
                
                j=0;
                while (j < i && s.cloud[j].x != point.x && s.cloud[j].y != point.y) j++;
                
                if (s.cloud[j].z < point.z) clones.push_back(s.cloud[j]);
                else {
                    
                    //clones.push_back(point);
                    clones.push_back(Surfel(point.x,point.y,point.z,s.cloud[i].r,s.cloud[i].g,s.cloud[i].b,s.cloud[i].radius));
                    //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
                    pix_vec[x_res-screen_x][y_res-screen_y][0] = s.cloud[i].r;
                    pix_vec[x_res-screen_x][y_res-screen_y][1] = s.cloud[i].g;
                    pix_vec[x_res-screen_x][y_res-screen_y][2] = s.cloud[i].b;
                    
                }
                                
            } else {
                                
                if (clones[j].z > point.z) {
                    
                    //clones[j] = point;
                    clones[j] = Surfel(point.x,point.y,point.z,s.cloud[i].r,s.cloud[i].g,s.cloud[i].b,s.cloud[i].radius);
                    //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
                    pix_vec[x_res-screen_x][y_res-screen_y][0] = s.cloud[i].r;
                    pix_vec[x_res-screen_x][y_res-screen_y][1] = s.cloud[i].g;
                    pix_vec[x_res-screen_x][y_res-screen_y][2] = s.cloud[i].b;
                    
                }
                
            }
            
        }
        //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
        //s.cloud[i] = point;
        
    }
    
    /*for (int it=0; it<clones.size(); it++) {
        std::cout << "x: " << clones[it].x << " y: " << clones[it].y << " z: " << clones[it].z << " color: " << clones[it].color << std::endl;
    }*/
    std::cout << clones.size() << std::endl;
    
    char f_name[200] = "/Users/osurfer3/Desktop/test.png";

    //Recordar que casi me he quedado sin stack. Y ademas tengo que devolver pixels con puntero.
    Image *im = new Image(f_name,x_res,y_res,pix_vec);
    
    im->write_png_file(f_name);
    
}

