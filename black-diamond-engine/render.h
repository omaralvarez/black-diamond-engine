//
//  render.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_render_h
#define black_diamond_engine_render_h

#include "scene.h"
#include "ray.h"
#include "kdtreeaccel.h"
#include <vector>
#include <iostream>
#include <fstream>

struct VisibilityTester;

class Render {
    
    int x_res;
    int y_res;
    Camera cam;
    std::vector<std::vector<Ray> > rays;
    
    void shading(Ray &ray);
    
public:
    Scene s;
    Render(){
        x_res = 0; y_res = 0; s = Scene(); cam = Camera();
    }
    
    //Creates a render with scene, camera and resolution.
    Render(Scene sc, Camera c, float x, float y){
        s = sc; cam = c; x_res = x; y_res = y;
    }
    
    void trans_world_to_cam();
    
    void filter_frustum();
    
    void get_pixel_info_ortho();
    
    void get_rays();
    
    void get_ray_hits();
    
    void get_kd_ray_hits();
    
    //Function used to export normals to file.
    void export_normals() {
        
        std::ofstream myfile;
        myfile.open("/Users/osurfer3/Dropbox/PFC/Datasets/custom_bde_datasets/normals.txt");
        
        if (myfile.is_open()){
            for (int i = 0; i < s.cloud.size(); i++) {
                
                bdm::Point new_point = cam.get_world_point(s.cloud[i]);
                bdm::Point temp = s.cloud[i] + s.cloud[i].normal;
                bdm::Point new_norm = cam.get_world_point(temp);
                
                myfile << new_point.x << " " << new_point.y << " " << -new_point.z << " " << new_norm.x << " " << new_norm.y << " " << -new_norm.z << "\n";
                
            }
            myfile.close();
        } else std::cout << "Unable to open file"<<std::endl;
        
    }
        
};

#endif
