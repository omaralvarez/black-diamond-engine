//
//  transform.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_transform_h
#define black_diamond_engine_transform_h

#include "matrix4x4.h"
#include "point.h"
#include "vector.h"

class Transform {
    
    Matrix4x4 m,mInv;
    
public:
    Transform();
    Transform(float mat[4][4]);
    Transform(Matrix4x4 mat);
    Transform(Matrix4x4 mat, Matrix4x4 minv);
    Transform get_inverse();
    Transform look_at(Point pos, Point look, Vector up);
    Transform scale(float x, float y, float z);
    Transform translate(Vector delta);
    Transform orthographic(float znear, float zfar);
    Point operator()(Point pt);  
    Transform operator*(Transform t2) {
        
        Matrix4x4 m1 = m*t2.m;
        Matrix4x4 m2 = t2.mInv*mInv;
        
        return Transform(m1,m2);
        
    }
};

#endif
