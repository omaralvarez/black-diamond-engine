//
//  transform.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "transform.h"
#include "vector.h"

Transform::Transform(){
    
    m = mInv = Matrix4x4();
    
}

Transform::Transform(float mat[4][4]){
    
    m = Matrix4x4(mat);
    mInv = m.inverse();
    
}

Transform::Transform(Matrix4x4 mat){
    
    m = mat;
    mInv = m.inverse();
    
}

Transform::Transform(Matrix4x4 mat, Matrix4x4 minv){
    
    m = mat;
    mInv = minv;
    
}

Transform Transform::get_inverse(){
    
    return Transform(m,mInv);
    
}

Transform Transform::look_at(Point pos, Point look, Vector up){
    
    float mat[4][4];
    
    //Initialize column 4 to camera position.
    mat[0][3] = pos.x;
    mat[1][3] = pos.y;
    mat[2][3] = pos.z;
    mat[3][3] = 1;
    
    //Initialize rest of columns.
    Vector dir = (look - pos).normalize();
    Vector right = dir.cross(up.normalize());
    Vector new_up = right.cross(dir);
    
    mat[0][0] = right.x;
    mat[1][0] = right.y;
    mat[2][0] = right.z;
    mat[3][0] = 0.f;
    
    mat[0][1] = new_up.x;
    mat[1][1] = new_up.y;
    mat[2][1] = new_up.z;
    mat[3][1] = 0.f;
    
    mat[0][2] = dir.x;
    mat[1][2] = dir.y;
    mat[2][2] = dir.z;
    mat[3][2] = 0.f;
    
    Matrix4x4 cam_to_world = Matrix4x4(mat);
    
    return Transform(cam_to_world.inverse(),cam_to_world);
    
}

Transform Transform::scale(float x, float y, float z){
    
    Matrix4x4 mat = Matrix4x4(x, 0, 0, 0,
                              0, y, 0, 0,
                              0, 0, z, 0,
                              0, 0, 0, 1);
    
    Matrix4x4 mat_inv = Matrix4x4(1.f/x, 0, 0, 0,
                                  0, 1.f/y, 0, 0,
                                  0, 0, 1.f/z, 0,
                                  0, 0, 0, 1);
    
    return Transform(mat,mat_inv);
    
}

Transform Transform::translate(Vector delta){
    
    Matrix4x4 mat = Matrix4x4(1, 0, 0, delta.x,
                              0, 1, 0, delta.y,
                              0, 0, 1, delta.z,
                              0, 0, 0, 1);
    
    Matrix4x4 mat_inv = Matrix4x4(1, 0, 0, -delta.x,
                                  0, 1, 0, -delta.y,
                                  0, 0, 1, -delta.z,
                                  0, 0, 0, 1);
    
    return Transform(mat,mat_inv);
    
}

Transform Transform::orthographic(float znear, float zfar){
    
    return scale(1.f, 1.f, 1.f/(zfar-znear)) * translate(Vector(0.f,0.f,-znear));
    
}

Point Transform::operator()(Point pt){
    
    float x = pt.x; float y = pt.y; float z = pt.z;
    float xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
    float yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
    float zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
    float wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
    
    assert(wp!=0);
    if(wp == 1.) return Point(xp,yp,zp,pt.color);
    else return Point(xp,yp,zp,pt.color)/wp;
    
}

