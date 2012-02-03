//
//  matrix4x4.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_matrix4x4_h
#define black_diamond_engine_matrix4x4_h

namespace bdm {
    
    class Matrix4x4 {
        
    public:
        float m[4][4];
        Matrix4x4();
        Matrix4x4(float mat[4][4]);
        Matrix4x4(float t00, float t01, float t02, float t03,
                  float t10, float t11, float t12, float t13,
                  float t20, float t21, float t22, float t23,
                  float t30, float t31, float t32, float t33);
        Matrix4x4 operator*(Matrix4x4 m2);
        Matrix4x4 transpose();
        Matrix4x4 inverse();
        
    };
    
}

#endif
