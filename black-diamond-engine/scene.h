//
//  scene.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_scene_h
#define black_diamond_engine_scene_h

#include <vector>
#include "camera.h"
#include "surfel.h"

class Scene {
    
public:
    std::vector<Surfel> cloud;
    Camera cam; 
    
    Scene(){
        std::vector<Surfel> data(0);
        cloud = data;
        cam = Camera();
    }
    
    Scene(std::vector<Surfel> data,Camera c) {
        cloud = data;
        cam = c;
    }
    
    ~Scene(){
        
    }
    
};

#endif
