/*
 *	matrix4x4.h
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
        inline Matrix4x4 transpose() {
            
            return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                             m[0][1], m[1][1], m[2][1], m[3][1],
                             m[0][2], m[1][2], m[2][2], m[3][2],
                             m[0][3], m[1][3], m[2][3], m[3][3]);
            
        }
        Matrix4x4 inverse();
        
    };
    
}

#endif
