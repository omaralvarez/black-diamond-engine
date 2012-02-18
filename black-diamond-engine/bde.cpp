//
//  bde.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/6/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "parser.h"
#include "camera.h"
#include "transform.h"
#include "scene.h"
#include "render.h"
#include "surfel.h"
#include "kdtreeaccel.h"//May erase.

int main (int argc, const char * argv[])
{

    using namespace std;
    
    //Remember parser axis order when reading different formats.
    char config_path[200] = "/Users/osurfer3/Dropbox/PFC/black-diamond-engine/config.xml";
    
    Parser *new_parser = new Parser();
    Render renderer = new_parser->parse_config(config_path);
    //cout << "x: " << renderer.s.cloud[1].x << " y: " << renderer.s.cloud[1].y << " z: " << renderer.s.cloud[1].z << endl;
    //cout << "x: " << renderer.s.cloud[2].x << " y: " << renderer.s.cloud[2].y << " z: " << renderer.s.cloud[2].z << endl;
    
    bdm::Transform scaler = scaler.scale(20,20,20);
    renderer.s.trans_scene(scaler);
    cout << "x: " << renderer.s.cloud[1].radius << " y: " << renderer.s.cloud[1].y << " z: " << renderer.s.cloud[1].z << endl;
    //cout << "x: " << renderer.s.cloud[2].x << " y: " << renderer.s.cloud[2].y << " z: " << renderer.s.cloud[2].z << endl;

    //cout << "Angulo: " << 70*PI/180 << endl;

    cout << "Tamaño: " << renderer.s.cloud.size() << endl;
    //bdm::Transform scaler = scaler.scale(80,80,80);
    //renderer.s.trans_scene(scaler);
    renderer.trans_world_to_cam(); 

    /*for (int i=0; i<renderer.s.cloud.size(); i++) {
        cout << "x: " << renderer.s.cloud[i].x << " y: " << renderer.s.cloud[i].y << " z: " << renderer.s.cloud[i].z << endl;
    }*/
    
    renderer.filter_frustum();
     
    /*for (int i=0; i<renderer.s.cloud.size(); i++) {
        cout  << renderer.s.cloud[i].x << " " << renderer.s.cloud[i].y << " " << renderer.s.cloud[i].z << endl;
    }*/
    cout << "Tamaño: " << renderer.s.cloud.size() << endl;
    //Def call: int icost = 80, int scost = 1,  float ebonus = 0.5f, int maxp = 1,int maxDepth = -1
    KdTreeAccel kd_tree = KdTreeAccel(renderer.s.cloud, 80, 1, 0.5f, 200, -1);
    
    //renderer.get_pixel_info_ortho();
    
    renderer.get_rays();
    cout << "Before tracing." << endl;
    renderer.get_kd_ray_hits(kd_tree);
    //renderer.get_ray_hits();
    cout << "After tracing." << endl;
    
    
    return 0;
    
}

