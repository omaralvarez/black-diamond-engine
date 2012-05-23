//
//  material.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/25/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_material_h
#define black_diamond_engine_material_h

//#include "brdf.h"

class Material {
    
public:
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float exp;
    float emit;
    //BRDF *brdf;
    
    //Creates a default material.
    Material() {
        
        for (int i = 0; i < 3; i++) {
            ambient[i] = 0;
            diffuse[i] = 0;
            specular[i] = 0;
        }
        
        exp = 0;
        emit = 0;
        
    }
    
    //Creates a material with ambient color, specular color and exponent for phong shading.
    Material(float amb[3], float dif[3], float spec[3],float e, float em) {
        
        for (int i = 0; i < 3; i++) {
            ambient[i] = amb[i];
            diffuse[i] = dif[i];
            specular[i] = spec[i];
        }
        
        exp = e;
        emit = em;

    }
    
};

#endif
