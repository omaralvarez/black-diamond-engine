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
#include "bdesettings.h"

extern BDESettings settings;

bool Surfel::intersect(Ray *ray) {
    
    if (settings.normal_est) {
        
        //ray->d.x =5;ray->d.y =4;ray->d.z =6;
        //ray->o.x =0;ray->o.y =0;ray->o.z =0;
        //std::cout<<"point: " <<x<<" "<<y<<" "<<z << std::endl;
        //x=4;y=3;z=5;
        //normal=bdm::Vector(0,1,-1);
        //radius=5;
        float denom = ray->d.dot(normal);
        
        //std::cout<< denom << std::endl;
        
        if (fabsf(denom) > 0) {
            
            float t;
            float numer = (*this - ray->o).dot(normal);
            //std::cout<<"num: " <<numer << std::endl;
            t = numer/denom;
            //std::cout<<"t: " <<t << std::endl;
            bdm::Point int_point = (*ray)(t);
            //std::cout<<"point: " <<int_point.x<<" "<<int_point.y<<" "<<int_point.z << std::endl;
            float sq = powf(int_point.x - x,2) + powf(int_point.y - y,2) + powf(int_point.z - z,2);
            //std::cout<<"sq: " << powf(radius,2)<<std::endl;
            if (t < ray->t_hit && t > ray->mint && sq <= radius*radius) {
                ray->hit = *this;
                ray->t_hit = t;
                return true;
            }
            
        } 
        
        return false;
        
    } else {
    
        float A = ray->d.dot(ray->d);
        float B = (ray->o - *this).dot(ray->d);
        float C = (ray->o - *this).dot(ray->o - *this) - powf(radius,2.f);
        
        float disc = B*B - A*C;
        
        //======TODO======== Tener en cuenta el tmin y tmax.
        
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
                ray->hit = *this;
                ray->t_hit = t;
                return true;
            }
            
        }
        
        return false;
        
    }
}
