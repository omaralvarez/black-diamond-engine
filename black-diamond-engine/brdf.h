//
//  brdf.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 5/12/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_brdf_h
#define black_diamond_engine_brdf_h

#include <vector>
#include "ray.h"

class Ray;

class BRDF {
    
public:
    virtual std::vector<float> brdf(Ray *view, Ray *sample);
    
};

#endif
