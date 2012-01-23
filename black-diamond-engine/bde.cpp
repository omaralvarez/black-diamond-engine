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

int main (int argc, const char * argv[])
{
    
    using namespace std;
    
    char filePath[200] = "/Users/osurfer3/Dropbox/PFC/Datasets/urban_scenes_sketchup/urban_scenes_sketchup/3d_man1/3d_man1-1.txt";
    Parser *new_parser;
    vector<Point> datos = new_parser->parse_data_file(filePath);
    //datos.push_back(Point(1,0,0));
    //datos.push_back(Point(0,1,0));
    //datos.push_back(Point(0,0,1));
    
    //Recuerda poner f en los float con decimales.
    Scene *new_scene = new Scene(datos,Camera(Point(10,-80,30),Point(0,0,0),Vector(0,0,-1),10,100,70.f,70.f));
    //Scene new_scene = Scene(datos,Camera(Point(-213.093f, 228.631f, 146.275f),Point(8.57638f, 6.96166f, 35.44f),Vector(0.235702f, -0.235702f, 0.942809f),10,100,80,80));
    
    cout << "x = " << new_scene->cloud[0].x << " y = " << new_scene->cloud[0].y << " z = " << new_scene->cloud[0].z << endl;
    
    Render *renderer = new Render(*new_scene,1440,900);
    
    delete new_scene;
    
    //cout << "Tamaño: " << new_scene->cloud.size() << " tan: " << tanf(45) << endl;
    
    renderer->trans_world_to_cam(); 
    
    //cout << "x = " << new_scene->cloud[0].x << " y = " << new_scene->cloud[0].y << " z = " << new_scene->cloud[0].z << endl;
    //Render debe tener scene como componente en vez de pasarselo.
    renderer->filter_frustum();
    
    renderer->get_pixel_info_ortho();
    
    //cout << "Tamaño: " << new_scene->cloud.size() << " tan: " << tanf(45) << endl;
    
    //Transform world_to_cam;
    //Transform world_to_cam2 = world_to_cam.look_at(Point(1,2,1),Point(1.33333,2.666666,1.666666),Vector(2,1,-2));
    
    //Point punto = Point(8,7,6);
    //Point punto = Point(4,0,6);
    
    //Point punto_trans = world_to_cam2(punto);
    
    //std::cout << punto_trans.x << " " << punto_trans.y << " " << punto_trans.z << std::endl;
    
    return 0;
    
}

