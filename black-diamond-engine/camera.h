/*
 *	camera.h
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
    
    //Creates a camera with origin, orientation, up vector, near clipping plane, far clipping plane and vision angles.
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
    
    //Obtains point in camera coordinates.
    bdm::Point get_camera_point(bdm::Point &p) {
        return world_to_cam(p);
    }
    
    bdm::Vector get_camera_normal(bdm::Vector &v) {
        return world_to_cam(v);
    }
    
    //Obtains point in world coordinates.
    bdm::Point get_world_point(bdm::Point &p) {
        return cam_to_world(p);
    }
    
    bdm::Vector get_world_normal(bdm::Vector &v) {
        return cam_to_world(v);
    }
    
};

#endif
