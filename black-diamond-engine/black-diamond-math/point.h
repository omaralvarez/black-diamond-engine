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
        
        inline Point operator+(Point p){
            return Point(x + p.x, y + p.y, z + p.z);
        }
        
        inline Point operator+=(Point p){
            x += p.x; y += p.y; z += p.z;
            return *this;
        }
        
        inline Point operator+(Vector v){
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
