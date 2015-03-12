/*
 *	bde.cpp
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

#ifdef _OPENMP
#include <omp.h>
#endif

//#include <boost/timer/timer.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "parser.h"
#include "camera.h"
#include "transform.h"
#include "scene.h"
#include "render.h"
#include "surfel.h"
#include "bdesettings.h"

BDESettings settings = BDESettings();

int main (int argc, const char * argv[]) {
        
    //boost::timer::auto_cpu_timer t;
    
    using namespace std;
    
    //Remember parser axis order when reading different formats.
    char config_path[200] = "/Users/osurfer3/Dropbox/PFC/black-diamond-engine/blender_config.xml";
    std::cout << "Reading data..." << std::endl;
    Parser *new_parser = new Parser();
    Render renderer = new_parser->parse_config(config_path);
    
    std::cout << "Applying transformations..." << std::endl;
    //bdm::Transform scaler = scaler.scale(20,20,20);
    //renderer.s.trans_scene(scaler);
    //bdm::Transform trans = trans.translate(4);
    //renderer.s.trans_scene(trans);
    
    //renderer.s.get_normals();
        
    cout << "Size before filtering: " << renderer.s.cloud.size() << endl;

    renderer.trans_world_to_cam(); 

    std::cout << "Filtering frustum..." << std::endl;
    //renderer.filter_frustum();
    
    if(settings.normal_est && !settings.est_accel) //renderer.s.get_normals();
    //renderer.s.get_radius();
    //renderer.s.cloud[0].normal = bdm::Vector(0,0,-1);
    //renderer.s.cloud[1].normal = bdm::Vector(0,0,-1);
    //renderer.export_normals();
     
    /*for (int i=0; i<renderer.s.cloud.size(); i++) {
        cout  << renderer.s.cloud[i].x << " " << renderer.s.cloud[i].y << " " << renderer.s.cloud[i].z << endl;
    }*/
    std::cout << "Size after filtering: " << renderer.s.cloud.size() << std::endl;
    //Def call: int icost = 80, int scost = 1,  float ebonus = 0.5f, int maxp = 1,int maxDepth = -1
    //renderer.s.kd_tree = kd_tree;
    
    std::cout << "Creating kd-tree..." << std::endl;
    if(settings.kd_accel) renderer.s.create_kd_tree(80, 1, 0.5f, settings.maxs, -1);
    
    if(settings.normal_est && settings.est_accel) renderer.s.get_normals_accel();
    
    //std::cout << renderer.s.cloud[0].normal.x << std::endl;
    
    //renderer.get_pixel_info_ortho();
    std::cout << "Generating rays..." << std::endl;
    renderer.get_rays();
    cout << "Starting tracing..." << endl;
    if (settings.kd_accel) renderer.get_kd_ray_hits();
    else renderer.get_ray_hits();
    cout << "Tracing finished..." << endl;
    
    delete new_parser;
    
    return 0;
    
}

