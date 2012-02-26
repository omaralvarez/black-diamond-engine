//
//  pointlight.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/24/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "pointlight.h"
#include "point.h"
#include "vector.h"

float PointLight::sample_l(bdm::Point p, float p_epsilon, float time, bdm::Vector wi, float pdf) {
    
    wi = (light_pos - p).normalize();
    pdf = 1.f;
    
    return intensity/(light_pos - p).length_sqrd();
    
}