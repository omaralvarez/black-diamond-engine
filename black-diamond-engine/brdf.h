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
#include "surfel.h"

class Surfel;

class BRDF {
    
public:
    virtual std::vector<float> brdf(bdm::Vector sample, bdm::Vector view) = 0;
    virtual ~BRDF() {}
};

class BRDFDiffuse : public BRDF {

public:
    
	std::vector<float> brdf(bdm::Vector sample, bdm::Vector view) {
        //std::cout << "BRDF Diffuse" << std::endl;
        std::vector<float> rgb;
        return rgb;
    }
    
    
    
    ~BRDFDiffuse() {}
    
    friend class BRDF;
    
};

#endif
