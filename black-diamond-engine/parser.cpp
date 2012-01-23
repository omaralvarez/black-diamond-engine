//
//  parser.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//
//  Class in charge of parsing data files and config files.


#include <iostream>
#include <fstream>
#include "parser.h"
#include "point.h"

//Constructor.
Parser::Parser(){} 

//Function that parses a file with x,y and z values of each point.
std::vector<Point> Parser::parse_data_file(std::string filePath) {
    
    using namespace std;   
    
    ifstream inputFile;
    
    //Opens data file for reading.
    inputFile.open(filePath.c_str());
    
    //Creates vector, initially with 0 points.
    vector<Point> data(0);
    float temp_x,temp_y,temp_z;
    
    //Read contents of file till EOF.
    while (inputFile.good()){
        
        inputFile >> temp_x >> temp_y >> temp_z;
        data.push_back(Point(temp_x,temp_y,temp_z));
        
    }
    
    //Close data file.
    inputFile.close();
    
    return data;
}





