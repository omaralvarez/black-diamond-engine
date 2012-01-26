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
#include <vector>
#include "parser.h"
#include "point.h"
#include "tinyxml.h"
#include "scene.h"
#include "vector.h"

//Constructor.
Parser::Parser(){} 

//Parses a data file with all the neccessary data for the app to work.
Render Parser::parse_config(char* config_path){
    
    int ren[2];      //Render data.
    float cam[13];   //Camera data.
    char d_path[200];//Data file name.
    int node=0;      //Render, camera or filename switch.
    
    TiXmlDocument doc(config_path); //Load config file.
	bool loadOkay = doc.LoadFile();
    
	if (loadOkay) {
        
        TiXmlNode* pParent = &doc;  //Access parent node and advance to first child node to pass the declaration.
        pParent = pParent->FirstChild()->NextSibling();
        TiXmlNode* pChild;
        
		for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
            
            int t = pChild->Type(); //See if the object is a property element.
            
            if (t==TiXmlNode::TINYXML_ELEMENT) {
                
                TiXmlElement* pElement = pChild->ToElement();
                TiXmlAttribute* pAttrib=pElement->FirstAttribute();
                int i=0;
                int ival;
                double val;

                while (pAttrib) {
                    
                    switch (node) {     //Fill the correct array depending on the node that we are. Camera, Render or data file.
                        case 0:
                            if (pAttrib->QueryDoubleValue(&val)==TIXML_SUCCESS) cam[i] = float(val);    //Camera data.
                            break;
                        case 1:
                            if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS) ren[i] = ival;            //Render data.
                            break;
                        case 2:
                            strcpy(d_path, pAttrib->Value());                                           //Data file.
                            break;
                            
                        default:
                            std::cout << "Error while parsing xml." << std::endl;
                            break;
                    }
                    
                    i++;
                    pAttrib=pAttrib->Next();
                }
                node++;
            }
            
            
        }
        
        std::vector<Point> data = parse_data_file(d_path);
        
        return Render(Scene(data,Camera(Point(cam[0],cam[1],cam[2]),Point(cam[3],cam[4],cam[5]),Vector(cam[6],cam[7],cam[8]),cam[9],cam[10],cam[11],cam[12])),ren[0],ren[1]);
        
	} else {
		printf("Failed to load file \"%s\"\n", config_path);
        return Render();
	}
    
    return Render();
    
}

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

    //Read contents of file till EOF.
    while (inputFile.good()){
        
        inputFile >> temp_x >> temp_y >> temp_z >> rgb >> discard_1 >> discard_2;
        data.push_back(Point(temp_x,temp_y,temp_z,float(rgb)));
        
    }
    
    if (!inputFile.eof())
        if (inputFile.fail()) cout << "Type mismatch during parsing." << endl;
        else cout << "Unknow problem during parsing." << endl;
    
    //Close data file.
    inputFile.close();
    
    return data;
}





