//
//  main.cpp
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

int main (int argc, const char * argv[])
{

    using namespace std;
    
    //char filePath[200] = "/Users/osurfer3/Dropbox/PFC/Datasets/urban_scenes_sketchup/urban_scenes_sketchup/3d_man1/3d_man1-1.txt";
    //char filePath[200] = "/Users/osurfer3/Dropbox/PFC/Datasets/apple_rgb/apple_1.txt";
    char config_path[200] = "/Users/osurfer3/Dropbox/PFC/black-diamond-engine/config.xml";
    
    Parser *new_parser;
    Render renderer = new_parser->parse_config(config_path);
    
    cout << "Tamaño: " << renderer.s.cloud.size() << endl;
    
    renderer.trans_world_to_cam(); 
    renderer.filter_frustum();
    
    cout << "Tamaño: " << renderer.s.cloud.size() << endl;
    
    renderer.get_pixel_info_ortho();
    
    return 0;
    
}

