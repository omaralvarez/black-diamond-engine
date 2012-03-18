//
//  bdesettings.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 3/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_bdesettings_h
#define black_diamond_engine_bdesettings_h

class BDESettings {
  
public:
    //Anti Aliasing
    bool AA;
    
    //Normal estimation.
    bool normal_est;
    bool est_accel;
    int r;
    float max_dist,min_dist;

    //Ray Tracing.
    bool kd_accel;
    int icost,tcost,maxs,max_depth;
    float ebonus;
    
    BDESettings() {
        
        AA = false;
        normal_est = false;
        est_accel = false;
        r = 2; max_dist = 0.1f; min_dist = 1e-04f;
        kd_accel = true;
        icost = 80; tcost = 1; ebonus = 0.5f; maxs = 1000; max_depth = -1;
        
    }
    
};

#endif
