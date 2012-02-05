//
//  point.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

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
        
        Point operator+(Vector v){
            return Point(x + v.x, y + v.y, z + v.z);
        }
        
        Point operator+=(Vector v){
            x += v.x; y += v.y; z += v.z;
            return *this;
        }
        
        Vector operator-(Point p){
            return Vector(x - p.x, y - p.y, z - p.z);
        }
        
        Point operator-(Vector v){
            return Point(x - v.x, y - v.y, z - v.z);
        }
        
        Point operator-=(Vector v){
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }
        
        Point operator/(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            return Point(x * inv, y * inv, z * inv);
        }
        
        Point operator/=(float f) {
            assert(f!=0);
            float inv = 1.f / f;
            x *= inv; y *= inv; z *= inv;
            return *this;
        }
        
    };
    
}

#endif
