//
//  bbox.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "bbox.h"

//Returns new bounding box that contains the point p.
BBox BBox::uni(bdm::Point p){
    
    BBox ret = BBox(); //Remember this used to be called with a BBox, not creating one.
    
    ret.p_min.x = fminf(p_min.x, p.x);
    ret.p_min.y = fminf(p_min.y, p.y);
    ret.p_min.z = fminf(p_min.z, p.z);
    
    ret.p_max.x = fmaxf(p_max.x, p.x);
    ret.p_max.y = fmaxf(p_max.y, p.y);
    ret.p_max.z = fmaxf(p_max.z, p.z);
    
    return ret;
    
}

//Returns new bounding box that covers the two bounding boxes.
BBox BBox::uni(BBox b2) {
    
    BBox ret = BBox(); //Remember this used to be called with a BBox, not creating one.
    
    ret.p_min.x = fminf(p_min.x, b2.p_min.x);
    ret.p_min.y = fminf(p_min.y, b2.p_min.y);
    ret.p_min.z = fminf(p_min.z, b2.p_min.z);
    
    ret.p_max.x = fmaxf(p_max.x, b2.p_max.x);
    ret.p_max.y = fmaxf(p_max.y, b2.p_max.y);
    ret.p_max.z = fmaxf(p_max.z, b2.p_max.z);
    
    return ret;
    
}

//Returns true if bounding boxes overlap.
bool BBox::overlaps (BBox b) {
    
    bool x = (p_max.x >= b.p_min.x) && (p_min.x <= b.p_max.x);
    bool y = (p_max.y >= b.p_min.y) && (p_min.y <= b.p_max.y);
    bool z = (p_max.z >= b.p_min.z) && (p_min.z <= b.p_max.z);
    
    return (x && y && z);
    
}

//Returns longest axis.
int BBox::maximum_extent() {
    
    bdm::Vector diag = p_max - p_min;
    
    if (diag.x > diag.y && diag.x > diag.z) return 0;
    else if(diag.y > diag.z) return 1;
         else return 2;
    
}

//Returns true if the ray intersects with the bounding box.
bool BBox::intersect_p(Ray *ray, float *hit0, float *hit1) {
    
    float t0 = ray->mint, t1 = ray->maxt;
        
    for (int i = 0; i < 3; i++) {
        
        float inv_ray_dir = 1.f / ray->d[i];
        float tnear = (p_min[i] - ray->o[i]) * inv_ray_dir;
        float tfar  = (p_max[i] - ray->o[i]) * inv_ray_dir;
        
        if (tnear > tfar) std::swap(tnear,tfar);
        t0 = tnear > t0 ? tnear : t0;
        t1 = tfar < t1 ? tfar : t1;
        if (t0 > t1) return false;
        
    }
    
    if (hit0) *hit0 = t0;
    if (hit1) *hit1 = t1;
    
    return true;
    
}
