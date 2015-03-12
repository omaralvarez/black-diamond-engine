/*
 *	transform.cpp
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

#include <iostream>
#include "transform.h"
#include "vector.h"

namespace bdm {
    
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
    
    Transform Transform::rotate_x(float angle) {
        
        float sin_t = sinf(angle);
        float cos_t = cosf(angle);
        
        Matrix4x4 m = Matrix4x4(1, 0, 0, 0,
                                0, cos_t, -sin_t, 0,
                                0, sin_t, cos_t, 0,
                                0, 0, 0, 1);
        
        return Transform(m,m.transpose());
        
    }
    
    Transform Transform::rotate_y(float angle) {
        
        float sin_t = sinf(angle);
        float cos_t = cosf(angle);
        
        Matrix4x4 m = Matrix4x4(cos_t, 0, sin_t, 0,
                                0, 1, 0, 0,
                                -sin_t, 0, cos_t, 0,
                                0, 0, 0, 1);
        
        return Transform(m,m.transpose());
        
    }
    
    Transform Transform::rotate_z(float angle) {
        
        float sin_t = sinf(angle);
        float cos_t = cosf(angle);
        
        Matrix4x4 m = Matrix4x4(cos_t, -sin_t, 0, 0,
                                sin_t, cos_t, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1);
        
        return Transform(m,m.transpose());
        
    }
    
    Transform Transform::rotate(Vector v1, Vector v2) {
        
        Vector v1_n = v1.normalize();
        Vector v2_n = v2.normalize();
        Vector a = v1.cross(v2).normalize();
        
        float s = v1_n.cross(v2_n).length();
        float c = v1_n.dot(v2_n);
        float m[4][4];
        
        m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
        m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
        m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
        m[0][3] = 0;
        
        m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
        m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
        m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
        m[1][3] = 0;
        
        m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
        m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
        m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
        m[2][3] = 0;
        
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
        
        Matrix4x4 mat(m);
        
        return Transform(mat,mat.transpose());
        
    }

}