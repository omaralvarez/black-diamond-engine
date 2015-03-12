/*
 *	transform.h
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
#ifndef black_diamond_engine_transform_h
#define black_diamond_engine_transform_h

#include "matrix4x4.h"
#include "point.h"
#include "vector.h"

namespace bdm {
    
    class Transform {
        
        Matrix4x4 m,mInv;
        
    public:
        Transform();
        Transform(float mat[4][4]);
        Transform(Matrix4x4 mat);
        Transform(Matrix4x4 mat, Matrix4x4 minv);
        inline Transform get_inverse() {
            
            return Transform(mInv,m);
            
        }
        Transform look_at(Point pos, Point look, Vector up);
        Transform scale(float x, float y, float z);
        Transform translate(Vector delta);
        Transform orthographic(float znear, float zfar) {
            
            return scale(1.f, 1.f, 1.f/(zfar-znear)) * translate(Vector(0.f,0.f,-znear));
            
        }
        Transform rotate_x(float angle);
        Transform rotate_y(float angle);
        Transform rotate_z(float angle);
        Transform rotate(Vector axis, Vector vec);
        inline Point operator()(Point pt) {
            
            float x = pt.x; float y = pt.y; float z = pt.z;
            float xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
            float yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
            float zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
            float wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
            
            assert(wp!=0);
            if(wp == 1.) return Point(xp,yp,zp);
            else return Point(xp,yp,zp)/wp;
            
        }
        inline Vector operator()(Vector v){
            
            float x = v.x, y = v.y, z = v.z;
            
            return Vector(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
                          m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
                          m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
            
        }
        Transform operator*(Transform t2) {
            
            Matrix4x4 m1 = m*t2.m;
            Matrix4x4 m2 = t2.mInv*mInv;
            
            return Transform(m1,m2);
            
        }
        
    };
    
}

#endif
