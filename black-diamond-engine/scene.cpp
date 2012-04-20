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
#include "bdesettings.h"

extern BDESettings settings;

//Estimates normals without k-d tree.
void Scene::get_normals() {
    
    int r = 2;
    float max_dist = 0.05f, min_dist = 1e-04f;
    
    r = settings.r; max_dist = settings.max_dist; min_dist = settings.min_dist; //Read settings.

    float M[3][3] = { 0.f };
    
    unsigned long pct = cloud.size() / 100;
    bool flag = false;
    
    for (int i = 0; i < cloud.size(); i++) {
        Surfel p_i = cloud[i];
        //if (p_i.x <= -0.222 && p_i.x >= -0.2223 && p_i.y <= -0.7777 && p_i.y >= -0.7778 && p_i.z == 5) flag = true;
        //if (p_i.x >= 1.4953 && p_i.x <= 1.4954 && p_i.y >= 0.625 && p_i.y <= 0.626 && p_i.z >= 6.4701 && p_i.z <= 6.4702) flag = true;
        //if (p_i.x == -0.7222 && p_i.y == -0.8333  && p_i.z == 5) flag = true;
        double mean_dist = 0; //For surfel radius estimation.
        double weight_neigh = 0;
        for (int j = 0; j < cloud.size(); j++) { //Create sum of neighbour distances: Sum( (pi-q) (pi-q)T theta(||pi-q||) )
            if (i == j) continue;
            Surfel p_j = cloud[j];
            //0.1 era el valor adecuado.
            if (fabsf(p_i.x - p_j.x) > max_dist || fabsf(p_i.y - p_j.y) > max_dist || fabsf(p_i.z - p_j.z) > max_dist) continue;
            
            //std::cout << i << " " << j << std::endl; //Diferencia entre manzana y mono signo de la z. La manzana es todo positivo en el mono hay combinacion de zs positivas y negativas.
            bdm::Vector dist = p_j - p_i;
            
            float d = dist.length();
            
            if (d > max_dist || d < min_dist) continue;
            
            mean_dist += d; //Weighted arithmetic mean.
            
            //std::cout << p_j.x << " " << p_j.y << " " << p_j.z << std::endl;
            float theta = 1.f/powf(d,r);
            //weight_neigh += 1.f/d; //Weighted arithmetic mean.
            weight_neigh++;
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
        
        /*if (i % pct == 0) {
            std::cout << i / pct << "%" << std::endl;
        }*/
        
        if (flag){
        std::cout << "------------" << std::endl;
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
        
        //if (flag) std::cout << x << " " << y << " " << z << " " << std::endl;
        
        float k,l,m,n,o,p;
        
        k = sqrtf((y * y) / 4.f - z);
        l = powf(k,(1.f/3.f));
        float aux = 0;
        if(-(y/(2.f*k))<-1) aux = -1;
        else if(-(y/(2.f*k))>1) aux = 1;
        else aux = -(y/(2.f*k));
        m = acosf(aux); //Problema nan. Solucion problema acosf(-1.00000001) tiene que estar entre [-1,1]. Redondeando a int -(y/(2.f*k)) lo hace bien.
        n = cosf(m/3.f);
        o = sqrtf(3.f)*sinf(m/3.f);
        p = -b/(3.f*a);
        
        //if (flag)if(-(y/(2.f*k))<-1) std::cout << "Se cumple."<<std::endl;
        
        //if (flag) std::cout << k << " " << l << " " << m << " " << n << " " << o << " " << p << std::endl;
        
        //Equation roots that are the eigenvalues.
        float eig1,eig2,eig3;
        
        eig1 = 2.f*l*n + p;
        eig2 = -l * (n + o) + p;
        eig3 = -l * (n - o) + p;
        
        //if (flag) std::cout << p_i.x << " " << p_i.y << " " << p_i.z << std::endl;
        //std::cout << "Eig: "<< eig1 << " " << eig2 << " " << eig3 << std::endl;
        /*if (fabsf(eig1) < min_dist) eig1 = MAXFLOAT;
        if (fabsf(eig2) < min_dist) eig2 = MAXFLOAT;
        if (fabsf(eig3) < min_dist) eig3 = MAXFLOAT;*/
        //std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        if (flag)std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        if (flag) std::cout << p_i.x << " " << p_i.y << " " << p_i.z << std::endl;
        
        float min_eig = fminf(fminf(eig1,eig2),eig3);
        if (flag)std::cout << min_eig << std::endl;
        
        M[0][0] -= min_eig;
        M[1][1] -= min_eig;
        M[2][2] -= min_eig;
        
        u_int8_t col0 = 0, col1 = 0, col2 = 0;
        //if (flag)std::cout << col0 << " " << col1 << " " << col2 << std::endl;
        
        //Checks for 0s in the matrix for eigenvector calculation.
        for (int it = 0; it < 3; it++) {
            if (fabsf(M[it][0]) <= min_dist) {M[it][0]=0;col0++;}
            if (fabsf(M[it][1]) <= min_dist) {M[it][1]=0;col1++;}
            if (fabsf(M[it][2]) <= min_dist) {M[it][2]=0;col2++;}
        } 
        
        /*col1 = fabsf(M[0][1]) + fabsf(M[1][1]) + fabsf(M[2][1]);
        col2 = fabsf(M[0][2]) + fabsf(M[1][2]) + fabsf(M[2][2]);*/
        //if (flag)std::cout << col0 << " " << col1 << " " << col2 << std::endl;
                
        float n_x = 0, n_y = 0, n_z = 0;
        
        /*if (fabsf(M[0][1]) <= min_dist && fabsf(M[0][2]) <= min_dist && fabsf(M[1][2]) <= min_dist) {
            
            
            if (M[0][0] <= M[1][1] && M[0][0] <= M[2][2]) {n_x=1;n_y=0;n_z=0;}
            if (M[1][1] <= M[0][0] && M[1][1] <= M[2][2]) {n_x=0;n_y=1;n_z=0;}
            if (M[2][2] <= M[0][0] && M[2][2] <= M[1][1]) {n_x=0;n_y=0;n_z=1;}
            
            
        } else {*/
            //If there are more than two 0s you cannot solve the system from this row. 
            //Then checks if its independent if not returns (0,0,0). Then checks to solve from the next row.
            if (col0 < 2) {
                n_x = M[1][1] * M[2][2] - M[1][2] * M[2][1];
                n_y = -(M[1][0] * M[2][2] - M[1][2] * M[2][0]);
                n_z = M[1][0] * M[2][1] - M[1][1] * M[2][0];
            }
            if (fabsf(n_x) <= min_dist && fabsf(n_y) <= min_dist && fabsf(n_z) <= min_dist && col1 < 2) {
            
                n_x = M[0][1] * M[2][2] - M[0][2] * M[2][1];
                n_y = -(M[0][0] * M[2][2] - M[0][2] * M[2][0]);
                n_z = M[0][0] * M[2][1] - M[0][1] * M[2][0];
            
            } 
            
            if (fabsf(n_x) <= min_dist && fabsf(n_y) <= min_dist && fabsf(n_z) <= min_dist && col2 < 2) {
                
                n_x = M[0][1] * M[1][2] - M[0][2] * M[1][1];
                n_y = -(M[0][0] * M[1][2] - M[1][0] * M[0][2]);
                n_z = M[0][0] * M[1][1] - M[0][1] * M[1][0];
                
            }
            
        //}
            
        //If it is not able to solve the system then there is at least one row with all 0s.
        if (n_x == 0 && n_y == 0 && n_z == 0) {
            if (col0 == 3) n_x = 1;
            if (col1 == 3) n_y = 1;
            if (col2 == 3) n_z = 1;
        }
            
        if(flag)std::cout << "Norms: " << n_x << " " << n_y << " " << n_z << std::endl;
        
        //Checks the normal orientation if its opposite to the camera flips it.
        if (n_z > 0) {
            cloud[i].normal = -(bdm::Vector(n_x,n_y,n_z).normalize());
            //std::cout << "Entr" << std::endl;
        } else {
            cloud[i].normal = bdm::Vector(n_x,n_y,n_z).normalize();
        }
           
        /*if (cloud[i].normal.z != -1) {
            std::cout << "Point: " << cloud[i].x << " " << cloud[i].y << " " << cloud[i].z << std::endl;
            std::cout << cloud[i].normal.x << " " << cloud[i].normal.y << " " << cloud[i].normal.z << std::endl;
        }*/
        
        //Auto radius calculation.
        if(!cloud[i].radius) cloud[i].radius = float((mean_dist/weight_neigh)*3/4);
        //if(!cloud[i].radius) cloud[i].radius = float((mean_dist/weight_neigh)/2);
        //std::cout << num_neigh << std::endl;
        
        //std::cout << cloud[i].normal.x << " " << cloud[i].normal.y << " " << cloud[i].normal.z << std::endl;
        if(flag)std::cout << "Point: " << cloud[i].x << " " << cloud[i].y << " " << cloud[i].z << std::endl;
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

//Estimates normals using the k-d tree. Creates a little error.
//===TODO=== Actualizar con cambios de arriba.
void Scene::get_normals_accel() {
    
    int r = 2;
    float max_dist = 0.1f, min_dist = 1e-04f;//0.07 r4 cara atras orejas.
    
    r = settings.r; max_dist = settings.max_dist; min_dist = settings.min_dist;
    
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
            //if (fabsf(p_i.x - p_j->x) > max_dist || fabsf(p_i.y - p_j->y) > max_dist || fabsf(p_i.z - p_j->z) > max_dist) continue;
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
        /*if (fabsf(eig1) < min_dist) eig1 = MAXFLOAT;
        if (fabsf(eig2) < min_dist) eig2 = MAXFLOAT;
        if (fabsf(eig3) < min_dist) eig3 = MAXFLOAT;*/
        //std::cout << eig1 << " " << eig2 << " " << eig3 << std::endl;
        
        
        
        float min_eig = fminf(fminf(eig1,eig2),eig3);
        //std::cout << min_eig << std::endl;
        
        M[0][0] -= min_eig;
        M[1][1] -= min_eig;
        M[2][2] -= min_eig;
        
        u_int8_t col0 = 0, col1 = 0, col2 = 0;
        
        for (int it = 0; it < 3; it++) {
            if (fabsf(M[it][0]) <= min_dist) col0++;
            if (fabsf(M[it][1]) <= min_dist) col1++;
            if (fabsf(M[it][2]) <= min_dist) col2++;
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
