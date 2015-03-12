/*
 *	point.h
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
#ifndef black_diamond_engine_point_h
#define black_diamond_engine_point_h

#include "vector.h"
namespace bdm {
    
    class Point {
    
    public:
        float x,y,z;    //Point position.
        
        //Constructor.
        Point(float _x=0, float _y=0, float _z=0)
        : x(_x), y(_y), z(_z) {}
        
        inline Point operator+(Point p){
            return Point(x + p.x, y + p.y, z + p.z);
        }
        
        inline Point operator+=(Point p){
            x += p.x; y += p.y; z += p.z;
            return *this;
        }
        
        //Poner const al final si no toca nada en el objeto local.
        inline Point operator+(Vector v) {
            return Point(x + v.x, y + v.y, z + v.z);
        }
        
        inline Point operator+=(Vector v){
            x += v.x; y += v.y; z += v.z;
            return *this;
        }
        
        inline Vector operator-(Point p){
            return Vector(x - p.x, y - p.y, z - p.z);
        }
        
        inline Point operator-(Vector v){
            return Point(x - v.x, y - v.y, z - v.z);
        }
        
        inline Point operator-=(Vector v){
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }
        
        inline Point operator/(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            return Point(x * inv, y * inv, z * inv);
        }
        
        inline Point operator/=(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            x *= inv; y *= inv; z *= inv;
            return *this;
        }
        
        inline Point operator*(float f) {
            return Point(f*x, f*y, f*z);
        }
        
        inline Point operator*=(float f) {
            x *= f; y *= f; z *= f;
            return *this;
        }

        inline float operator[](int i) const {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }
        
        inline float &operator[](int i) {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }
        
    };
    
}

#endif
