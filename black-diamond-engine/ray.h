/*
 *	ray.h
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

#ifndef black_diamond_engine_ray_h
#define black_diamond_engine_ray_h

#define RAY_EPSILON 1e-3f

#ifndef INFINITY            //Hay que mirar como ponerlo en bdm.
#define INFINITY FLT_MAX
#endif

#include "surfel.h"
#include <vector>

class Ray {
    
public:
    bdm::Point o;
    bdm::Vector d;
    float mint, maxt, t_hit;
    Surfel hit;
    //Calculo media cortes.
    std::vector<Surfel> hitlist;
    std::vector<float> hitlist_t;
    //--------
    
    //Creates a default ray from almost 0 to infinity.
    Ray() {
        o = bdm::Point(); d = bdm::Vector(); mint = RAY_EPSILON; maxt = INFINITY; hit = Surfel(); t_hit = INFINITY;
    }
    
    //Creates a ray with origin, direction and minimum t value and maximum t value. The hit is initialized to infinity.
    Ray(bdm::Point ori, bdm::Vector dir, float min, float max) {
        o = ori; d = dir; mint=min; maxt = max; hit = Surfel(); t_hit = INFINITY;
    }
    
    //Obtains the point from the parametric value t. 
    inline bdm::Point operator()(float t) {
        return o + d * t;
    }
    
};

#endif
