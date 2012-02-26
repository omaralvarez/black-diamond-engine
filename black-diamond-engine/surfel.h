//
//  surfel.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/3/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_surfel_h
#define black_diamond_engine_surfel_h

#include "point.h"
#include "material.h"

class Ray;

class Surfel : public bdm::Point {
  
public:
    float r,g,b;
    Material mat;
    float radius;
    
    Surfel() : Point() {
        
        r = 0; g = 0; b = 0; radius = 0; mat = Material();
        
    }
    
    Surfel(float xp, float yp, float zp, float rs, float gs, float bs, float rad) : Point(xp,yp,zp) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = Material();
        
    }
    
    Surfel(float xp, float yp, float zp, float rs, float gs, float bs, float rad, Material m) : Point(xp,yp,zp) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = m;
        
    }
    
    Surfel(bdm::Point p, float rs, float gs, float bs, float rad) : bdm::Point(p.x,p.y,p.z) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = Material();
        
    }
    
    Surfel(bdm::Point p, float rs, float gs, float bs, float rad, Material m) : bdm::Point(p.x,p.y,p.z) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = m;
        
    }
    
    bool intersect(Ray *ray);
    
};

#endif
