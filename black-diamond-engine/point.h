//
//  point.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_point_h
#define black_diamond_engine_point_h

class Point {
    
public:
    float x,y,z;
    float color; 
    
    Point(float _x=0, float _y=0, float _z=0)
        : x(_x), y(_y), z(_z) {}
    
};

#endif
