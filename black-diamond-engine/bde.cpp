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

int main (int argc, const char * argv[])
{
    
    using namespace std;
    
    char filePath[200] = "/Users/osurfer3/Dropbox/PFC/Datasets/urban_scenes_sketchup/urban_scenes_sketchup/3d_man1/3d_man1-1.txt";
    
    ifstream inputFile;
    
    inputFile.open(filePath);
    
    vector<float> data(0);
    float temp,temp2,temp3;
    
    while (inputFile.good()){
        
        inputFile >> temp >> temp2 >> temp3;
        
        cout << temp << " " << temp2 << " " << temp3 << endl;
        
        data.push_back(temp);
        
    }
    
    cout << "Primera lectura: " << temp << endl;
    
    for (int i=0; i<data.size(); i++) {
        //cout << i << ". " << data[i] << endl;
    }
    
    return 0;
}

