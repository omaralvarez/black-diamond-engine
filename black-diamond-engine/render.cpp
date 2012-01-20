//
//  render.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "render.h"
#include "point.h"
#include "scene.h"
#include <math.h>

Scene Render::trans_world_to_cam(Scene s) {
    
    for (int i = 0; i < s.cloud.size(); i++) {
        Point new_point = s.cam.get_camera_point(s.cloud[i]); 
        s.cloud[i] = new_point;
    }
    
    return s;
}

Scene Render::filter_frustum(Scene s) {
    
    std::vector<Point> frustum_cloud(0); 
    
    for (int i = 0; i < s.cloud.size(); i++) {
        if ((s.cloud[i].z >= s.cam.hither && s.cloud[i].z <= s.cam.yon) &&
            (fabsf(s.cloud[i].x) >=0 && fabsf(s.cloud[i].x) <= fabsf(tanf(s.cam.angle_x/2)*s.cloud[i].z)) && 
            (fabsf(s.cloud[i].y) >=0 && fabsf(s.cloud[i].y) <= fabsf(tanf(s.cam.angle_y/2)*s.cloud[i].z))) {
            frustum_cloud.push_back(s.cloud[i]);
        }
    }
    
    s.cloud = frustum_cloud;
    
    return s;
    
}