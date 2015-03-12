/*
 *	parser.cpp
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

#include <iostream>
#include <fstream>
#include "parser.h"
#include "tinyxml.h"
#include "scene.h"
#include "vector.h"
//#include "material.h"

//Constructor.
Parser::Parser(){} 

//Parses a data file with all the neccessary data for the app to work.
Render Parser::parse_config(char* config_path){
    
    int ren[2];      //Render data.
    float cam[13];   //Camera data.
    char d_path[200];//Data file name.
    float radius=0;    //Surfel radius.
    int node=0;      //Render, camera or filename switch.
    std::string opt("surfel_radius"); 
    
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
                            //Render data.
                            if (opt.compare(pAttrib->Name()) && pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS && i < 2) ren[i] = ival; 
                            else if (pAttrib->QueryDoubleValue(&val)==TIXML_SUCCESS && i < 3) radius = float(val);
                            
                            if (!strncmp(pAttrib->Name(),"AA",2) && !strncmp(pAttrib->Value(),"true",4)) settings.AA = true;
                            if (!strncmp(pAttrib->Name(),"AA",2) && !strncmp(pAttrib->Value(),"false",5)) settings.AA = false;
                            
                            if (!strncmp(pAttrib->Name(),"normal_est",10) && !strncmp(pAttrib->Value(),"true",4)) settings.normal_est = true;
                            if (!strncmp(pAttrib->Name(),"normal_est",10) && !strncmp(pAttrib->Value(),"false",5)) settings.normal_est = false;
                            
                            if (!strncmp(pAttrib->Name(),"est_accel",9) && !strncmp(pAttrib->Value(),"true",4)) settings.est_accel = true;
                            if (!strncmp(pAttrib->Name(),"est_accel",9) && !strncmp(pAttrib->Value(),"false",5)) settings.est_accel = false;
                            
                            if (!strncmp(pAttrib->Name(),"r_func",6) && pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS) settings.r = ival;
                            if (!strncmp(pAttrib->Name(),"max_dist",8) && pAttrib->QueryDoubleValue(&val)==TIXML_SUCCESS) settings.max_dist = float(val);
                            if (!strncmp(pAttrib->Name(),"min_dist",8) && pAttrib->QueryDoubleValue(&val)==TIXML_SUCCESS) settings.min_dist = float(val);
                            
                            if (!strncmp(pAttrib->Name(),"kd_accel",8) && !strncmp(pAttrib->Value(),"true",4)) settings.kd_accel = true;
                            if (!strncmp(pAttrib->Name(),"kd_accel",8) && !strncmp(pAttrib->Value(),"false",5)) settings.kd_accel = false;
                            
                            if (!strncmp(pAttrib->Name(),"kd_nodes",8) && pAttrib->QueryDoubleValue(&val)==TIXML_SUCCESS) settings.maxs = float(val);
                            
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
    
        std::vector<Surfel> data = parse_data_file_v002(d_path,radius);
        
        std::vector<PointLight> lights = parse_lights_file("/Users/osurfer3/Dropbox/PFC/Datasets/custom_bde_datasets/blender.bdl");
        
        return Render(Scene(data, lights),Camera(bdm::Point(cam[0],cam[1],cam[2]),bdm::Point(cam[3],cam[4],cam[5]),bdm::Vector(cam[6],cam[7],cam[8]),cam[9],cam[10],cam[11],cam[12]),ren[0],ren[1]);
        
	} else {
		printf("Failed to load file \"%s\"\n", config_path);
        return Render();
	}
    
    return Render();
    
}

//Reads lights from lights data file.
std::vector<PointLight> Parser::parse_lights_file(std::string filePath) {
    
    using namespace std;   
    
    ifstream inputFile;
    
    //Opens data file for reading.
    inputFile.open(filePath.c_str());
    
    //Creates vector, initially with 0 points.
    vector<PointLight> data(0);
    float temp_x,temp_y,temp_z;
    
    //Read contents of file till EOF.
    while (inputFile.good()){
                 
        char c = inputFile.peek();
        
        if(c != '\n'){
        
            inputFile >> temp_x >> temp_y >> temp_z;                                        
            
            PointLight l = PointLight(bdm::Point(temp_x,temp_y,temp_z),0);
            
            data.push_back(l);
            
        } else inputFile.ignore(2); //Takes care of the last white line.
        
    }
    
    if (!inputFile.eof())
        if (inputFile.fail()) cout << "Type mismatch during parsing." << endl;
        else cout << "Unknow problem during parsing." << endl;
    
    //Close data file.
    inputFile.close();
    
    return data;
    
}


//Function that parses a file with x,y and z values of each point.
std::vector<Surfel> Parser::parse_data_file_v002(std::string filePath, float radius) {
    
    using namespace std;   
    
    ifstream inputFile;
    
    //Opens data file for reading.
    inputFile.open(filePath.c_str());
    
    //Creates vector, initially with 0 points.
    vector<Surfel> data(0);
    float temp_x,temp_y,temp_z,normal_x=0,normal_y=0,normal_z=0;
    double rgbd=0;
    std::string version;
    std::string latest_version("v0.02");
    
    if (inputFile.good()) inputFile >> version;
    
    if (version.compare(latest_version)) {
        std::cout << "Error: Wrong config file version. Latest version is " << latest_version << std::endl;
        return data;
    } 
    
    
    //Read contents of file till EOF.
    while (inputFile.good()){
        
        //Translation. //This should be last transformation applied if we want total integration with blender.
        float trans1=0, trans2=0, trans3=0;
        inputFile >> trans1 >> trans2 >> trans3;
        bdm::Transform translation = translation.translate(bdm::Vector(trans1,trans2,trans3));
        
        //Rotation.
        float rot1=0, rot2=0, rot3=0;
        inputFile >> rot1 >> rot2 >> rot3;
        bdm::Transform rot_x = rot_x.rotate_x(rot1);
        bdm::Transform rot_y = rot_y.rotate_y(rot2);
        bdm::Transform rot_z = rot_z.rotate_z(rot3);
        
        //Scaling.
        float sc1=0, sc2=0, sc3=0;
        inputFile >> sc1 >> sc2 >> sc3;
        bdm::Transform scaling = scaling.scale(sc1,sc2,sc3);
        
        //Material.
        float a_r=0,a_g=0,a_b=0;
        inputFile >> a_r >> a_g >> a_b;
        float d_r=0,d_g=0,d_b=0;
        inputFile >> d_r >> d_g >> d_b;
        float s_r=0,s_g=0,s_b=0;
        inputFile >> s_r >> s_g >> s_b;
        float a[3] = {a_r,a_g,a_b};
        float d[3] = {d_r,d_g,d_b};
        float s[3] = {s_r,s_g,s_b};
        float exp=0,em=0;
        inputFile >> exp >> em;
        //inputFile >> temp_x >> temp_y >> temp_z >> rgbd >> discard_1 >> discard_2;    //Original.
        //inputFile >> temp_x >> temp_z >> temp_y >> rgbd >> discard_1 >> discard_2;    //Dataset RGB-D.
        u_int32_t n_sur=0;
        inputFile >> n_sur;
        
        data.reserve(data.size() + n_sur);
        
        for(int i = 0; i < n_sur; i++){                                                 //.bde file type.
            
            inputFile >> temp_x >> temp_y >> temp_z >> normal_x >> normal_y >> normal_z;                                    
            
            bdm::Point new_p = bdm::Point(temp_x,temp_y,temp_z);
            bdm::Vector normal = bdm::Vector(normal_x,normal_y,normal_z);
            
            new_p = rot_x(new_p);
            new_p = rot_y(new_p);
            new_p = rot_z(new_p);
            new_p = scaling(new_p);
            new_p = translation(new_p);
            
            normal = rot_x(normal);
            normal = rot_y(normal);
            normal = rot_z(normal);
            /*normal = scaling(normal);
            normal = translation(normal);*/
            
            //BRDF *p = new BRDFDiffuse();
            if (em) data.push_back(Surfel(new_p.x,new_p.y,new_p.z,0,0,0,1,Material(a,d,s,exp,em),normal));
            else data.push_back(Surfel(new_p.x,new_p.y,new_p.z,0,0,0,radius,Material(a,d,s,exp,em),normal));
            
        }
        
    }
    
    if (!inputFile.eof())
        if (inputFile.fail()) cout << "Type mismatch during parsing." << endl;
        else cout << "Unknow problem during parsing." << endl;
    
    //Close data file.
    inputFile.close();
    
    return data;
    
}

//Function that parses a file with x,y and z values of each point.
std::vector<Surfel> Parser::parse_data_file(std::string filePath, float radius) {
    
    using namespace std;   
    
    ifstream inputFile;
    
    //Opens data file for reading.
    inputFile.open(filePath.c_str());
    
    //Creates vector, initially with 0 points.
    vector<Surfel> data(0);
    float temp_x,temp_y,temp_z,discard_1=0,discard_2=0,rgbf=0;
    double rgbd=0;
    std::string version;
    std::string latest_version("v0.01");
    
    if (inputFile.good()) inputFile >> version;
    
    if (version.compare(latest_version)) {
        std::cout << "Error: Wrong config file version. Latest version is " << latest_version << std::endl;
        return data;
    } 
    
    
    //Read contents of file till EOF.
    while (inputFile.good()){
        
        //Translation. //This should be last transformation applied if we want total integration with blender.
        float trans1=0, trans2=0, trans3=0;
        inputFile >> trans1 >> trans2 >> trans3;
        bdm::Transform translation = translation.translate(bdm::Vector(trans1,trans2,trans3));
        
        //Rotation.
        float rot1=0, rot2=0, rot3=0;
        inputFile >> rot1 >> rot2 >> rot3;
        bdm::Transform rot_x = rot_x.rotate_x(rot1);
        bdm::Transform rot_y = rot_y.rotate_y(rot2);
        bdm::Transform rot_z = rot_z.rotate_z(rot3);
        
        //Scaling.
        float sc1=0, sc2=0, sc3=0;
        inputFile >> sc1 >> sc2 >> sc3;
        bdm::Transform scaling = scaling.scale(sc1,sc2,sc3);
        
        //Material.
        float a_r=0,a_g=0,a_b=0;
        inputFile >> a_r >> a_g >> a_b;
        float d_r=0,d_g=0,d_b=0;
        inputFile >> d_r >> d_g >> d_b;
        float s_r=0,s_g=0,s_b=0;
        inputFile >> s_r >> s_g >> s_b;
        float a[3] = {a_r,a_g,a_b};
        float d[3] = {d_r,d_g,d_b};
        float s[3] = {s_r,s_g,s_b};
        float exp=0,em=0;
        inputFile >> exp >> em;
        //inputFile >> temp_x >> temp_y >> temp_z >> rgbd >> discard_1 >> discard_2;    //Original.
        //inputFile >> temp_x >> temp_z >> temp_y >> rgbd >> discard_1 >> discard_2;    //Dataset RGB-D.
        u_int32_t n_sur=0;
        inputFile >> n_sur;
        
        data.reserve(data.size() + n_sur);
        
        for(int i = 0; i < n_sur; i++){                                                 //.bde file type.
            
            inputFile >> temp_x >> temp_y >> temp_z;    //Allocate full array size!!! Now we now the number of surfels.                                    
            
            //Aqui lectura color.!!!! ====TODO=====
            //rgbf = float(rgbd);
            //uint32_t rgbi = *(uint32_t*)&rgbf; 
            uint8_t r=0,g=0,b=0;
            
            /*r = (rgbi & 0xFF0000) >> 16;
             g = (rgbi & 0x00FF00) >> 8;
             b = (rgbi & 0x0000FF);*/
            
            bdm::Point new_p = bdm::Point(temp_x,temp_y,temp_z);
            
            new_p = rot_x(new_p);
            new_p = rot_y(new_p);
            new_p = rot_z(new_p);
            new_p = scaling(new_p);
            new_p = translation(new_p);
            
            //BRDF *p = new BRDFDiffuse();
            
            data.push_back(Surfel(new_p.x,new_p.y,new_p.z,r,g,b,radius,Material(a,d,s,exp,em))); //Blender -temp_z
            
        }
        
    }
    
    if (!inputFile.eof())
        if (inputFile.fail()) cout << "Type mismatch during parsing." << endl;
        else cout << "Unknow problem during parsing." << endl;
    
    //Close data file.
    inputFile.close();
    
    return data;
    
}




