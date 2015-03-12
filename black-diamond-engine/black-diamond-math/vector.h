/*
 *	vector.h
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
#ifndef black_diamond_engine_vector_h
#define black_diamond_engine_vector_h

#include <cmath>
#include <assert.h>

namespace bdm {

    class Vector {
        
    public:
        float x,y,z;
        
        Vector(float _x=0, float _y=0, float _z=0)
        : x(_x), y(_y), z(_z) {}
        
        inline Vector operator+(Vector v) {
            return Vector(x + v.x, y + v.y, z + v.z);
        }
        
        inline Vector operator+=(Vector v) {
            x += v.x; y += v.y; z += v.z;
            return *this;
        }
        
        inline Vector operator-(Vector v) {
            return Vector(x - v.x, y - v.y, z - v.z);
        }
        
        inline Vector operator-=(Vector v) {
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }
        
        inline Vector operator-() {
            return Vector(-x,-y,-z);
        }
        
        inline Vector operator/(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            return Vector(x * inv, y * inv, z * inv);
        }
        
        inline Vector operator/=(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            x *= inv; y *= inv; z *= inv;
            return *this;
        }
        
        inline float length_sqrd() {
            return x*x + y*y + z*z;
        }
        
        inline float length() {
            return sqrtf(length_sqrd());
        }
        
        inline Vector normalize() {
            if (x == 0 && y == 0 && z == 0) return *this;  
            else return *this/length();
        }
        
        inline Vector operator*(float f) {
            return Vector(f*x, f*y, f*z);
        }
        
        inline Vector operator*=(float f) {
            x *= f; y *= f; z *= f;
            return *this;
        }
        
        inline Vector cross(Vector v2){
            return Vector((y * v2.z) - (z * v2.y), (z * v2.x) - (x * v2.z), (x * v2.y) - (y * v2.x));
        }
        
        inline float dot(Vector v2) {
            return x * v2.x + y * v2.y + z * v2.z;
        }
        
        inline float operator[](int i) {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }
                
    };
    
}

#endif
