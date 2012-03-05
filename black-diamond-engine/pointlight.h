//
//  pointlight.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/24/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_pointlight_h
#define black_diamond_engine_pointlight_h

#include "light.h"
#include "point.h"
#include "vector.h"

class PointLight : public Light {
    
    float intensity;
    
public:
    bdm::Point light_pos;
    
    PointLight(float intens) {
        
        light_pos = bdm::Point(0,0,-10);
        intensity = intens;
        
    }
    
    float sample_l(bdm::Point p, float p_epsilon, float time, bdm::Vector wi, float pdf);
    
    float power() {
        return 4.f * PI * intensity;
    }
    bool is_delta_light() {
        return true;
    }
    
};

#endif
