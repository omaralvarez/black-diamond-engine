//
//  ray.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/6/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include "ray.h"

Ray::Ray() {
    o = bdm::Point(); d = bdm::Vector(); mint = RAY_EPSILON; maxt = INFINITY; hit = NULL; t_hit = INFINITY;
}

Ray::Ray(bdm::Point ori, bdm::Vector dir, float min, float max) {
    o = ori; d = dir; mint=min; maxt = max; hit = NULL; t_hit = INFINITY;
}