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

#define PI 3.14159265f

class Camera {
    
    bdm::Point origin;
    bdm::Point orientation; 
    bdm::Vector up;
    bdm::Transform world_to_cam;
    bdm::Transform cam_to_world;
    
public:
    float hither;
    float yon;
    float angle_x;
    float angle_y;
    
    Camera(){
        origin = bdm::Point();
        orientation = bdm::Point();
        up = bdm::Vector();
        hither = 0;
        yon = 0;
        angle_x = 0;
        angle_y = 0;
        world_to_cam = bdm::Transform();
        cam_to_world = bdm::Transform();
    }
    
    Camera(bdm::Point ori, bdm::Point orient, bdm::Vector u, float hith, float y, float ang_x, float ang_y){
        
        //std::cout << "x = " << origin.x << " y = " << origin.y << " z = " << origin.z << std::endl;
        //std::cout << "x = " << orientation.x << " y = " << orientation.y << " z = " << orientation.z << std::endl;
        //std::cout << "x = " << up.x << " y = " << up.y << " z = " << up.z << std::endl;
        origin = ori;
        orientation = orient;
        up = u;
        hither = hith;
        yon = y;
        //angle_x = ang_x*PI/180;
        //angle_y = ang_y*PI/180;
        angle_x = ang_x;//Blender gives radian values.
        angle_y = ang_y;
        bdm::Transform wtc = wtc.look_at(origin,orientation,up);
        world_to_cam = wtc;
        cam_to_world = wtc.get_inverse();
        
    }
    
    bdm::Point get_camera_point(bdm::Point &p) {
        return world_to_cam(p);
    }
    
    bdm::Point get_world_point(bdm::Point &p) {
        return cam_to_world(p);
    }
    
};

#endif
