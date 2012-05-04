//
//  vector.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_vector_h
#define black_diamond_engine_vector_h

#include <math.h>
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
