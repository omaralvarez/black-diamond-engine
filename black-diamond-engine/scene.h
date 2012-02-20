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
#include "kdtreeaccel.h"

class Scene {
    
public:
    KdTreeAccel *kd_tree;
    std::vector<Surfel> cloud;
    
    Scene(){
        kd_tree = NULL;
        std::vector<Surfel> data(0);
        cloud = data;
    }
    
    Scene(std::vector<Surfel> data) {
        kd_tree = NULL;
        cloud = data;
    }
    
    ~Scene() {
        
        delete kd_tree;
        
    }
    
    void create_kd_tree(int icost, int tcost, float ebonus, int maxs, int max_depth) {
        
        kd_tree = new KdTreeAccel(cloud, icost, tcost, ebonus, maxs, max_depth);
        
    }
    
    void trans_scene(bdm::Transform t) {
        
        #pragma omp parallel for
        for (int i = 0; i < cloud.size(); i++) {
            bdm::Point new_p = t(cloud[i]);
            cloud[i].x = new_p.x; cloud[i].y = new_p.y; cloud[i].z = new_p.z;
        }
        
    }
    
};

#endif
