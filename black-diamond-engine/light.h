/*
 *	light.h
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

#ifndef black_diamond_engine_light_h
#define black_diamond_engine_light_h

#include "point.h"
#include "vector.h"

#define PI 3.14159265f

class Light {
    
public:
    virtual float sample_l(bdm::Point p, float p_epsilon, float time, bdm::Vector wi, float pdf) {
        return 0;
    }
    
    virtual float power() {
        return 0;
    }
    
    virtual bool is_delta_light() {
        return false;
    }
    
};

#endif
