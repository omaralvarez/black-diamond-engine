//
//  montecarlo.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 4/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "montecarlo.h"
#include "ray.h"
//Cambiar escena a puntero. Surfel puntero tambien.
std::vector<float> MonteCarlo::integrate(Scene *s, Surfel *surfel, int level) {
    
    std::vector<float> rgb(3,0);
    std::vector<float> sum_rgb(3,0);
    std::vector<bdm::Vector> samples(n_samples);
    float inv_samples = 1.f/n_samples;
    //Calculate samples for Monte Carlo integration.

    sampler.compute(*surfel, surfel->normal, samples);
    
    /*if (surfel.x <= -0.097  && surfel.x >= -0.098 && surfel.y >= 0.8596 && surfel.y <= 0.8597  && surfel.z >= 9.9332 && surfel.z <= 9.9333){ for (int i = 0; i < n_samples; i++) std::cout << samples[i].x << " " << samples[i].y << " " << samples[i].z << std::endl;
        std::cout << "-----------------" << std::endl;}*/
    /*std::cout << surfel.x << " " << surfel.y << " " << surfel.z << std::endl;
    std::cout << surfel.normal.x << " " << surfel.normal.y << " " << surfel.normal.z << std::endl;
    //std::cout << samples[i].x << " " << samples[i].y << " " << samples[i].z << std::endl;
    std::cout << "-----------------" << std::endl;*/
        
    for (int i = 0; i < n_samples; i++) {
        
        //For each sample check for intersections.
        //Interseccion con surfels en local. Convendria tener puntero?
        Ray ray = Ray(*surfel,samples[i],0.5f,INFINITY);
        Ray hit = s->kd_tree->intersect(ray);
        
        //Check if the sample intersects with any light.
        for (int j = 0; j < s->lights.size(); j++) s->lights[j].intersect(&ray);
        //std::cout << level <<"-------" << std::endl;
        //If the ray intersected with other surfels and they are closer than light intersection if any.
        if (hit.hit.radius != 0.f && hit.t_hit < ray.t_hit) {
            //std::cout << "Inter." << std::endl;
            
            //If it intersects with other surfels, calculate MC again.
            if (level < 1) {
                
                MonteCarlo new_mc = MonteCarlo();
                //todo.push_back(&new_mc);
                rgb = new_mc.integrate(s, &hit.hit,level+1);
                
            }
            
        } 
        
        //If the ray intersected with a light and is closer than the other intersections.
        if (ray.hit.radius != 0.f && hit.t_hit >= ray.t_hit) {
            //std::cout << "Light." << std::endl;
            rgb[0] = surfel->mat.diffuse[0];
            rgb[1] = surfel->mat.diffuse[1];
            rgb[2] = surfel->mat.diffuse[2];
            
        }
        
        //If the ray doesn't intersect with anything ambient contribution.
        if (ray.hit.radius == 0.f && hit.hit.radius == 0.f) {
            //std::cout << "Nothing." << std::endl;
            rgb[0] = surfel->mat.ambient[0];
            rgb[1] = surfel->mat.ambient[1];
            rgb[2] = surfel->mat.ambient[2];
            
        }
        //std::cout << level <<"*********" << std::endl;
        sum_rgb[0] += rgb[0];
        sum_rgb[1] += rgb[1];
        sum_rgb[2] += rgb[2];
        
    }
    
    //Return avg rgb values from all samples.
    sum_rgb[0] *= inv_samples;
    sum_rgb[1] *= inv_samples;
    sum_rgb[2] *= inv_samples;
    
    //Comprobar que no sean mayores de 255. Tema de acabar recursividad por profundidad que se pone en rgb.
    //if (sum_rgb[0] >255 || sum_rgb[1] > 255 || sum_rgb[2] > 255) std::cout << sum_rgb[0] << " " << sum_rgb[1] << " " << sum_rgb[2] << std::endl;
    return sum_rgb;
    
}