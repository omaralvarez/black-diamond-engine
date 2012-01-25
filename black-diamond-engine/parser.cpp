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
    float temp_x,temp_y,temp_z,discard_1=0,discard_2=0;
    double rgb=0;
    //inputFile.setf(ios_base::scientific,ios_base::floatfield);
    //inputFile.precision(7);
    //Read contents of file till EOF.
    while (inputFile.good()){
        
        //inputFile >> temp_x >> temp_y >> temp_z;
        //string line;
        //for (int i=0; i<10; i++) getline (inputFile,line);
        
        inputFile >> temp_x >> temp_y >> temp_z >> rgb >> discard_1 >> discard_2;
        //cout << temp_x << " " << temp_y << " "<< temp_z << " "<< float(rgb) << " "<<discard_1 << " " << discard_2 << endl;
        data.push_back(Point(temp_x,temp_y,temp_z,float(rgb)));
        
    }
    
    if (!inputFile.eof())
        if (inputFile.fail()) cout << "Type mismatch during parsing." << endl;
        else cout << "Unknow problem during parsing." << endl;
    
    //Close data file.
    inputFile.close();
    
    return data;
}





