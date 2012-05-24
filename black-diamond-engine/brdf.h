//
//  brdf.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 5/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_brdf_h
#define black_diamond_engine_brdf_h

#include <vector>
#include <iostream>
#include "vector.h"

class BRDF {
    
public:
    virtual float diffuse(bdm::Vector sample, bdm::Vector normal) = 0;
    virtual float specular(bdm::Vector sample, bdm::Vector view, bdm::Vector normal) = 0;
    static BRDF* create(std::string type);
    virtual ~BRDF() {}
};

class BRDFSimple : public BRDF {

public:
    
    //Compute diffuse reflection.
	float diffuse(bdm::Vector sample, bdm::Vector normal) {
        
        float cos_t = sample.dot(normal);
        
        return cos_t;
        
    }
    
    float specular(bdm::Vector sample, bdm::Vector view, bdm::Vector normal) {
        
        bdm::Vector h = (sample + (-(view))).normalize();
        float cos_h = h.dot(normal);
        float phong = powf(cos_h,50.f);
        
        return phong;
    }

    ~BRDFSimple() {}
    
    friend class BRDF;
    
};

#endif
