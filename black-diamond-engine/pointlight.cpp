//
//  pointlight.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/24/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "pointlight.h"
#include "point.h"
#include "vector.h"

float PointLight::sample_l(bdm::Point p, float p_epsilon, float time, bdm::Vector wi, float pdf) {
    
    wi = (light_pos - p).normalize();
    pdf = 1.f;
    
    return intensity/(light_pos - p).length_sqrd();
    
}

/*bool PointLight::intersect(Ray *ray) {
    
    float radius = 1.6f;
    
    bdm::Vector dis = ray->o - light_pos;
    float A = ray->d.dot(ray->d);
    float B = dis.dot(ray->d);
    float C = dis.dot(dis) - radius*radius;
    
    float disc = B*B - A*C;
    
    if (disc < 0.f) {
        //std::cout << "Ray miss" << std::endl;
    } else {
        
        float root = sqrtf(disc);
        float t;
        
        if (root >=0.f && root < 0.0000001f) t = -B/A;
        else {
            t = (-B - root)/A; //En principio la t mas pequeña deberia ser siempre esta. Pero hay que comprobarlo.
            
            //float t2 = (-B + root)/A; 
            
        }
        
        if (t < ray->t_hit && t > ray->mint) {
            ray->hit = &Surfel(light_pos,0,0,0,1.f); //Gotta change this and check in montecarlo for intersection with t_hit < infinity.
            ray->t_hit = t;
            return true;
        }
        
    }
    
    return false;
    
}*/