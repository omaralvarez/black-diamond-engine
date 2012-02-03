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
        
        Vector operator/(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            return Vector(x * inv, y * inv, z * inv);
        }
        
        Vector operator/=(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            x *= inv; y *= inv; z *= inv;
            return *this;
        }
        
        float length_sqrd() {
            return x*x + y*y + z*z;
        }
        
        float length() {
            return sqrtf(length_sqrd());
        }
        
        Vector normalize() {
            return *this/length();
        }
        
        Vector cross(Vector v2){
            
            return Vector((y * v2.z) - (z * v2.y), (z * v2.x) - (x * v2.z), (x * v2.y) - (y * v2.x));
            
        }
        
    };
    
}

#endif
