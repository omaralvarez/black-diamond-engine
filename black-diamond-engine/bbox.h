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
#include "ray.h"

class BBox {
    
public:
    bdm::Point p_min,p_max;
    
    //Default bounding box.
    BBox(){
        p_min = bdm::Point(INFINITY,INFINITY,INFINITY);
        p_max = bdm::Point(-INFINITY,-INFINITY,-INFINITY);
    }
    
    BBox(bdm::Point p){
        p_min = p;
        p_max = p;
    }
    
    //Bounding box from point radius r.
    BBox(bdm::Point p,float r){
        p_min = bdm::Point(p.x - r,p.y - r,p.z - r);
        p_max = bdm::Point(p.x + r,p.y + r,p.z + r);
    }
    
    //Bounding box from two points that represent the lower left and the upper right points (the furthest points).
    BBox(bdm::Point p1, bdm::Point p2){
        p_min = bdm::Point(fminf(p1.x,p2.x),fminf(p1.y,p2.y),fminf(p1.z,p2.z));
        p_max = bdm::Point(fmaxf(p1.x,p2.x),fmaxf(p1.y,p2.y),fmaxf(p1.z,p2.z));
    }
    
    BBox uni(bdm::Point p);
    
    BBox uni(BBox b2);
    
    bool overlaps (BBox b);
    
    //Returns true if the point is inside the bounding box.
    bool inside(bdm::Point pt) {
        
        return (pt.x >= p_min.x && pt.x <= p_max.x &&
                pt.y >= p_min.y && pt.y <= p_max.y &&
                pt.z >= p_min.z && pt.z <= p_max.z);
        
    }
    
    //Expands the bounding box.
    void expand(float delta) {
        
        p_min -= bdm::Vector(delta,delta,delta); 
        p_max += bdm::Vector(delta,delta,delta);
        
    }
    
    //Returns the bounding box volume.
    float volume() {
        
        bdm::Vector d = p_max - p_min;
        
        return d.x * d.y * d.z;
        
    }
    
    int maximum_extent();
    
    //Returns center and radius of a bounding sphere.
    void bounding_sphere(bdm::Point *c, float *rad) {
        
        *c = p_min * .5f + p_max * .5f;
        *rad = (*c-p_max).length();
        
    }
    
    bool intersect_p(Ray ray, float *hit1, float *hit2);
    
};

#endif
