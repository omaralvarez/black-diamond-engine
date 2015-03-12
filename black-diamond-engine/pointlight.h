/*
 *	pointlight.h
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

#ifndef black_diamond_engine_pointlight_h
#define black_diamond_engine_pointlight_h

#include "light.h"
#include "point.h"
#include "vector.h"
#include "ray.h"

class PointLight : public Light {
    
    float intensity;
    
public:
    bdm::Point light_pos;
    
    PointLight() {
        
        light_pos = bdm::Point(0,0,0);
        intensity = 0;
        
    }
    
    PointLight(bdm::Point p,float intens) {
        
        light_pos = p;
        intensity = intens;
        
    }
    
    float sample_l(bdm::Point p, float p_epsilon, float time, bdm::Vector wi, float pdf);
    
    float power() {
        return 4.f * PI * intensity;
    }
    bool is_delta_light() {
        return true;
    }
    
    bool intersect(Ray *ray);
    
};

#endif
