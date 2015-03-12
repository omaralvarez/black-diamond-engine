/*
 *	scene.h
 *	black-diamond-engine
 *
 *	Created by Luis Omar Alvarez Mures on 2/13/12.
 *	Copyright (c) 2012
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef black_diamond_engine_scene_h
#define black_diamond_engine_scene_h

#include <vector>
#include "camera.h"
#include "surfel.h"
#include "kdtreeaccel.h"
#include "light.h"
#include "pointlight.h"
#include <boost/timer/timer.hpp>

class Scene {
    
public:
    KdTreeAccel *kd_tree;
    std::vector<Surfel> cloud;
    std::vector<PointLight> lights;
    
    Scene(){
        kd_tree = NULL;
        std::vector<Surfel> data(0);
        cloud = data;
    }
    
    //Creates scene with lights or without them.
    Scene(std::vector<Surfel> data) {
        kd_tree = NULL;
        cloud = data;
    }
    
    Scene(std::vector<Surfel> data, PointLight l) {
        kd_tree = NULL;
        cloud = data;
        lights.push_back(l); 
    }
    
    Scene(std::vector<Surfel> data, std::vector<PointLight> l) {
        kd_tree = NULL;
        cloud = data;
        lights = l; 
    }
    
    ~Scene() {
        
        delete kd_tree;
        
    }
    
    void create_kd_tree(int icost, int tcost, float ebonus, int maxs, int max_depth) {
        boost::timer::auto_cpu_timer t;
        
        kd_tree = new KdTreeAccel(cloud, icost, tcost, ebonus, maxs, max_depth);
        
    }
    
    //Applies transform t to the scene.
    void trans_scene(bdm::Transform t) {
        
        #pragma omp parallel for
        for (int i = 0; i < cloud.size(); i++) {
            bdm::Point new_p = t(cloud[i]);
            cloud[i].x = new_p.x; cloud[i].y = new_p.y; cloud[i].z = new_p.z;
        }
        
    }
    
    void get_normals();
    
    void get_radius();
    
    void get_normals_accel();
    
};

#endif
