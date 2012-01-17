//
//  parser.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "parser.h"
#include "point.h"

Parser::Parser(){}

std::vector<Point> Parser::parse_data_file(std::string filePath) {
    
    using namespace std;
    
    ifstream inputFile;
    
    inputFile.open(filePath.c_str());
    
    vector<Point> data(0);
    float temp_x,temp_y,temp_z;
    
    while (inputFile.good()){
        
        inputFile >> temp_x >> temp_y >> temp_z;
        
        data.push_back(Point(temp_x,temp_y,temp_z));
        
    }
    
    return data;
}





