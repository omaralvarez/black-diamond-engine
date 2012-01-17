//
//  camera.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_camera_h
#define black_diamond_engine_camera_h

#include "vector.h"

class Camera {
    
public:
    float x,y,z;
    Vector orientation; 
    float hither;
    float yon;
    float angle;
    
    Camera(float _x=0, float _y=0, float _z=0, float _hither=0, float _yon=0, float _angle=0, Vector _orientation=Vector(0,0,0))
    : x(_x), y(_y), z(_z), hither(_hither), yon(_yon), angle(_angle), orientation(_orientation) {}
    
};

#endif
