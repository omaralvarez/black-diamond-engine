/*
 *	material.cpp
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
    
    /*~Material() {
            //delete brdf;
    }*/
    
};

#endif
