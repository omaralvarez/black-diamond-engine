//
//  bbox.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "bbox.h"

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

bool BBox::overlaps (BBox b) {
    
    bool x = (p_max.x >= b.p_min.x) && (p_min.x <= b.p_max.x);
    bool y = (p_max.y >= b.p_min.y) && (p_min.y <= b.p_max.y);
    bool z = (p_max.z >= b.p_min.z) && (p_min.z <= b.p_max.z);
    
    return (x && y && z);
    
}

int BBox::maximum_extent() {
    
    bdm::Vector diag = p_max - p_min;
    
    if (diag.x > diag.y && diag.x > diag.z) return 0;
    else if(diag.y > diag.z) return 1;
         else return 2;
    
}
