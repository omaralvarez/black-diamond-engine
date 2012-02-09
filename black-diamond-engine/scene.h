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
    
    Scene(){
        std::vector<Surfel> data(0);
        cloud = data;
    }
    
    Scene(std::vector<Surfel> data) {
        cloud = data;
    }
    
    ~Scene() {
        
    }
    
    void trans_scene(bdm::Transform t) {
        
        for (int i = 0; i < cloud.size(); i++) {
            bdm::Point new_p = t(cloud[i]);
            cloud[i].x = new_p.x; cloud[i].y = new_p.y; cloud[i].z = new_p.z;
        }
        
    }
    
};

#endif
