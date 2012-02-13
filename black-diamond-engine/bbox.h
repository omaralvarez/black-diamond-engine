//
//  bbox.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_bbox_h
#define black_diamond_engine_bbox_h

#include "point.h"
//#include <iostream>
//#include <algorithm>

class BBox {
    
public:
    bdm::Point p_min,p_max;
    
    BBox(){
        p_min = bdm::Point(INFINITY,INFINITY,INFINITY);
        p_max = bdm::Point(-INFINITY,-INFINITY,-INFINITY);
    }
    
    BBox(bdm::Point p){
        p_min = p;
        p_max = p;
    }
    
    BBox(bdm::Point p1, bdm::Point p2){
        p_min = bdm::Point(fminf(p1.x,p2.x),fminf(p1.y,p2.y),fminf(p1.z,p2.z));
        p_max = bdm::Point(fmaxf(p1.x,p2.x),fmaxf(p1.y,p2.y),fmaxf(p1.z,p2.z));
    }
    
    BBox uni(bdm::Point p);
    
    bool overlaps (BBox b);
    
    bool inside(bdm::Point pt) {
        
        return (pt.x >= p_min.x && pt.x <= p_max.x &&
                pt.y >= p_min.y && pt.y <= p_max.y &&
                pt.z >= p_min.z && pt.z <= p_max.z);
        
    }
    
    void expand(float delta) {
        
        p_min -= bdm::Vector(delta,delta,delta); 
        p_max += bdm::Vector(delta,delta,delta);
        
    }
    
    float volume() {
        
        bdm::Vector d = p_max - p_min;
        
        return d.x * d.y * d.z;
        
    }
    
    int maximum_extent();
    
    void bounding_sphere(bdm::Point *c, float *rad) {
        
        *c = p_min * .5f + p_max * .5f;
        *rad = (*c-p_max).length();
        
    }
    
};

#endif
