/*
 *	surfel.cpp
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

#include <iostream>
#include "ray.h"
#include "surfel.h"
#include "bdesettings.h"

extern BDESettings settings;

//Checks if a ray intersects with a surfel.
bool Surfel::intersect(Ray *ray) {
    
    if (settings.normal_est && !mat.emit) {
        
        //Disk intersection.
        
        //ray->d.x =5;ray->d.y =4;ray->d.z =6;
        //ray->o.x =0;ray->o.y =0;ray->o.z =0;
        //std::cout<<"point: " <<x<<" "<<y<<" "<<z << std::endl;
        //x=4;y=3;z=5;
        //normal=bdm::Vector(0,1,-1);
        //radius=5;
        float denom = ray->d.dot(normal);
        
        //std::cout<< denom << std::endl;
        
        if (fabsf(denom) > 0.f) {
            
            float t;
            float numer = (*this - ray->o).dot(normal);
            //std::cout<<"num: " <<numer << std::endl;
            t = numer/denom;
            //std::cout<<"t: " <<t << std::endl;
            bdm::Point int_point = (*ray)(t);
            //std::cout<<"point: " <<int_point.x<<" "<<int_point.y<<" "<<int_point.z << std::endl;
            float sq = powf(int_point.x - x,2) + powf(int_point.y - y,2) + powf(int_point.z - z,2);
            //std::cout<<"sq: " << powf(radius,2)<<std::endl;
            //std::cout<<"point: " <<x<<" "<<y<<" "<<z << std::endl;
            //std::cout << ray->mint<<std::endl;
            //Calculo media cortes.
            if(sq <= radius*radius && t > ray->mint) {
                //std::cout << "Enter." << std::endl;
                ray->hitlist.push_back(*this);
                ray->hitlist_t.push_back(t);
            }
            //std::cout << "* "<<ray->hitlist.size() << std::endl;
            //if (ray->hitlist.size()>1) std::cout <<"* "<< ray->hitlist.size() << std::endl;
            //---------
            if (t < ray->t_hit && t > ray->mint && sq <= radius*radius) { //Separar en 3 bools para ver que esta pasando con rendimiento.
                ray->hit = *this;
                ray->t_hit = t;
                return true;
            }
            
        } 
        
        return false;
        
    } else {
        
        //Sphere intersection.
        //std::cout << "Entra???" << std::endl;
        bdm::Vector dis = ray->o - *this;
        float A = ray->d.dot(ray->d);
        float B = dis.dot(ray->d);
        float C = dis.dot(dis) - radius*radius;//Cambiar ray->o - this. Reutilizado varias veces.====PERF====
        
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
                ray->hit = *this;
                ray->t_hit = t;
                return true;
            }
            
        }
        
        return false;
        
    }
}
