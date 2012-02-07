//
//  ray.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/6/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_ray_h
#define black_diamond_engine_ray_h

#define RAY_EPSILON 1e-3f

#ifndef INFINITY            //Hay que mirar como ponerlo en bdm.
#define INFINITY FLT_MAX
#endif

class Ray {
    
public:
    bdm::Point o;
    bdm::Vector d;
    float mint, maxt;
    
    Ray() {
        o = bdm::Point(); d = bdm::Vector(); mint = RAY_EPSILON; maxt = INFINITY;
    }
    
    Ray(bdm::Point ori, bdm::Vector dir, float min, float max) {
        o = ori; d = dir; mint=min; maxt = max;
    }
    
    bdm::Point operator()(float t) {
        return o + d * t;
    }
    
};

#endif
