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

#include "surfel.h"
#include <vector>

class Ray {
    
public:
    bdm::Point o;
    bdm::Vector d;
    float mint, maxt, t_hit;
    Surfel hit;
    //Calculo media cortes.
    //std::vector<Surfel> hitlist;
    //std::vector<float> hitlist_t;
    //--------
    
    //Creates a default ray from almost 0 to infinity.
    Ray() {
        o = bdm::Point(); d = bdm::Vector(); mint = RAY_EPSILON; maxt = INFINITY; hit = Surfel(); t_hit = INFINITY;
    }
    
    //Creates a ray with origin, direction and minimum t value and maximum t value. The hit is initialized to infinity.
    Ray(bdm::Point ori, bdm::Vector dir, float min, float max) {
        o = ori; d = dir; mint=min; maxt = max; hit = Surfel(); t_hit = INFINITY;
    }
    
    //Obtains the point from the parametric value t. 
    bdm::Point operator()(float t) {
        return o + d * t;
    }
    
};

#endif
