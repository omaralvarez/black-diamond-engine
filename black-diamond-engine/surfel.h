/*
 *	surfel.h
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

#ifndef black_diamond_engine_surfel_h
#define black_diamond_engine_surfel_h

#include "point.h"
#include "material.h"

class Ray;

class Surfel : public bdm::Point {
  
public:
    float r,g,b;
    Material mat;
    float radius;
    bdm::Vector normal;
    
    Surfel() : Point() {
        
        r = 0; g = 0; b = 0; radius = 0; mat = Material(); normal = bdm::Vector();
        
    }
    
    Surfel(float xp, float yp, float zp, float rs, float gs, float bs, float rad) : Point(xp,yp,zp) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = Material(); normal = bdm::Vector();
        
    }
    
    Surfel(float xp, float yp, float zp, float rs, float gs, float bs, float rad, Material m) : Point(xp,yp,zp) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = m; normal = bdm::Vector();
        
    }
    
    Surfel(float xp, float yp, float zp, float rs, float gs, float bs, float rad, Material m, bdm::Vector n) : Point(xp,yp,zp) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = m; normal = n;
        
    }
    
    Surfel(bdm::Point p, float rs, float gs, float bs, float rad) : bdm::Point(p.x,p.y,p.z) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = Material(); normal = bdm::Vector();
        
    }
    
    Surfel(bdm::Point p, float rs, float gs, float bs, float rad, Material m) : bdm::Point(p.x,p.y,p.z) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = m; normal = bdm::Vector();
        
    }
    
    Surfel(bdm::Point p, float rs, float gs, float bs, float rad, Material m, bdm::Vector n) : bdm::Point(p.x,p.y,p.z) {
        
        r = rs; g = gs; b = bs; radius = rad; mat = m; normal = n;
        
    }
    
    bool intersect(Ray *ray);
    
};

#endif
