//
//  montecarlo.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 4/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_montecarlo_h
#define black_diamond_engine_montecarlo_h

#include "sampler.h"
#include "surfel.h"
#include "scene.h"
#include <vector>

class MonteCarlo {
    
    int n_samples;
    Sampler sampler;
    //std::vector<MonteCarlo *> todo;
    
public:
    MonteCarlo() {
        n_samples = 4;
        //todo.resize(0);
    }
    
    std::vector<float> integrate(Scene *s, Surfel surfel, u_int32_t level); 
    
};

#endif
