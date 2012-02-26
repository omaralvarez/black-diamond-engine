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

struct VisibilityTester;

class Render {
    
    int x_res;
    int y_res;
    Camera cam;
    std::vector<std::vector<Ray> > rays;
    
public:
    Scene s;
    Render(){
        x_res = 0; y_res = 0; s = Scene(); cam = Camera();
    }
    
    Render(Scene sc, Camera c, float x, float y){
        s = sc; cam = c; x_res = x; y_res = y;
    }
    
    void trans_world_to_cam();
    
    void filter_frustum();
    
    void get_pixel_info_ortho();
    
    void get_rays();
    
    void get_ray_hits();
    
    void get_kd_ray_hits();
        
};

#endif
