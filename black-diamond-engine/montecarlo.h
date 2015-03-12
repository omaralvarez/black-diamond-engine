/*
 *	montecarlo.h
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

#ifndef black_diamond_engine_montecarlo_h
#define black_diamond_engine_montecarlo_h

#include "sampler.h"
#include "surfel.h"
#include "scene.h"
#include <vector>

class MonteCarlo {
    
    int n_samples;
    //Sampler sampler;
    //std::vector<MonteCarlo *> todo;
    
public:
    MonteCarlo() {
        n_samples = 60;
        //todo.resize(0);
    }
    
    std::vector<float> integrate(Scene *s, Ray *view, int level); 
    
};

#endif
