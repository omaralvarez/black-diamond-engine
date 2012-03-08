//
//  scene.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/15/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include "scene.h"

void Scene::get_normals() {
    
    int r = 2;
    float max_dist = 0.1f, min_dist = 1e-05f;
    float M[3][3] = { 0.f };
    
    unsigned long pct = cloud.size() / 100;
    bool flag = false;
    
    for (int i = 0; i < cloud.size(); i++) {
        Surfel p_i = cloud[i];
        //if (p_i.x == 0 && p_i.y <= -0.8883 && p_i.y >= -0.8884 && p_i.z >= 10.315 && p_i.z <= 10.316) flag = true;
        
        for (int j = 0; j < cloud.size(); j++) { //Create sum of neighbour distances: Sum( (pi-q) (pi-q)T theta(||pi-q||) )
            if (i == j) continue;
            Surfel p_j = cloud[j];
            //0.1 era el valor adecuado.
            if (fabsf(p_i.x - p_j.x) > max_dist || fabsf(p_i.y - p_j.y) > max_dist || fabsf(p_i.z - p_j.z) > max_dist) continue;
            
            //std::cout << i << " " << j << std::endl; //Diferencia entre manzana y mono signo de la z. La manzana es todo positivo en el mono hay combinacion de zs positivas y negativas.
            bdm::Vector dist = p_j - p_i;
            
            float d = dist.length();
            
            if (d > max_dist || d < min_dist) continue;
            
            //std::cout << dist.x << " " << dist.y << " " << dist.z << std::endl;
            float theta = 1.f/powf(d,r);
            //std::cout << theta << std::endl;
            //std::cout << "------------" << std::endl;
            
            M[0][0] += dist[0] * dist[0] * theta;
            M[0][1] += dist[0] * dist[1] * theta;
            M[0][2] += dist[0] * dist[2] * theta;
            
            M[1][0] += dist[1] * dist[0] * theta;
            M[1][1] += dist[1] * dist[1] * theta;
            M[1][2] += dist[1] * dist[2] * theta;
            
            M[2][0] += dist[2] * dist[0] * theta;
            M[2][1] += dist[2] * dist[1] * theta;
            M[2][2] += dist[2] * dist[2] * theta;
        }
        
        if (i % pct == 0) {
            std::cout << i / pct << "%" << std::endl;
        }
        
        if (flag){
        for (int fil = 0; fil < 3; fil++) 
            for (int col = 0; col < 3; col++) 
                std::cout << M[fil][col] << std::endl;
        
            std::cout << "------------" << std::endl;}
        
        //Determinant calculation.
        float a = -1.f,b,c,d; 
        
        b = M[0][0] + M[1][1] + M[2][2];
        c = M[1][0] * M[1][0] + M[2][0] * M[2][0] + M[2][1] * M[2][1] - M[0][0] * M[1][1] - M[0][0] * M[2][2] - M[1][1] * M[2][2];
        d = M[0][0] * M[1][1] * M[2][2] - M[0][0] * M[2][1] * M[2][1] - M[1][0] * M[1][0] * M[2][2] + M[1][0] * M[0][2] * M[2][1] +
            M[1][0] * M[0][2] * M[2][1] - M[0][2] * M[0][2] * M[1][1];
        
        //std::cout << a << " " << b << " " << c << " " << d << std::endl;
        
        //Equation resolution.
        float x,y,z;
        
        x = (((3.f*c)/a) - ((b*b)/(a*a)))/3.f;
        y = (((2.f*b*b*b)/(a*a*a)) - ((9.f * b * c)/(a*a)) + ((27.f * d)/a))/27.f;
        z = (y*y)/4.f + (x * x * x)/27.f;
        
        //std::cout << x << " " << y << " " << z << " " << std::endl;
        
        float k,l,m,n,o,p;
        
        k = sqrtf((y * y) / 4.f - z);
        l = powf(k,(1.f/3.f));
        m = acosf(-(y/(2.f*k)));
        n = cosf(m/3.f);
        o = sqrtf(3.f)*sinf(m/3.f);
        p = -b/(3.f*a);
        
        //std::cout << k << " " << l << " " << 1e-04 << " " << n << " " << o << " " << p << std::endl;
        
        //Equation roots that are the eigenvalues.
        float eig1,eig2,eig3;
        
        eig1 = 2.f*l*n + p;
        eig2 = -l * (n + o) + p;
        eig3 = -l * (n - o) + p;
        
        if (flag) std::cout << p_i.x << " " << p_i.y << " " << p_i.z << std::endl;
        if (flag) std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        if (fabsf(eig1) < 0.01) eig1 = MAXFLOAT;
        if (fabsf(eig2) < 0.01) eig2 = MAXFLOAT;
        if (fabsf(eig3) < 0.01) eig3 = MAXFLOAT;
        //std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        
        
        
        float min_eig = fminf(fminf(eig1,eig2),eig3);
        //std::cout << min_eig << std::endl;
        
        M[0][0] -= min_eig;
        M[1][1] -= min_eig;
        M[2][2] -= min_eig;
        
        u_int8_t col0 = 0, col1 = 0, col2 = 0;
        
        for (int it = 0; it < 3; it++) {
            if (fabsf(M[it][0]) <= 0.0001) col0++;
            if (fabsf(M[it][1]) <= 0.0001) col1++;
            if (fabsf(M[it][2]) <= 0.0001) col2++;
        } 
        
        /*col1 = fabsf(M[0][1]) + fabsf(M[1][1]) + fabsf(M[2][1]);
        col2 = fabsf(M[0][2]) + fabsf(M[1][2]) + fabsf(M[2][2]);*/
        
        float n_x = 0, n_y = 0, n_z = 0;
        
        if (col0 < col1 && col0 < col2) {
            
            //cloud[i].normal = bdm::Vector(-1,0,0);
            //std::cout << "Entra 1" << std::endl;
            n_x = M[1][1] * M[2][2] - M[1][2] * M[2][1];
            n_y = -(M[1][0] * M[2][2] - M[1][2] * M[2][0]);
            n_z = M[1][0] * M[2][1] - M[1][1] * M[2][0];
            
        } else if (col1 < col0 && col1 < col2) {
            
            //cloud[i].normal = bdm::Vector(0,-1,0);
            //std::cout << "Entra 2" << std::endl;
            n_x = M[0][1] * M[2][2] - M[0][2] * M[2][1];
            n_y = -(M[0][0] * M[2][2] - M[0][2] * M[2][0]);
            n_z = M[0][0] * M[2][1] - M[0][1] * M[2][0];
            
        } else {
            
            n_x = M[0][1] * M[1][2] - M[0][2] * M[1][1];
            n_y = -(M[0][0] * M[1][2] - M[1][0] * M[0][2]);
            n_z = M[0][0] * M[1][1] - M[0][1] * M[1][0];
            
        }
            
            
            
            if (n_z > 0) {
                cloud[i].normal = -(bdm::Vector(n_x,n_y,n_z).normalize());
            } else {
                cloud[i].normal = bdm::Vector(n_x,n_y,n_z).normalize();
            }
            
        
        
        //std::cout << cloud[i].normal.x << " " << cloud[i].normal.y << " " << cloud[i].normal.z << std::endl;
        
        //std::cout << eig1 << " " << eig2 << " " << eig3 << " " << std::endl;
        
        /*for (int i = 0; i < 3; i++) 
            for (int j = 0; j < 3; j++) 
                std::cout << M[i][j] << std::endl;*/
        
        //std::cout << "------------" << std::endl;
        
        for (int i = 0; i < 3; i++)  //Mirar si la matriz hay que reiniciarla en los continues.
            for (int j = 0; j < 3; j++) 
                M[i][j] = 0.f;
        
        flag = false;
        
    }
    
    
}

void Scene::get_normals_accel() {
    
    int r = 2;
    float max_dist = 0.1f, min_dist = 1e-05f;//0.07 r4 cara atras orejas.
    float M[3][3] = { 0.f };
    
    unsigned long pct = cloud.size() / 100;
    bool flag = false;
   
    for (int i = 0; i < cloud.size(); i++) {
        
        //std::cout <<"i: " << i << std::endl;
        
        Surfel p_i = cloud[i];
        u_int32_t *n_neighbours = (u_int32_t *) malloc(sizeof(u_int32_t));
        *n_neighbours = 0;
        Surfel **neighbours = kd_tree->get_neighbours(cloud[i], max_dist, n_neighbours);
        
        //std::cout << "Neig: " << *n_neighbours << std::endl;
        
        for (int j = 0; j < *n_neighbours; j++) { //Create sum of neighbour distances: Sum( (pi-q) (pi-q)T theta(||pi-q||) )

            Surfel *p_j = neighbours[j];

            //0.1 era el valor adecuado.
            if (fabsf(p_i.x - p_j->x) > max_dist || fabsf(p_i.y - p_j->y) > max_dist || fabsf(p_i.z - p_j->z) > max_dist) continue;
            //std::cout << p_j->x << " " << p_j->y << " " << p_j->z << std::endl; 
            //std::cout << i << " " << j << std::endl; //Diferencia entre manzana y mono signo de la z. La manzana es todo positivo en el mono hay combinacion de zs positivas y negativas.
            bdm::Vector dist = *p_j - p_i;//Cuidado que ahora tenemos punteros...!!!!
            
            float d = dist.length();
            
            //if (d > max_dist || d < min_dist) continue;
            if (d > max_dist || d < min_dist) continue;
            
            //std::cout << d << std::endl;
            float theta = 1.f/powf(d,r);
            //std::cout << theta << std::endl;
            //std::cout << "------------" << std::endl;
            
            M[0][0] += dist[0] * dist[0] * theta;
            M[0][1] += dist[0] * dist[1] * theta;
            M[0][2] += dist[0] * dist[2] * theta;
            
            M[1][0] += dist[1] * dist[0] * theta;
            M[1][1] += dist[1] * dist[1] * theta;
            M[1][2] += dist[1] * dist[2] * theta;
            
            M[2][0] += dist[2] * dist[0] * theta;
            M[2][1] += dist[2] * dist[1] * theta;
            M[2][2] += dist[2] * dist[2] * theta;
        }
        
        if (i % pct == 0) {
         std::cout << i / pct << "%" << std::endl;
        }
        
        if (flag){
            for (int fil = 0; fil < 3; fil++) 
                for (int col = 0; col < 3; col++) 
                    std::cout << M[fil][col] << std::endl;
            
            std::cout << "------------" << std::endl;}
        
        //Determinant calculation.
        float a = -1.f,b,c,d; 
        
        b = M[0][0] + M[1][1] + M[2][2];
        c = M[1][0] * M[1][0] + M[2][0] * M[2][0] + M[2][1] * M[2][1] - M[0][0] * M[1][1] - M[0][0] * M[2][2] - M[1][1] * M[2][2];
        d = M[0][0] * M[1][1] * M[2][2] - M[0][0] * M[2][1] * M[2][1] - M[1][0] * M[1][0] * M[2][2] + M[1][0] * M[0][2] * M[2][1] +
        M[1][0] * M[0][2] * M[2][1] - M[0][2] * M[0][2] * M[1][1];
        
        //std::cout << a << " " << b << " " << c << " " << d << std::endl;
        
        //Equation resolution.
        float x,y,z;
        
        x = (((3.f*c)/a) - ((b*b)/(a*a)))/3.f;
        y = (((2.f*b*b*b)/(a*a*a)) - ((9.f * b * c)/(a*a)) + ((27.f * d)/a))/27.f;
        z = (y*y)/4.f + (x * x * x)/27.f;
        
        //std::cout << x << " " << y << " " << z << " " << std::endl;
        
        float k,l,m,n,o,p;
        
        k = sqrtf((y * y) / 4.f - z);
        l = powf(k,(1.f/3.f));
        m = acosf(-(y/(2.f*k)));
        n = cosf(m/3.f);
        o = sqrtf(3.f)*sinf(m/3.f);
        p = -b/(3.f*a);
        
        //std::cout << k << " " << l << " " << 1e-04 << " " << n << " " << o << " " << p << std::endl;
        
        //Equation roots that are the eigenvalues.
        float eig1,eig2,eig3;
        
        eig1 = 2.f*l*n + p;
        eig2 = -l * (n + o) + p;
        eig3 = -l * (n - o) + p;
        
        //if (flag) std::cout << p_i.x << " " << p_i.y << " " << p_i.z << std::endl;
        //if (flag) std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        if (fabsf(eig1) < 0.01) eig1 = MAXFLOAT;
        if (fabsf(eig2) < 0.01) eig2 = MAXFLOAT;
        if (fabsf(eig3) < 0.01) eig3 = MAXFLOAT;
        //std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        
        
        
        float min_eig = fminf(fminf(eig1,eig2),eig3);
        //std::cout << min_eig << std::endl;
        
        M[0][0] -= min_eig;
        M[1][1] -= min_eig;
        M[2][2] -= min_eig;
        
        u_int8_t col0 = 0, col1 = 0, col2 = 0;
        
        for (int it = 0; it < 3; it++) {
            if (fabsf(M[it][0]) <= 0.0001) col0++;
            if (fabsf(M[it][1]) <= 0.0001) col1++;
            if (fabsf(M[it][2]) <= 0.0001) col2++;
        } 
        
        /*col1 = fabsf(M[0][1]) + fabsf(M[1][1]) + fabsf(M[2][1]);
         col2 = fabsf(M[0][2]) + fabsf(M[1][2]) + fabsf(M[2][2]);*/
        
        float n_x = 0, n_y = 0, n_z = 0;
        
        if (col0 < col1 && col0 < col2) {
            
            //cloud[i].normal = bdm::Vector(-1,0,0);
            //std::cout << "Entra 1" << std::endl;
            n_x = M[1][1] * M[2][2] - M[1][2] * M[2][1];
            n_y = -(M[1][0] * M[2][2] - M[1][2] * M[2][0]);
            n_z = M[1][0] * M[2][1] - M[1][1] * M[2][0];
            
        } else if (col1 < col0 && col1 < col2) {
            
            //cloud[i].normal = bdm::Vector(0,-1,0);
            //std::cout << "Entra 2" << std::endl;
            n_x = M[0][1] * M[2][2] - M[0][2] * M[2][1];
            n_y = -(M[0][0] * M[2][2] - M[0][2] * M[2][0]);
            n_z = M[0][0] * M[2][1] - M[0][1] * M[2][0];
            
        } else {
            
            n_x = M[0][1] * M[1][2] - M[0][2] * M[1][1];
            n_y = -(M[0][0] * M[1][2] - M[1][0] * M[0][2]);
            n_z = M[0][0] * M[1][1] - M[0][1] * M[1][0];
            
        }
        
        //std::cout << n_x << " " << n_y << " " << n_z << std::endl;
        
        if (n_z > 0) {
            kd_tree->surfels[i].normal = -(bdm::Vector(n_x,n_y,n_z).normalize());
        } else {
            kd_tree->surfels[i].normal = bdm::Vector(n_x,n_y,n_z).normalize();
        }
        
        
        
        //std::cout << cloud[i].normal.x << " " << cloud[i].normal.y << " " << cloud[i].normal.z << std::endl;
        
        //std::cout << eig1 << " " << eig2 << " " << eig3 << " " << std::endl;
        
        /*for (int i = 0; i < 3; i++) 
         for (int j = 0; j < 3; j++) 
         std::cout << M[i][j] << std::endl;*/
        
        //std::cout << "------------" << std::endl;
        
        for (int i = 0; i < 3; i++)  //Mirar si la matriz hay que reiniciarla en los continues.
            for (int j = 0; j < 3; j++) 
                M[i][j] = 0.f;
        
        flag = false;
     
        free(neighbours);
        free(n_neighbours);
        
    }
    
}
