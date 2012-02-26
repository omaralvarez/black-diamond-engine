//
//  light.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/24/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_light_h
#define black_diamond_engine_light_h

#include "point.h"
#include "vector.h"

#define PI 3.14159265f

class Light {
    
public:
    virtual float sample_l(bdm::Point p, float p_epsilon, float time, bdm::Vector wi, float pdf) {
        return 0;
    }
    
    virtual float power() {
        return 0;
    }
    
    virtual bool is_delta_light() {
        return false;
    }
    
};

#endif
