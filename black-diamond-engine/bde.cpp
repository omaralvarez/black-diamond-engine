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
#include "parser.h"
#include "camera.h"
#include "transform.h"
#include "scene.h"
#include "render.h"
#include <math.h>

#define PI 3.14159265

int main (int argc, const char * argv[])
{
    
    using namespace std;
    
    char filePath[200] = "/Users/osurfer3/Dropbox/PFC/Datasets/urban_scenes_sketchup/urban_scenes_sketchup/3d_man1/3d_man1-1.txt";
    Parser new_parser;
    vector<Point> datos = new_parser.parse_data_file(filePath);
    datos.push_back(Point(-1.77,3,16.1));
    
    Scene new_scene = Scene(datos,Camera(Point(4,0,6),Point(4,0,7),Vector(0,1,0),10,30,60*PI/180,60*PI/180));
    
    cout << "x = " << new_scene.cloud[0].x << " y = " << new_scene.cloud[0].y << " z = " << new_scene.cloud[0].z << endl;
    
    Render renderer = Render();
    
    new_scene = renderer.trans_world_to_cam(new_scene); 
    
    cout << "x = " << new_scene.cloud[new_scene.cloud.size()-1].x << " y = " << new_scene.cloud[new_scene.cloud.size()-1].y << " z = " << new_scene.cloud[new_scene.cloud.size()-1].z << endl;
    
    new_scene = renderer.filter_frustum(new_scene);
    
    cout << "Tamaño: " << new_scene.cloud.size() << " tan: " << tanf(45*PI/180) << endl;
    
    //Transform world_to_cam;
    //Transform world_to_cam2 = world_to_cam.look_at(Point(1,2,1),Point(1.33333,2.666666,1.666666),Vector(2,1,-2));
    
    //Point punto = Point(8,7,6);
    //Point punto = Point(4,0,6);
    
    //Point punto_trans = world_to_cam2(punto);
    
    //std::cout << punto_trans.x << " " << punto_trans.y << " " << punto_trans.z << std::endl;
    
    return 0;
}

