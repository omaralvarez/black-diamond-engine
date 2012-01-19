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

int main (int argc, const char * argv[])
{
    
    using namespace std;
    
    char filePath[200] = "/Users/osurfer3/Dropbox/PFC/Datasets/urban_scenes_sketchup/urban_scenes_sketchup/3d_man1/3d_man1-1.txt";
    Parser new_parser;
    vector<Point> datos = new_parser.parse_data_file(filePath);
    
    cout << "x = " << datos[1].x << " y = " << datos[1].y << " z = " << datos[1].z << endl;
    
    Transform world_to_cam;
    Transform world_to_cam2 = world_to_cam.look_at(Point(4,0,6),Point(4,0,7),Vector(0,1,0));
    
    Point punto = Point(6,3,11);
    //Point punto = Point(4,0,6);
    
    Point punto_trans = world_to_cam2(punto);
    
    std::cout << punto_trans.x << " " << punto_trans.y << " " << punto_trans.z << std::endl;
    
    return 0;
}

