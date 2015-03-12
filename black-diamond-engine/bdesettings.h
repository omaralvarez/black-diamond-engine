/*
 *	bdesettings.h
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

#ifndef black_diamond_engine_bdesettings_h
#define black_diamond_engine_bdesettings_h

class BDESettings {
  
public:
    //Anti Aliasing
    bool AA;
    
    //Normal estimation.
    bool normal_est;
    bool est_accel;
    int r;
    float max_dist,min_dist;

    //Ray Tracing.
    bool kd_accel;
    int icost,tcost,maxs,max_depth;
    float ebonus;
    
    BDESettings() {
        
        AA = false;
        normal_est = false;
        est_accel = false;
        r = 2; max_dist = 0.1f; min_dist = 1e-04f;
        kd_accel = true;
        icost = 80; tcost = 1; ebonus = 0.5f; maxs = 1000; max_depth = -1;
        
    }
    
};

#endif
