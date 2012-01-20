//
//  camera.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_camera_h
#define black_diamond_engine_camera_h

#include "point.h"
#include "vector.h"
#include "transform.h"
//#include <iostream>


class Camera {
    
    Point origin;
    Point orientation; 
    Vector up;
    Transform world_to_cam;
    Transform cam_to_world;
    
public:
    float hither;
    float yon;
    float angle_x;
    float angle_y;
    
    Camera(){
        origin = Point();
        orientation = Point();
        up = Vector();
        hither = 0;
        yon = 0;
        angle_x = 0;
        angle_y = 0;
        world_to_cam = Transform();
        cam_to_world = Transform();
    }
    
    Camera(Point ori, Point orient, Vector u, float hith, float y, float ang_x, float ang_y){
        
        //std::cout << "x = " << origin.x << " y = " << origin.y << " z = " << origin.z << std::endl;
        //std::cout << "x = " << orientation.x << " y = " << orientation.y << " z = " << orientation.z << std::endl;
        //std::cout << "x = " << up.x << " y = " << up.y << " z = " << up.z << std::endl;
        origin = ori;
        orientation = orient;
        up = u;
        hither = hith;
        yon = y;
        angle_x = ang_x;
        angle_y = ang_y;
        Transform wtc = wtc.look_at(origin,orientation,up);
        world_to_cam = wtc;
        cam_to_world = wtc.get_inverse();
        
    }
    
    Point get_camera_point(Point p) {
        return world_to_cam(p);
    }
    
    Point get_world_point(Point p) {
        return cam_to_world(p);
    }
    
};

#endif
