//
//  surfel.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/3/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "ray.h"
#include "surfel.h"

bool Surfel::intersect(Ray ray) {
    
    float A = ray.d.dot(ray.d);
    float B = (ray.o - *this).dot(ray.d);
    float C = (ray.o - *this).dot(ray.o - *this) - powf(radius,2.f);
    
    float disc = B*B - A*C;
    
    if (disc < 0.f) {
        //std::cout << "Ray miss" << std::endl;
    } else {
        
        float root = sqrtf(disc);
        float t;
        
        if (root >=0 && root < 0.0000001) t = -B/A;
        else {
            t = (-B - root)/A; //En principio la t mas pequeña deberia ser siempre esta. Pero hay que comprobarlo.
            
            //float t2 = (-B + root)/A; 
            
            //std::cout << t1 << " " << t2 << std::endl;
        }
        
        if (t < ray.t_hit) {
            ray.hit = *this;
            ray.t_hit = t;
        }
        
        return true;
        
    }
    
    return false;
    
}
